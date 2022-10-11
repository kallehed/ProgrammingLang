#include "Scope.h"

void Scope::add_var(Name name, int value)
{
    _vars.insert({ name, value });
}

int Scope::get_value(Name name) {
    if (_vars.find(name) == _vars.end()) {
        for ( ;; ) {}
    }
    else {
        return _vars[name];
    }
}

void Scope::set_value(Name name, int value) {
    _vars[name] = value;
}

Value_And_Where Scope::get_var_value(Where w)
{
    auto extract = Name_Util::extract_name(w);
    int value = this->get_value(extract._name);
    return { value, extract._w };
}