#include "Parser.h"
#include "Testing.h"

int main()
{
    Testing::do_tests();

    Parser p;
    p.start("code3.kal");
}