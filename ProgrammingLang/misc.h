#pragma once

constexpr long long SIZE = 128;
// maxmimum chars in code file
constexpr int MAX_CODE_LEN = 32768;
constexpr int space_per_indent = 4;

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