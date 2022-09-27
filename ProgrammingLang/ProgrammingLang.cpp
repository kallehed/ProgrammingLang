#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <cstdint>

#ifdef _DEBUG
#define DEBUG_MSG(str) do { std::cout << str; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

int ipow(int a, int b) { // a raised to b
    int result = 1;
    for (int i = 0; i < b; ++i) {
        result *= a;
    }
    return result;
}

// starts at [1]
int len_to_char(const char* text, char looking_for) {
    int i = 1;
    while (text[i] != '\0') {
        if (text[ i ] == looking_for) {
            return i;
        }
        ++i;
    }
    throw std::invalid_argument("Could not find char");
}

bool is_num(char possible_num) { return possible_num >= '0' && possible_num <= '9'; }

// first assumed to be correct
int len_while_condition(const char* text, bool (*cond)(char)) { 
    int i = 1;
    while (cond(text[i])) {
        ++i;
    }
    return i;
}

struct Value_and_length {
    int value;
    int length;
};

// we know the first char is a num
Value_and_length extract_num(const char* text)
{    
    int length = len_while_condition(text, &is_num);
    int val = 0;
    for (int i = 0; i < length; ++i) {
        val += (text[i] - '0') * ipow(10, length - i - 1);
    }
    return { val, length };
}

struct Var_name {
    union {
        std::array<char, 8> chars;
        int64_t number;
    };

    bool operator==(const Var_name& other) const
    {
        return this->number == other.number;
    }
    struct HashFunction
    {
        size_t operator()(const Var_name& obj) const
        {
            return std::hash<int64_t>()(obj.number);
        }
    };
};
class Var_Handler {
    std::unordered_map<Var_name, int, Var_name::HashFunction> _vars;
public:
    void add_var(Var_name name, int value)
    {
        _vars.insert({ name, value });
    }
    void print_var(Var_name name) {
        int value = _vars[name];
        DEBUG_MSG("Variable \'");
        for (int i = 0; i < 8; ++i) {
            DEBUG_MSG(name.chars[i]);
        }
        DEBUG_MSG( "\' with value: " << value << '\n');
    }
    int get_value(Var_name name) {
        return _vars[name];
    }
};

bool legal_var_name_char(char c) { return (c >= 'A' && c <= 'z') || (c >= '0' && c <= '9'); }

// assume first character is '
Value_and_length extract_var(const char* text, Var_Handler var_handler)
{
    
    //int length = len_to_char(text, ' ');
    int length = len_while_condition(text, legal_var_name_char);
    if (length > 8) { throw std::invalid_argument("Very too long variable"); }
    Var_name name;
    name.number = 0;
    for (int i = 0; i < length; ++i) {
        name.chars[i] = text[i];
    }
    int value = var_handler.get_value(name);
    var_handler.print_var(name);
    return { value, length };
}

namespace OPER {
    enum _ : char {
        PLUS,
        MULT,
    };
}

template <char begin, char end>
int til_right_char(const char* text) {
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

int use_oper (OPER::_ oper, int start_value, int new_value) {
    switch (oper) {
    case OPER::PLUS:
        return start_value + new_value;
    case OPER::MULT:
        return start_value * new_value;
    default:
        throw std::invalid_argument("INVALID OPER, NOT CORRECT");
    }
}

// returns value and length of expression, ASSUMES buffer starts with '('
Value_and_length eval_math(const char* buffer, Var_Handler& var_handler) {
    // find length of expression
    int length = til_right_char<'(',')'>(buffer);
    DEBUG_MSG("LENGTH: " << length << '\n');
        
    OPER::_ oper = OPER::PLUS;
    int i = 1;
    int result = 0;
    while (i < length) {
        switch (buffer[i]) {
        case ' ':
            break;
        case '(':
        {
            auto [value, len] = eval_math(buffer + i, var_handler);
            i += len;
            result = use_oper(oper, result, value);
            break;
        }
        case ')':
            throw std::invalid_argument("FOUND ILLEGAL ')' ERROR!");
            break;
        case '*':
            oper = OPER::MULT;
            break;
        case '+':
            oper = OPER::PLUS;
            break;
        default:
            if (is_num(buffer[i])) { // is number
                auto extracted = extract_num(buffer + i);
                DEBUG_MSG("IS NUM!!! " << " num: " << extracted.value << "\n");
                i += extracted.length;
                result = use_oper(oper, result, extracted.value);
            }
            else {
                // it is a variable
                auto extracted = extract_var(buffer + i, var_handler);
                result = use_oper(oper, result, extracted.value);
                i += extracted.length;
            }
            break;
        }
        ++i;
        
    }
    DEBUG_MSG("TOTAL: " << result << "\n");
    return { result, length };
}

// assume it starts with ", returns length of string
int print_string(const char* text)
{
    int len = len_to_char(text, '\"');
    DEBUG_MSG("Say String: ");
    for (int i = 1; i < len; ++i) {
        std::cout << text[i];
    }
    DEBUG_MSG('\n');
    return len;
}

bool str_equal(const char* str1, const char* str2, int len) {
    for (int i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) { return false; }
    }
    return true;
}

// assumes to start with '
void create_var(const char* text, Var_Handler& var_handler)
{
    int len_of_name = len_to_char(text, ' ');
    if (len_of_name > 9) { throw std::invalid_argument("Too long variable name error!"); }
    Var_name name;
    name.number = 0;
    for (int i = 1; i < len_of_name; ++i) {
        name.chars[i - 1] = text[i];
    }
    // find what it is equal too
    if (str_equal(text + len_of_name, " equal to (", 11)) {
        auto [value, _] = eval_math(text + len_of_name + 10, var_handler);
        var_handler.add_var(name, value);
        var_handler.print_var(name);
    }
    else {
        throw std::invalid_argument("Wrong text after creating variable");
    }
}

void say(const char* text, Var_Handler var_handler)
{
    int i = 0;
    while (true) {
        switch (text[i]) {
        case '\0':
            return;
        case '\"':
            i += print_string(text + i);
            break;
        case '(':
        {
            DEBUG_MSG("You said following expression: ");
            auto [value, length] = eval_math(text + i, var_handler);
            std::cout << value;
            DEBUG_MSG('\n');
            i += length;
            break;
        }
        }
        ++i;
    }
}

int main()
{
    DEBUG_MSG("Hello World!\n");

    //if (til_right_char <'(', ')'>("(12()34((45 + 2))56)") != 19) { throw std::invalid_argument("til_right_char does not work"); }

    Var_Handler var_handler;
    
    std::ifstream f;
    f.open("code.kal");

    constexpr long long SIZE = 128;
    char buffer[SIZE];
    
    if (f.is_open())
    {
        while(f.good()) {
            f.getline(buffer, SIZE);
            DEBUG_MSG("Text we have: " << buffer << "\n");
            if (str_equal(buffer, "//", 2)) {
                // ignore line
            }
            else if (str_equal(buffer, "say ", 4)) {
                DEBUG_MSG("say stuff\n");
                say(buffer + 4, var_handler);
            }
            else if (str_equal(buffer, "say (", 5)) {
                
            }
            else if (str_equal(buffer, "say \"", 5)) {
                
            }
            else if (str_equal(buffer, "create var ", 11)) {
                create_var(buffer + 10, var_handler);
            }
        }
        if (f.fail()) { std::cerr << "FAIL!!!!!"; }
        f.close(); // Close input ffile
    }
    else { //Error message
        std::cerr << "ERROR: Can't find input file " << std::endl;
    }

}