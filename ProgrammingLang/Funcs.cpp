#include "Funcs.h"

void Funcs::add_func(Name name, Name param_name, std::streampos pos) {
    _funcs.insert({ name, {param_name, pos} });
}

Func Funcs::get_func(Name name) {
    if (_funcs.find(name) == _funcs.end()) {
        for (;;) {}
    }
    else {
        return _funcs[name];
    }
}