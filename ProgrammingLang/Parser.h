#pragma once

#include <fstream>
#include "misc.h"
#include "Funcs.h"
#include "Scope.h"
#include "Io.h"
#include "Util.h"

class Parser
{
    std::ifstream _f;
    Funcs _funcs;

	// returns value and length of expression, ASSUMES buffer starts with '(' OR "CHARACTER NOT IMPORTANT TO EXPRESSION". Priority determines whether to stop att UN-priotirized like + or -
	template <bool priority = false>
    Value_And_Length eval_math(const char* const buffer, Scope& scope, int result = 0);

    void say(const char* const text, Scope& scope);

    // [0] is part of name and not checked
    void set_var(const char* const text, Scope& scope);

    Result_And_If_Nextline_And_Exits eval_block(char* const buffer, int indent, Scope& scope, const bool checkfile);

public:
    Parser() = default;
    void start(const char* const file_path);
};



// returns value and length of expression, ASSUMES buffer starts with '(' OR "CHARACTER NOT IMPORTANT TO EXPRESSION". Priority determines whether to stop att UN-priotirized like + or -
template <bool priority>
Value_And_Length Parser::eval_math(const char* const buffer, Scope& scope, int result)
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
            auto [value, len] = eval_math<>(buffer + i, scope);
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
        case '%':
            oper = OPER::MOD;
            ++i;
            break;
        case '=':
            oper = OPER::EQUAL;
            ++i;
            break;
        default:
            int direct_value;
            if (Util::is_num(buffer[i])) { // is number
                auto extracted = Util::extract_num(buffer + i);
                DEBUG_MSG("IS NUM!!! " << " num: " << extracted.value << "\n");
                i += extracted.length;

                direct_value = extracted.value;
            }
            else if (Util::legal_name_char(buffer[i])) {
                // it is a variable OR FUNCTION
                auto extracted = Name_Util::extract_name(buffer + i);
                int after_name = i + extracted.length;
                if (buffer[after_name] == '(') { // FUNCTION

                    auto start = _f.tellg();
                    const Func& func = _funcs.get_func(extracted.name);
                    Scope new_scope;
                    
                    int args_length = 1;

                    if (buffer[after_name + 1] != ')') { // there are args
                        args_length = 0;
                        int arg_index = 0;
                        while (true) {
                            if (buffer[after_name + args_length] == ')') {
                                break;
                            }
                            else if (buffer[after_name + args_length] == '(' || buffer[after_name + args_length] == ',') {

                                auto arg_extract = eval_math<>(buffer + after_name + args_length, scope);

                                args_length += arg_extract.length;
                                // add var
                                new_scope.add_var(func.param_names[arg_index], arg_extract.value);
                                ++arg_index;
                            }
                            else {
                                ++arg_index;
                            }
                        }
                    }

                    char new_buffer[SIZE];
                    _f.seekg(func.pos);
                    direct_value = eval_block(new_buffer, space_per_indent, new_scope, true).result;

                    i += extracted.length + args_length + 1;

                    _f.seekg(start);
                }
                else { // Variable
                    direct_value = scope.get_value(extracted.name);
                    i += extracted.length;
                }
            }
            else { // NO LEGAL CHARACTER AT ALL, QUIT
                DEBUG_MSG("ILLEGAL CHAR: " << buffer[i] << '\n');
                goto END_OF_EVAL_MATH;
            }

            // look forward after direct value, possibly apply priority
            auto new_extract = eval_math<true>(buffer + i - 1, scope, direct_value);
            i += new_extract.length - 1; // minus 1
            result = use_oper(oper, result, new_extract.value);

            break;
        }
    }
END_OF_EVAL_MATH:
    DEBUG_MSG("TOTAL: " << result << "\n");
    return { result, i };
}


