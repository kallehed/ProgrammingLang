#pragma once

#include <iostream>

#include "Name.h"

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

	// assume it starts with ", returns length of string
	static int print_string(const char* const text);

	static void print_x_chars_of(const char* const text, int length);
};