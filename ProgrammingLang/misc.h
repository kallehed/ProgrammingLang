#pragma once

constexpr long long SIZE = 128;
constexpr int space_per_indent = 4;

struct Value_And_Length {
    int value;
    int length;
};

namespace OPER {
    enum _ : char {
        PLUS,
        MULT,
        MORE_THAN,
        LESS_THAN,
        MINUS,
    };
}

struct Result_And_If_Nextline_And_Exits {
    int result;
    bool should_nextline;
    int exits;
};

int use_oper(OPER::_ oper, int start_value, int new_value);