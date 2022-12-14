#pragma once

constexpr long long SIZE = 128;
// maxmimum chars in code file
constexpr int MAX_CODE_LEN = 32768;
constexpr int SPACE_PER_INDENT = 4;

constexpr int MAX_FUNC_PARAMS = 8;

extern char* const code;

// index of _code we are at
using Where = int;

struct Value_And_Where {
    int _value;
    Where _w;
};

struct Value_And_Length {
    int _value;
    int _length;
};

// contains where line starts and how long it is AND it's indent
struct Line {
    Where _w; // where does the line start EXACTLY 
    int _indent; // will be -1 for illegal line
};

namespace OPER {
    enum _ : char {
        PLUS,
        MULT,
        MORE_THAN,
        LESS_THAN,
        MINUS,
        MOD,
        EQUAL,
    };
}

struct Result_And_Exit {
    int _result;
    bool _exit;
};

int use_oper(OPER::_ oper, int start_value, int new_value);