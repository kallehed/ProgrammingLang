#pragma once
#include <unordered_map>

#include "Name.h"
#include "misc.h"
#include "Scope.h"

struct Func {
    std::array<Name, MAX_FUNC_PARAMS> _param_names;
    int _line;

    // personal scope for function
    Scope _scope;

    static void reset_names(std::array<Name, MAX_FUNC_PARAMS>& names);
};

class Funcs
{
	std::unordered_map<Name, Func, Name::HashFunction> _funcs;

	Funcs(const Funcs&) = delete;

public:

    Funcs() = default;

    void add_func(Name name, std::array<Name, MAX_FUNC_PARAMS> param_names, int line);

    Func& get_func(Name name);
};

