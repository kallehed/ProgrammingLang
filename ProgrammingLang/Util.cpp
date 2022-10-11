#include "Util.h"

// a raised to b
int Util::ipow(int a, int b) {
    int result = 1;
    for (int i = 0; i < b; ++i) {
        result *= a;
    }
    return result;
}

// starts at [1]
int Util::where_is_char(Where w, char looking_for) {
    while (true) {
        ++w;
        if (code[w] == looking_for) {
            return w;
        }
    }
}

bool Util::is_num(char possible_num) { return possible_num >= '0' && possible_num <= '9'; }

// first assumed to be correct, returns where condition is NOT true first.
Where Util::where_while_condition(Where w, bool (*cond)(char)) {
    while (cond(code[w])) {
        ++w;
    }
    return w;
}

// we know the first char is a num
Value_And_Where Util::extract_num(Where w)
{
    int end_w = Util::where_while_condition(w, &Util::is_num);
    int val = 0;
    for (; w < end_w; ++w) {
        val += (code[w] - '0') * Util::ipow(10, end_w - w - 1);
    }
    return { val, end_w };
}

bool Util::legal_name_char(char c) { return (c >= 'A' && c <= 'z'); }

bool Util::str_equal(const char* const str1, const char* str2, int len) {
    for (int i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) { return false; }
    }
    return true;
}