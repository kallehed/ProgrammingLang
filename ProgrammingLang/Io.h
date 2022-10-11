#pragma once

#include <iostream>

#include "Name.h"

#include "misc.h"

#ifdef _DEBUG
#define DEBUG_MSG(str) do { std::cout << str; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

class Scope;

class Io {
public:
	// prints name and value
	static void print_var_details(Name name, Scope& scope);

	// assume it starts with ", returns where string ends: "asd"Here
	static Where print_string(Where w);

	static void print_x_chars_of(const char* const text, int length);
};