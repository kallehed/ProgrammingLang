#include "Io.h"
#include "Scope.h"
#include "Util.h"

void Io::print_var_details(Name name, Scope& scope)
{
    int value = scope.get_value(name);
    DEBUG_MSG("Variable \'");
    for (int i = 0; i < 8; ++i) {
        DEBUG_MSG(name.chars[i]);
    }
    DEBUG_MSG("\' with value: " << value << '\n');
}

// assume it starts with ", returns length of string
int Io::print_string(const char* const text)
{
    int len = Util::len_to_char(text, '\"');
    DEBUG_MSG("Say String: ");
    for (int i = 1; i < len; ++i) {
        std::cout << text[i];
    }
    DEBUG_MSG('\n');
    return len;
}

void Io::print_x_chars_of(const char* const text, int length)
{
    for (int i = 0; i < length; ++i) {
        std::cout << text[i];
    }
}