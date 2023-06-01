#pragma once
#include <map>
#include <string>

template <typename T, typename ... A>
class BaseFactory {
public:
    virtual T* create(const A& ... args) = 0;

};

/// @brief abstract factory that calls other factories
/// @tparam T = type to construct
/// @tparam ...A = argument types used to construct the object
template <typename T, typename ... A>
class Factory : public BaseFactory<T, A ...> {

    using FactoryMap = std::map<const std::string, BaseFactory<T, A ...>*>;

protected:
    _FORCE_INLINE_ static FactoryMap& get_factory() {
        static FactoryMap map;
        return map;
    }

public:
    _FORCE_INLINE_ static T* get(const std::string& p_id, const A& ... args) {
        // if the id exists, construct the object
        if (get_factory().count(p_id)) {
            return get_factory()[p_id]->create(args ...);
        }
        return nullptr;
    }

    ~Factory() {
        // delete all the registered factories
        for (auto it = get_factory().begin(); it != get_factory().end(); it++) {
            delete it->second;
        }
    }
};