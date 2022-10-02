#include "misc.h"

int use_oper(OPER::_ oper, int start_value, int new_value) {
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
    case OPER::MOD:
        return start_value % new_value;
    case OPER::EQUAL:
        return start_value == new_value;
    default:
        for (;;) {}
    }
}
