#include "Parser.h"

void Parser::say(Where w, Scope& scope)
{
    while (true) {
        switch (code[w]) {
        case '\n':
        case '\0':
            std::cout << '\n';
            return;
        case '\"':
            w = Io::print_string(w);
            break;
        case '(':
        {
            DEBUG_MSG("You said following expression: ");
            auto extract= eval_math(w, scope);
            std::cout << extract._value;
            DEBUG_MSG('\n');
            w = extract._w;
            break;
        }
        }
        ++w;
    }
}

void Parser::set_var(Where w, Scope& scope)
{
    auto extract_name = Name_Util::extract_name(w);

    if (Util::str_equal(code + extract_name._w, " =", 2)) {
        auto extract_math = eval_math(extract_name._w + 1, scope);
        scope.set_value(extract_name._name, extract_math._value);

        Io::print_var_details(extract_name._name, scope);
    }
    else {
        throw std::invalid_argument("Wrong semantics very bad, Setting variable incorrect");
    }
}

Result_And_Exit Parser::eval_block(Where w, const int indent, Scope& scope)
{
    while (true)
    {
        // where /n is
        if (code[w] == '\0') { return { 0, false }; }
        const Where end_of_line = Util::where_is_char(w - 1, '\n');
        const Where next_line_begins = end_of_line + 1;
        const int line_length = end_of_line - w;

        w += indent;
            //if (len_of_line - 1 <= indent) {
            //    continue;
            //}
            //if (code[0] == '\t') {
            //    std::cout << "ERROR TAB!!!!!";
            //    std::cin.get();
            //}
        //DEBUG_MSG("Text we have: " << code[w] << "\n");

        /* { // check line for other indentation IN PREVIOUS indents
            int i = 0;
            while (i + 1 < len_of_line && i < indent) {
                if (buffer[i] != ' ') {
                    int exits = (indent - i) / space_per_indent - 1;
                    return { 0, false, exits };
                }
                i += space_per_indent;
            }
        }*/

        if (Util::str_equal(code + w, "#", 1)) {
            // ignore line
        }
        else if (Util::str_equal(code + w, "say ", 4))
        {
            DEBUG_MSG("say stuff\n");
            say(w + 4, scope);
        }
        else if (Util::str_equal(code + w, "if ", 3))
        {
            int expr = eval_math<>(w + 2, scope)._value;
            if (expr) {
                auto extract = eval_block(end_of_line + 1, indent + space_per_indent, scope);
                if (extract._exit) {
                    return { extract._result, extract._exit };
                }
            }
        }
        else if (Util::str_equal(code + w, "while ", 6))
        {
            w += 5; // w set to: whileHERE
            while (true) { //
                if (eval_math<>(w, scope)._value) {

                    auto extract = eval_block(end_of_line + 1, indent + space_per_indent, scope);
                    if (extract._exit) {
                        return { extract._result, true };
                    }
                }
                else {
                    break;
                }
            }
        }
        else if (Util::str_equal(code + w, "def ", 4))
        {
            DEBUG_MSG("Creating function!\n");

            //const char* place = text + 4;
            w += 4; // should be at: def HERE, at beginning of name
            auto def_extract = Name_Util::extract_name(w);
            w = def_extract._w;

            std::array<Name, MAX_FUNC_PARAMS> param_names;
            Func::reset_names(param_names);

            int name_index = 0;

            while (true) { // register all names
                if (code[w] == '\n') {
                    break;
                }
                else if (Util::legal_name_char(code[w]))
                {
                    auto extract = Name_Util::extract_name(w);
                    param_names[name_index] = extract._name;
                    ++name_index;
                    w = extract._w;
                }
                else {
                    ++w;
                }
            }
            _funcs.add_func(def_extract._name, param_names, next_line_begins);
        }
        else if (Util::str_equal(code + w, "return ", 7)) // should exit all until finding math expression
        { 
            w += 6; // w set to returnHERE
            return { eval_math(w, scope)._value, true}; 
        }
        else if (Util::legal_name_char(code[w])) { // variable set to something
            set_var(w, scope);
        }

        w = next_line_begins; // set to next line
    }
    return { 0 , false };
}

void Parser::start(const char * const file_path)
{
    DEBUG_MSG("Hello World!\n");

    //if (til_right_char <'(', ')'>("(12()34((45 + 2))56)") != 19) { throw std::invalid_argument("til_right_char does not work"); }

    std::ifstream f;

    f.open(file_path);

    if (f.is_open())
    {
        f.getline(code, MAX_CODE_LEN, '\0');
        if (f.fail()) { std::cerr << "FILE TOO LONG ERROR!!!"; }

        Scope scope;

        eval_block(0, 0, scope);

        f.close(); // Close input ffile
    }
    else { //Error message
        std::cerr << "ERROR: Can't find input file " << std::endl;
    }
}
