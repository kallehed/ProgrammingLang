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
        default:
            ++w;
            break;
        }
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

Result_And_Exit Parser::eval_block(int line, Scope& scope) // MOST IMPORTANT FUNCTION
{
    while (line < _total_lines)
    {
        const Where w = _lines[line]._w;
        if (code[w] == '\0') { return { 0, false }; }
        // where /n is
        {
            int a = 4;
        }
        // print line
#if _DEBUG
        { std::cout << "Line: "; Io::print_x_chars_of(code + w, _lines[line + 1]._w - w - 1); std::cout << "\n"; }
#endif

        if (Util::str_equal(code + w, "#", 1)) {
            // ignore line
        }
#if _DEBUG
        else if (code[w] == '¨') { // breakpoint
            std::cout << "BREAKPOINT HIT!\n";
        }
#endif
        else if (Util::str_equal(code + w, "say ", 4))
        {
            DEBUG_MSG("say stuff\n");
            say(w + 4, scope);
        }
        else if (Util::str_equal(code + w, "if ", 3))
        {
            int expr = eval_math<>(w + 2, scope)._value;
            if (expr) {
                auto extract = eval_block(line + 1, scope);
                if (extract._exit) {
                    return { extract._result, extract._exit };
                }
            }
        }
        else if (Util::str_equal(code + w, "while ", 6))
        {
            while (true) { 
                if (eval_math<>(w + 5, scope)._value) { // w set to: whileHERE

                    auto extract = eval_block(line + 1, scope);
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

            auto def_extract = Name_Util::extract_name(w + 4); // should be at: def HERE, at beginning of name
            Where new_w = def_extract._w;

            std::array<Name, MAX_FUNC_PARAMS> param_names;
            Func::reset_names(param_names);

            int name_index = 0;

            while (true) { // register all names
                if (code[new_w] == '\n') {
                    break;
                }
                else if (Util::legal_name_char(code[new_w]))
                {
                    auto extract = Name_Util::extract_name(new_w);
                    param_names[name_index] = extract._name;
                    ++name_index;
                    new_w = extract._w;
                }
                else {
                    ++new_w;
                }
            }
            _funcs.add_func(def_extract._name, param_names, line + 1);
        }
        else if (Util::str_equal(code + w, "return ", 7)) // should exit all until finding math expression
        { 
            return { eval_math(w + 6, scope)._value, true};  // w set to returnHERE
        }
        else if (Util::legal_name_char(code[w])) { // variable set to something
            set_var(w, scope);
        }

        GOTO_NEXT_LINE:

        ++line;
    }
    return { 0 , false };
}

void Parser::start(const char * const file_path)
{
    DEBUG_MSG("Hello Debug!\n");

    //if (til_right_char <'(', ')'>("(12()34((45 + 2))56)") != 19) { throw std::invalid_argument("til_right_char does not work"); }

    std::ifstream f;

    f.open(file_path);

    if (f.is_open())
    {
        f.getline(code, MAX_CODE_LEN, '\0');
        if (f.fail()) { std::cerr << "FILE TOO LONG(Probably) ERROR!!!"; goto GOTO_CLOSE_FILE; }

        // check for tabs, calulate length + indent + where of ALL lines
        {
            _lines.fill({ -1, -1});
            _lines[0]._w = 0;
            { // things with line
                int line = 1;
                for (int i = 0; i < MAX_CODE_LEN; ++i) {
                    switch (code[i]) {
                    case '\0':
                        _lines[line]._w = i + 1;
                        _lines[line]._indent = -1;
                        goto GOTO_AFTER_FOR;
                    case '\n':
                        _lines[line]._w = i + 1;
                        _lines[line]._indent = -1;
                        ++line;
                        break;
                    case '\t':
                        std::cerr << "TAB TAB TAB TAB TAB!!!! AHH TAAAAAAAAAAAAAAB!! ERROR!";
                        goto GOTO_CLOSE_FILE;
                    default:
                        if ((code[i] != ' ') && (_lines[line - 1]._indent == -1)) {
                            // code starts here
                            int spaces = (i - _lines[line - 1]._w);
                            if ((spaces % SPACE_PER_INDENT) != 0) {
                                std::cerr << "WRONG INDENTATION ON LINE: " << (line) << '\n';
                                goto GOTO_CLOSE_FILE;
                            }
                            else {
                                _lines[line - 1]._indent = spaces / SPACE_PER_INDENT;
                                if (spaces > 0) {
                                    memmove((code + i) - spaces, (code + i), MAX_CODE_LEN - i);
                                }
                            }
                        }
                        break;
                    }
                }
                GOTO_AFTER_FOR:
                _total_lines = line;
            }
        }

        {
            Scope scope;

            eval_block(0, scope);
        }

        GOTO_CLOSE_FILE:
        f.close(); // Close input ffile
    }
    else { //Error message
        std::cerr << "ERROR: Can't find input file " << std::endl;
    }
}
