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

Value_And_Length Scope::get_var_value(const char* const text)
{
    auto [name, length] = Name_Util::extract_name(text);
    int value = this->get_value(name);
    return { value, length };
}