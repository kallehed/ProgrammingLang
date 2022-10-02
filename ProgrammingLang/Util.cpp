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
int Util::len_to_char(const char* const text, char looking_for) {
    int i = 1;
    while (true) {
        if (text[i] == looking_for) {
            return i;
        }
        ++i;
    }
}

bool Util::is_num(char possible_num) { return possible_num >= '0' && possible_num <= '9'; }

// first assumed to be correct
int Util::len_while_condition(const char* const text, bool (*cond)(char)) {
    int i = 1;
    while (cond(text[i])) {
        ++i;
    }
    return i;
}

// we know the first char is a num
Value_And_Length Util::extract_num(const char* const text)
{
    int length = Util::len_while_condition(text, &Util::is_num);
    int val = 0;
    for (int i = 0; i < length; ++i) {
        val += (text[i] - '0') * Util::ipow(10, length - i - 1);
    }
    return { val, length };
}

bool Util::legal_name_char(char c) { return (c >= 'A' && c <= 'z'); }

bool Util::str_equal(const char* const str1, const char* str2, int len) {
    for (int i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) { return false; }
    }
    return true;
}