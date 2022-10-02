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

constexpr long long SIZE = 128;
constexpr int space_per_indent = 4;

int ipow(int a, int b) { // a raised to b
    int result = 1;
    for (int i = 0; i < b; ++i) {
        result *= a;
    }
    return result;
}

// starts at [1]
int len_to_char(const char* const text, char looking_for) {
    int i = 1;
    while (true) {
        if (text[i] == looking_for) {
            return i;
        }
        ++i;
    }
}

bool is_num(char possible_num) { return possible_num >= '0' && possible_num <= '9'; }

// first assumed to be correct
int len_while_condition(const char* const text, bool (*cond)(char)) { 
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
Value_and_length extract_num(const char* const text)
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

struct Func {
    Var_name param_name;
    std::streampos pos;
};
class Var_Handler {
    std::unordered_map<Var_name, int, Var_name::HashFunction> _vars;
    std::unordered_map<Var_name, Func, Var_name::HashFunction> _funcs;

    Var_Handler(const Var_Handler&) = delete;
public:
    Var_Handler() {};
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
        if (_vars.find(name) == _vars.end()) {
            std::cout << "ERROR: UNKNOWN VARIABLE: \"" << &(name.chars[0]) << "\"";
            std::cin.get();
            throw std::invalid_argument("UNKNOWN VARIABLE");
        }
        else {
            return _vars[name];
        }
    }
    void set_value(Var_name name, int value) {
        _vars[name] = value;
    }

    void add_func(Var_name name, Var_name param_name, std::streampos pos) {
        _funcs.insert({ name, {param_name, pos} });
    }
    Func get_func(Var_name name) {
        if (_funcs.find(name) == _funcs.end()) {
            std::cout << "ERROR:  UNKNOWN FUNCTION: \"" << &(name.chars[0]) << "\"";
            std::cin.get();
            throw std::invalid_argument("UNKNOWN FUNCTION");
        }
        else {
            return _funcs[name];
        }
    }
};

bool legal_var_name_char(char c) { return (c >= 'A' && c <= 'z') || (c >= '0' && c <= '9'); }

struct Var_name_and_length {
    Var_name name;
    int length;
};

// assume first character is FIRST OF VAR
Var_name_and_length extract_var_name(const char* const text, Var_Handler& var_handler)
{
    int length = len_while_condition(text, legal_var_name_char);
    if (length > 8) { throw std::invalid_argument("Very too long variable"); }
    Var_name name;
    name.number = 0;
    for (int i = 0; i < length; ++i) {
        name.chars[i] = text[i];
    }
    return { name, length };
}

Value_and_length extract_var_value(const char * const text, Var_Handler& var_handler)
{
    auto [name, length] = extract_var_name(text, var_handler);
    int value = var_handler.get_value(name);
    var_handler.print_var(name);
    return { value, length };
}

namespace OPER {
    enum _ : char {
        PLUS,
        MULT,
        MORE_THAN,
        LESS_THAN,
        MINUS,
    };
}

template <char begin, char end>
int til_right_char(const char* const text) {
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
    case OPER::MORE_THAN:
        return start_value > new_value;
    case OPER::LESS_THAN:
        return start_value < new_value;
    case OPER::MINUS:
        return start_value - new_value;
    default:
        throw std::invalid_argument("INVALID OPER, NOT CORRECT");
    }
}

struct result_and_if_nextline_and_exits {
    int result;
    bool should_nextline;
    int exits;
};

result_and_if_nextline_and_exits eval_block(std::ifstream& f, char* const buffer, int indent, Var_Handler& var_handler, const bool checkfile = false);

// returns value and length of expression, ASSUMES buffer starts with '(' OR "CHARACTER NOT IMPORTANT TO EXPRESSION". Priority determines whether to stop att UN-priotirized like + or -
template <bool priority = false>
Value_and_length eval_math(const char * const buffer, Var_Handler& var_handler, std::ifstream& f, int result = 0)
{        
    OPER::_ oper = OPER::PLUS;
    int i = 1;
    while (true) {
        switch (buffer[i]) {
        case ' ':
            ++i;
            break;
        case '(':
        {
            auto [value, len] = eval_math<>(buffer + i, var_handler, f);
            i += len + 1;
            result = use_oper(oper, result, value);
            break;
        }
        case ')':
            goto END_OF_EVAL_MATH; // TODO: FIX THIS ORDERING
            ++i;
        case '*':
            oper = OPER::MULT;
            ++i;
            break;
        case '+':
            if constexpr (priority) { return { result, i }; }
            ++i;
            oper = OPER::PLUS;
            
            break;
        case '>':
            oper = OPER::MORE_THAN;
            ++i;
            break;
        case '<':
            oper = OPER::LESS_THAN;
            ++i;
            break;
        case '-':
            if constexpr (priority) { return { result, i }; }
            ++i;
            oper = OPER::MINUS;
            
            break;
        default:
            int direct_value;
            if (is_num(buffer[i])) { // is number
                auto extracted = extract_num(buffer + i);
                DEBUG_MSG("IS NUM!!! " << " num: " << extracted.value << "\n");
                i += extracted.length;

                direct_value = extracted.value;
            }
            else if (legal_var_name_char(buffer[i])) {
                // it is a variable OR FUNCTION
                auto extracted = extract_var_name(buffer + i, var_handler);
                int after_name = i + extracted.length;
                if (buffer[after_name] == '(') { // FUNCTION
                    auto start = f.tellg();
                    Func func = var_handler.get_func(extracted.name);
                    auto arg_extract = eval_math<>(buffer + after_name, var_handler, f);
                    f.seekg(func.pos);
                    char new_buffer[SIZE];
                    Var_Handler new_var_handler;
                    new_var_handler.add_var(func.param_name, arg_extract.value);
                    direct_value = eval_block(f, new_buffer, space_per_indent, new_var_handler).result;

                    i += extracted.length + arg_extract.length;

                    f.seekg(start);
                }
                else { // Variable
                    direct_value = var_handler.get_value(extracted.name);
                    i += extracted.length;
                }
            }
            else { // NO LEGAL CHARACTER AT ALL, QUIT
                DEBUG_MSG("ILLEGAL CHAR: " << buffer[i] << '\n');
                goto END_OF_EVAL_MATH;
            }

            // look forward after direct value, possibly apply priority
            auto new_extract = eval_math<true>(buffer + i - 1, var_handler, f, direct_value);
            i += new_extract.length - 1; // minus 1
            result = use_oper(oper, result, new_extract.value);

            break;
        }
    }
END_OF_EVAL_MATH:
    DEBUG_MSG("TOTAL: " << result << "\n");
    return { result, i };
}

// assume it starts with ", returns length of string
int print_string(const char* const text)
{
    int len = len_to_char(text, '\"');
    DEBUG_MSG("Say String: ");
    for (int i = 1; i < len; ++i) {
        std::cout << text[i];
    }
    DEBUG_MSG('\n');
    return len;
}

bool str_equal(const char* const str1, const char* str2, int len) {
    for (int i = 0; i < len; ++i) {
        if (str1[i] != str2[i]) { return false; }
    }
    return true;
}

// assumes to start with '
void create_var(const char* const text, Var_Handler& var_handler, std::ifstream& f)
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
        auto [value, _] = eval_math(text + len_of_name + 10, var_handler, f);
        var_handler.add_var(name, value);
        var_handler.print_var(name);
    }
    else {
        throw std::invalid_argument("Wrong text after creating variable");
    }
}

void say(const char* const text, Var_Handler& var_handler, std::ifstream& f)
{
    int i = 0;
    while (true) {
        switch (text[i]) {
        case '\0':
            std::cout << '\n';
            return;
        case '\"':
            i += print_string(text + i);
            break;
        case '(':
        {
            DEBUG_MSG("You said following expression: ");
            auto [value, length] = eval_math(text + i, var_handler, f);
            std::cout << value;
            DEBUG_MSG('\n');
            i += length;
            break;
        }
        }
        ++i;
    }

}
void print_x_chars_of(const char* const text, int length)
{
    for (int i = 0; i < length; ++i) {
        std::cout << text[i];
    }
}

void set_var(const char* const text, Var_Handler& var_handler, std::ifstream& f)
{
    auto [name, length] = extract_var_name(text, var_handler);

    if (str_equal(text + length, " =", 2)) {
        auto [value, len] = eval_math(text + length + 1, var_handler, f);
        var_handler.set_value(name, value);

        var_handler.print_var(name);
    }
    else {
        throw std::invalid_argument("Wrong semantics very bad");
    }
}

result_and_if_nextline_and_exits eval_block(std::ifstream& f, char* const buffer, int indent, Var_Handler& var_handler, const bool checkfile)
{
    bool should_nextline = true;
    const char* const text = (const char* const)(buffer + indent);
    while (!checkfile || f.good()) {
        int len_of_line = 0;
        int i = 0;
        if (should_nextline) {
            f.getline(buffer, SIZE);
            
            len_of_line = len_to_char(buffer - 1, '\0');
            if (len_of_line - 1 <= indent) {
                continue;
            }
            if (text[0] == '\t') {
                std::cout << "ERROR TAB!!!!!";
                std::cin.get();
            }
            f.clear();
        }
        should_nextline = true;
        DEBUG_MSG("Text we have: " << text << "\n");

        { // check line for other indentation IN PREVIOUS indents
            int i = 0;
            while (i + 1 < len_of_line && i < indent) {
                if (buffer[i] != ' ') {
                    int exits = (indent - i) / space_per_indent - 1;
                    return { 0, false, exits};
                }
                i += space_per_indent;
            }
        }

        if (str_equal(text, "#", 1)) {
            // ignore line
        }
        else if (str_equal(text, "say ", 4)) {
            DEBUG_MSG("say stuff\n");
            say(text + 4, var_handler, f);
        }
        else if (str_equal(text, "create var ", 11)) {
            create_var(text + 10, var_handler, f);
        }
        else if (str_equal(text, "if ", 3)) {
            int expr = eval_math(text + 2, var_handler, f).value;
            if (expr) {
                auto extract = eval_block(f, buffer, indent + space_per_indent, var_handler, true);
                should_nextline = extract.should_nextline;
                if (extract.exits > 0) {
                    return { 0, false, extract.exits - 1 };
                }
            }
        }
        else if (str_equal(text, "while ", 6)) {
            std::streampos start = f.tellg();
            char while_buffer[SIZE];
            for (int i = 0; i < SIZE; ++i) {
                while_buffer[i] = text[i];
            }
            while (true) {
                if (eval_math(while_buffer + 5, var_handler, f).value) {

                    eval_block(f, buffer, indent + space_per_indent, var_handler, true);
                    f.clear();
                    f.seekg(start);
                    
                }
                else {
                    break;
                }
            }
        }
        else if (str_equal(text, ".", 1)) {
            DEBUG_MSG("breakout!");
            return { 0, true };
        }
        else if (str_equal(text, "def ", 4)) {
            DEBUG_MSG("Creating function!\n");

            const char* place = text + 4;
            auto def_extract = extract_var_name(place, var_handler);
            place += def_extract.length;
            Var_name param_name;
            while (true) {
                if (legal_var_name_char(place[0])) {
                    auto param_extract = extract_var_name(place, var_handler);
                    param_name = param_extract.name;
                    break;
                }
                ++place;
            }
            var_handler.add_func(def_extract.name, param_name, f.tellg());
        }
        else if (str_equal(text, "return ", 7)) {
            return { eval_math(text + 6, var_handler, f).value, true };
        }
        else if (legal_var_name_char(text[0])) { // variable set to something
            set_var(text, var_handler, f);
        }
    }
    return { 0 , false};
}

int main()
{
    DEBUG_MSG("Hello World!\n");

    //if (til_right_char <'(', ')'>("(12()34((45 + 2))56)") != 19) { throw std::invalid_argument("til_right_char does not work"); }

    std::ifstream f;
    f.open("code3.kal");
    
    char buffer[SIZE];

    Var_Handler var_handler;
    
    if (f.is_open())
    {
        eval_block(f, buffer, 0, var_handler, true);
        
        if (f.fail()) { std::cerr << "FAIL!!!!!"; }
        f.close(); // Close input ffile
    }
    else { //Error message
        std::cerr << "ERROR: Can't find input file " << std::endl;
    }

}