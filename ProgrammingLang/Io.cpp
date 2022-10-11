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
Where Io::print_string(Where w)
{
    int w_end = Util::where_is_char(w, '\"');
    DEBUG_MSG("Say String: ");
    for (++w; w < w_end; ++w) {
        std::cout << code[w];
    }
    DEBUG_MSG('\n');
    return w_end + 1;
}

void Io::print_x_chars_of(const char* const text, int length)
{
    for (int i = 0; i < length; ++i) {
        std::cout << text[i];
    }
}