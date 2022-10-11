#pragma once

#include "misc.h"

class Util
{
public:
    // a raised to b
    static int ipow(int a, int b);

    // starts at [1], returns where first occurence of char is
    static int where_is_char(Where w, char looking_for);

    static bool is_num(char possible_num);

    // first assumed to be correct, returns where condition is NOT true first.
    static Where where_while_condition(Where w, bool (*cond)(char));

    // we know the first char is a num, returns where first NON num char is
    static Value_And_Where extract_num(Where w);

    // if it is from the alphabet basically
    static bool legal_name_char(char c);

    static bool str_equal(const char* const str1, const char* str2, int len);

    // begins at [1], counts length between "( aoisjd ( aijsd ( ) dfo ) )", returns where the one that ends the first one is
    template <char begin, char end>
    static int til_right_char(Where w);
};

template <char begin, char end>
int Util::til_right_char(Where w) {
    int count = 1;
    while (true) {
        ++w;
        switch (code[w]) {
        case begin:
            ++count;
            break;
        case end:
            --count;
            if (count == 0) {
                return w;
            }
            break;
        }
        
    }
}


