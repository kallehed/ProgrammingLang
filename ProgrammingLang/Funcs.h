#pragma once
#include <unordered_map>

#include "Name.h"

struct Func {
    Name param_name;
    std::streampos pos;
};

class Funcs
{
	std::unordered_map<Name, Func, Name::HashFunction> _funcs;

	Funcs(const Funcs&) = delete;

public:

    Funcs() = default;

    void add_func(Name name, Name param_name, std::streampos pos);

    Func get_func(Name name);
};

