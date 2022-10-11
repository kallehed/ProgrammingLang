#pragma once
#include <unordered_map>

#include "Name.h"
#include "misc.h"

class Scope {
    std::unordered_map<Name, int, Name::HashFunction> _vars;
    
    Scope(const Scope&) = delete;
public:
    Scope() {};

    // add new variable
    void add_var(Name name, int value);

    // get var value from name
    int get_value(Name name);

    // set var to value
    void set_value(Name name, int value);

    // get from text
    Value_And_Where get_var_value(Where w);
};