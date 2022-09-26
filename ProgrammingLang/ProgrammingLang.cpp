#include <iostream>
#include <fstream>

int ipow(int a, int b) { // a raised to b
    int result = 1;
    for (int i = 0; i < b; ++i) {
        result *= a;
    }
    return result;
}

int len_to_char(const char* text, char looking_for) {
    int i = 1;
    while (text[i] != '\0') {
        if (text[ i ] == looking_for) {
            return i;
        }
        ++i;
    }
}

bool is_num(char possible_num) { return possible_num >= '0' && possible_num <= '9'; }

int len_while_condition(const char* text, bool (*cond)(char)) { // first assumed to be correct
    int i = 1;
    while (cond(text[i])) {
        ++i;
    }
    return i;
}

struct Extract_num_struct {
    int value;
    int length;
};

Extract_num_struct extract_num(const char* text) {
    // we know the first char is a num
    int length = len_while_condition(text, &is_num);
    int val = 0;
    for (int i = 0; i < length; ++i) {
        val += (text[i] - '0') * ipow(10, length - i - 1);
    }
    return { val, length };
}

void eval_math(const char* buffer) {
    if (buffer[0] == '$') {
        // find length of expression
        int length = len_to_char(buffer, '$');
        std::cout << "LENGTH: " << length  << '\n';
        
        int i = 1;
        int sum = 0;
        while (i < length) {
            if (is_num(buffer[i])) {
                auto extracted = extract_num(buffer + i);
                std::cout << "IS NUM!!! " << " num: " << extracted.value << "\n";
                i += extracted.length;
                sum += extracted.value;
            }
            else {
                ++i;
            }
        }

        
        
    }
}

bool str_equal(const char* str1, const char* str2, int len) {
    for (int i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) { return false; }
    }
    return true;
}

int main()
{
    std::cout << "Hello World!\n";
    
    std::ifstream f;
    f.open("code.kal");

    constexpr long long SIZE = 128;
    char buffer[SIZE];
    
    if (f.is_open())
    {
        while(f.good()) {
            f.getline(buffer, SIZE);
            std::cout << "Text we have: " << buffer << "\n";

            if (str_equal(buffer, "say ", 4)) {
                std::cout << buffer + 4 << " AFOGIDOA";
                eval_math(buffer + 4);
            }
        }
        if (f.fail()) { std::cerr << "FAIL!!!!!"; }
        f.close(); // Close input ffile
    }
    else { //Error message
        std::cerr << "ERROR: Can't find input file " << std::endl;
    }

}