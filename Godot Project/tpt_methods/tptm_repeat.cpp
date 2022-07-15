#include "tptm_repeat.hpp"
#include "../tpt_handler.hpp"

TPTRegister<TPTMRepeat> TPTMRepeat::m_reg("repeat");

bool TPTMRepeat::_verify_values(Array &r_val) const {
	if (delay)
		return r_val.size() == 1 && r_val[0].get_type() == Variant::INT;
	else {
		if (r_val[0].get_type() == Variant::INT)
			r_val[0] = false;
		return r_val.size() == 1 && (r_val[0].get_type() == Variant::STRING || r_val[0].get_type() == Variant::BOOL);
	}
}

void TPTMRepeat::_run() {
	if (delay)
		_beginning_repeat();
	else
		_closing_repeat();
	end();
}

void TPTMRepeat::_beginning_repeat() {
	repeat_amount = values[0];
	Ref<TPTMRepeat> closing = _get_repeat_pair();
	closing->pair = Ref<TPTMRepeat>(this);
	closing->repeat_amount = values[0];
	if (has_tag("duplicate")) {
		for (List<Ref<TPTMethodBase>>::Element *E = m_process->get_methods()->find(this); E; E = E->next()) {
			E->get()->remove_tag("duplicate");
			if (E->get() == closing) break;
		}
	}
}

void TPTMRepeat::_closing_repeat() {
	if (repeat_amount > 0 && !values[0]) {
		List<Ref<TPTMethodBase>>::Element *method = m_process->get_methods()->find(pair);
		Ref<TPTMethodBase> last_method = this;
		do {
			method = method->next();
			if (method->get()->has_tag("duplicate")) continue;
			Ref<TPTMethodBase> copy = _create_copy(method->get());
			m_process->add_method_after(copy, last_method);
			last_method = copy;
		} while (method->get() != this);
	}
}

Ref<TPTMethodBase> TPTMRepeat::_create_copy(const Ref<TPTMethodBase> p_base) {
	String id = String(typeid(**p_base).name()).trim_prefix("class TPTM").to_lower();
	Ref<TPTMethodBase> copy = TPTMethodFactory::get(id);
	copy->delay = p_base->delay;
	copy->targets = p_base->targets;
	copy->values = p_base->values;
	copy->set_tag("duplicate");
	if (typeid(**p_base) == typeid(*this) && !p_base->delay && ((Ref<TPTMRepeat>)p_base)->pair == pair) {
		((Ref<TPTMRepeat>)copy)->pair = pair;
		((Ref<TPTMRepeat>)copy)->repeat_amount = repeat_amount - 1;
	}
	return copy;
}

Ref<TPTMRepeat> TPTMRepeat::_get_repeat_pair() {
	List<Ref<TPTMethodBase>> *methods = m_process->get_methods();
	int nest = 0;
	for (List<Ref<TPTMethodBase>>::Element *E = methods->find(this)->next(); E; E = E->next()) {
		if (typeid(**E->get()) == typeid(*this)) {
			if (E->get()->delay) nest++;
			else if (nest) nest--;
			else return E->get();
		}
	}
	return NULL;
}

Array TPTMRepeat::get_default_value() const {
	Array array;
	array.push_back(repeat_amount);
	return array;
}
