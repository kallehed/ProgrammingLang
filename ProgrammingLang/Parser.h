#pragma once

#include <fstream>
#include "misc.h"
#include "Funcs.h"
#include "Scope.h"
#include "Io.h"
#include "Util.h"

class Parser
{
    Funcs _funcs;

	// returns value and AFTER where expression ends, ASSUMES buffer starts with '(' OR "CHARACTER NOT IMPORTANT TO EXPRESSION". Priority determines whether to stop att UN-priotirized like + or -.
	template <bool priority = false>
    Value_And_Where eval_math(Where w, Scope& scope, int result = 0);

    void say(Where w, Scope& scope);

    // [0] is part of name and not checked. Parses EX: "kalle = 232"
    void set_var(Where w, Scope& scope);

    // assumes to start ON line, NOT BEFORE: \nHERE
    Result_And_Exit eval_block(Where w, const int indent, Scope& scope);

public:
    Parser() = default;
    void start(const char* const file_path);
};

template <bool priority>
Value_And_Where Parser::eval_math(Where w, Scope& scope, int result)
{
    OPER::_ oper = OPER::PLUS;
    ++w;
    while (true) {
        switch (code[w]) {
        case ' ':
            ++w;
            break;
        case '(':
        {
            auto extract = eval_math<>(w, scope);
            w = extract._w;
            result = use_oper(oper, result, extract._value);
            break;
        }
        case ')':
            ++w;
            goto END_OF_EVAL_MATH;
            break;
        case '*':
            oper = OPER::MULT;
            ++w;
            break;
        case '+':
            
            oper = OPER::PLUS;
            ++w;
            if constexpr (priority) { goto END_OF_EVAL_MATH; }
            break;
        case '>':
            oper = OPER::MORE_THAN;
            ++w;
            break;
        case '<':
            oper = OPER::LESS_THAN;
            ++w;
            break;
        case '-':
            
            oper = OPER::MINUS;
            ++w;
            if constexpr (priority) { goto END_OF_EVAL_MATH; }
            break;
        case '%':
            oper = OPER::MOD;
            ++w;
            break;
        case '=':
            oper = OPER::EQUAL;
            ++w;
            break;
        default:

            // Direct value, is next value, EX: 2, (...), func(...), 42
            // w should be set to char directly after this expression

            int direct_value; // value which appeared first, will then be subjected to looking forward from
            if (Util::is_num(code[w])) { // is number
                auto extracted = Util::extract_num(w);
                DEBUG_MSG("IS NUM!!! " << " num: " << extracted._value << "\n");
                w = extracted._w;

                direct_value = extracted._value;
            }
            else if (Util::legal_name_char(code[w])) {
                // it is a variable OR FUNCTION
                auto extr_name = Name_Util::extract_name(w);
                w = extr_name._w; // now set to after name
                if (code[w] == '(') { // FUNCTION, this will put w AFTER function ...(...)HERE

                    Func& func = _funcs.get_func(extr_name._name);

                    if (code[w + 1] != ')') // there are args
                    { 
                        // what parameter we are on
                        int param = 0;
                        while (true) { // get all arguments
                            if (code[w] == ')') {
                                ++w; // go to 
                                break;
                            }
                            else if (code[w] == '(' || code[w] == ',')
                            {
                                auto arg_extract = eval_math<>(w, scope);

                                w = arg_extract._w - 1;
                                // add var
                                func._scope.set_value(func._param_names[param], arg_extract._value);
                                ++param;
                            }
                            else {
                                ++w;
                            }
                        }
                    }
                    else {
                        // no params, so w is set to after ...()HERE
                        w += 2;
                    }

                    direct_value = eval_block(func._w, space_per_indent, func._scope)._result;
                }
                else { // Variable, just set to that value. w already handled
                    direct_value = scope.get_value(extr_name._name);
                }
            }
            else { // NO LEGAL CHARACTER AT ALL, QUIT
                DEBUG_MSG("ILLEGAL CHAR: " << code[w] << '\n');
                if constexpr (priority) { ++w; }
                //++w; // will cause expression to return 1+1\n after newline, one character too far.
                goto END_OF_EVAL_MATH;
            }

            // look forward after direct value, possibly apply priority
            auto new_extract = eval_math<true>(w - 1, scope, direct_value);
            w = new_extract._w - 1;
            result = use_oper(oper, result, new_extract._value);

            break;
        }
    }
END_OF_EVAL_MATH:
    DEBUG_MSG("TOTAL: " << result << "\n");
    return { result, w };
}


