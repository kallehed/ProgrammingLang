#include "Parser.h"
#include "Testing.h"

char* const code = new char[MAX_CODE_LEN];

int main()
{

    Testing::do_tests();

    Parser p;
    p.start("code3.kal");
}