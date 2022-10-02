#include "Parser.h"

void Parser::say(const char* const text, Scope& scope)
{
    int i = 0;
    while (true) {
        switch (text[i]) {
        case '\0':
            std::cout << '\n';
            return;
        case '\"':
            i += Io::print_string(text + i);
            break;
        case '(':
        {
            DEBUG_MSG("You said following expression: ");
            auto [value, length] = eval_math(text + i, scope);
            std::cout << value;
            DEBUG_MSG('\n');
            i += length;
            break;
        }
        }
        ++i;
    }
}

void Parser::set_var(const char* const text, Scope& scope)
{
    auto [name, length] = Name_Util::extract_name(text);

    if (Util::str_equal(text + length, " =", 2)) {
        auto [value, len] = eval_math(text + length + 1, scope);
        scope.set_value(name, value);

        Io::print_var_details(name, scope);
    }
    else {
        throw std::invalid_argument("Wrong semantics very bad");
    }
}

Result_And_If_Nextline_And_Exits Parser::eval_block(char* const buffer, int indent, Scope& scope, const bool checkfile)
{
    bool should_nextline = true;
    const char* const text = (const char* const)(buffer + indent);
    while (!checkfile || _f.good()) {
        int len_of_line = 0;
        int i = 0;
        if (should_nextline) {
            _f.getline(buffer, SIZE);

            len_of_line = Util::len_to_char(buffer - 1, '\0');
            if (len_of_line - 1 <= indent) {
                continue;
            }
            if (text[0] == '\t') {
                std::cout << "ERROR TAB!!!!!";
                std::cin.get();
            }
            _f.clear();
        }
        should_nextline = true;
        DEBUG_MSG("Text we have: " << text << "\n");

        { // check line for other indentation IN PREVIOUS indents
            int i = 0;
            while (i + 1 < len_of_line && i < indent) {
                if (buffer[i] != ' ') {
                    int exits = (indent - i) / space_per_indent - 1;
                    return { 0, false, exits };
                }
                i += space_per_indent;
            }
        }

        if (Util::str_equal(text, "#", 1)) {
            // ignore line
        }
        else if (Util::str_equal(text, "say ", 4)) {
            DEBUG_MSG("say stuff\n");
            say(text + 4, scope);
        }
        else if (Util::str_equal(text, "if ", 3)) {
            int expr = eval_math(text + 2, scope).value;
            if (expr) {
                auto extract = eval_block(buffer, indent + space_per_indent, scope, true);
                should_nextline = extract.should_nextline;
                if (extract.exits > 0) {
                    return { 0, false, extract.exits - 1 };
                }
            }
        }
        else if (Util::str_equal(text, "while ", 6)) {
            std::streampos start = _f.tellg();
            char while_buffer[SIZE];
            for (int i = 0; i < SIZE; ++i) {
                while_buffer[i] = text[i];
            }
            while (true) {
                if (eval_math(while_buffer + 5, scope).value) {

                    eval_block(buffer, indent + space_per_indent, scope, true);
                    _f.clear();
                    _f.seekg(start);

                }
                else {
                    break;
                }
            }
        }
        else if (Util::str_equal(text, ".", 1)) {
            DEBUG_MSG("breakout!");
            return { 0, true };
        }
        else if (Util::str_equal(text, "def ", 4)) {
            DEBUG_MSG("Creating function!\n");

            const char* place = text + 4;
            auto def_extract = Name_Util::extract_name(place);
            place += def_extract.length;
            Name param_name;
            while (true) {
                if (Util::legal_var_name_char(place[0])) {
                    auto param_extract = Name_Util::extract_name(place);
                    param_name = param_extract.name;
                    break;
                }
                ++place;
            }
            _funcs.add_func(def_extract.name, param_name, _f.tellg());
        }
        else if (Util::str_equal(text, "return ", 7)) {
            return { eval_math(text + 6, scope).value, true };
        }
        else if (Util::legal_var_name_char(text[0])) { // variable set to something
            set_var(text, scope);
        }
    }
    return { 0 , false };
}

void Parser::start(const char * const file_path)
{
    DEBUG_MSG("Hello World!\n");

    //if (til_right_char <'(', ')'>("(12()34((45 + 2))56)") != 19) { throw std::invalid_argument("til_right_char does not work"); }

    _f.open(file_path);

    char buffer[SIZE];

    Scope scope;

    if (_f.is_open())
    {
        eval_block(buffer, 0, scope, true);

        if (_f.fail()) { std::cerr << "FAIL!!!!!"; }
        _f.close(); // Close input ffile
    }
    else { //Error message
        std::cerr << "ERROR: Can't find input file " << std::endl;
    }
}
