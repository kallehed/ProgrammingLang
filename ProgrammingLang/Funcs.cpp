#include "Funcs.h"

void Funcs::add_func(Name name, std::array<Name, MAX_FUNC_PARAMS> param_names, Where w)
{
    Func f;
    f._param_names = param_names;
    f._w = w;
    f._scope;
    //_funcs.insert({ name , {name, param_names} });
    _funcs[name] = f;
}

Func& Funcs::get_func(Name name) {
    if (_funcs.find(name) == _funcs.end()) {
        for (;;) {}
    }
    else {
        return _funcs[name];
    }
}

void Func::reset_names(std::array<Name, MAX_FUNC_PARAMS>& names)
{
    for (int i = 0; i < MAX_FUNC_PARAMS; ++i) {
        names[i].number = 0;
    }
}
