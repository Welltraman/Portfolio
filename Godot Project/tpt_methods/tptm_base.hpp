#pragma once
#include "../resource_manager.hpp"

class TPTMethodBase;
struct TPTMethodFactory {
	typedef Map<String, TPTMethodBase* (*)()> Method;
	static TPTMethodBase* get(const String& a_name) {
		Method::Element* E = getMap()->find(a_name);
		return (E) ? E->value()() : 0;
	}
	~TPTMethodFactory() { memdelete(m_method); }
protected:
	static Method* getMap() {
		if (!m_method) { m_method = memnew(Method); }
		return m_method;
	}
private:
	static Method* m_method;
};

template<typename T> TPTMethodBase* create() { return memnew(T); }
template<typename T> struct TPTRegister : TPTMethodFactory {
	TPTRegister(const String &a_name) { getMap()->insert(a_name, &create<T>); }
};

class TPTProcess;
class TPTMethodBase : public Reference {
	GDCLASS(TPTMethodBase, Reference);
protected:
	List<String> m_tags;
	TPTProcess* m_process;
	virtual void _end() {}
	virtual void _pause() {}
	virtual void _resume() {}
	virtual void _run() {}
	virtual bool _verify_values(Array &r_val) const { return false; }
	static void _bind_methods() {
		ADD_SIGNAL(MethodInfo("method_ended", PropertyInfo(Variant::OBJECT, "tpt_method")));
	}
public:
	bool delay = false;
	Vector<String> targets;
	Array values;
	bool operator==(const TPTMethodBase &rhs) { return get_instance_id() == rhs.get_instance_id(); }
	void end();
	virtual Array get_default_value() const { return Array(); }
	void pause() { _pause(); }
	void resume() { _resume(); }
	void run(TPTProcess *p_process);
	void set_tag(const String &a_tag) { m_tags.push_back(a_tag); }
	bool has_tag(const String &a_tag) { return m_tags.find(a_tag); }
	void remove_tag(const String &a_tag) { m_tags.erase(a_tag); }
};
