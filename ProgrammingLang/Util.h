#pragma once

#include "misc.h"

class Util
{
public:
    static int ipow(int a, int b);

    static int len_to_char(const char* const text, char looking_for);

    static bool is_num(char possible_num);

    static int len_while_condition(const char* const text, bool (*cond)(char));

    static Value_And_Length extract_num(const char* const text);

    static bool legal_var_name_char(char c);

    static bool str_equal(const char* const str1, const char* str2, int len);

    // begins at [1], counts length between "( aoisjd ( aijsd ( ) dfo ) )" the one that ends the first one
    template <char begin, char end>
    static int til_right_char(const char* const text);
};

template <char begin, char end>
int Util::til_right_char(const char* const text) {
    int count = 1;
    int i = 1;
    while (true) {
        switch (text[i]) {
        case begin:
            ++count;
            break;
        case end:
            --count;
            if (count == 0) {
                return i;
            }
            break;
        }
        ++i;
    }
}


