#include "Funcs.h"

void Funcs::add_func(Name name, std::array<Name, MAX_FUNC_PARAMS> param_names, std::streampos pos)
{
    _funcs.insert({ name, {param_names, pos} });
}

const Func& Funcs::get_func(Name name) {
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
