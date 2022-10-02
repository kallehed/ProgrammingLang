#include "Testing.h"

#include "Util.h"
#include "Name.h"

#include <cassert>

void Testing::do_tests()
{
	// Util
	{ // Util::extract_num
		auto e = Util::extract_num("348938");
		assert(e.value == 348938);
		assert(e.length == 6);

		e = Util::extract_num("4");
		assert(e.value == 4);
		assert(e.length == 1);
	}
	{ // Util::ipow
		auto e = Util::ipow(2, 3);
		assert(e == 8);
		e = Util::ipow(1, 5);
		assert(e == 1);
		e = Util::ipow(34, 0);
		assert(e == 1);
	}
	{ // Util::is_num
		assert(Util::is_num('3'));
		assert(!Util::is_num('a'));
		assert(Util::is_num('0'));
		assert(!Util::is_num('!'));
		assert(!Util::is_num('\0'));
	}
	{ // Util::legal_var_name_char
		assert(Util::legal_name_char('a'));
		assert(Util::legal_name_char('z'));
		assert(!Util::legal_name_char('0'));
		assert(!Util::legal_name_char(' '));
		assert(!Util::legal_name_char('\n'));
	}
	{ // Util::len_to_char
		assert(Util::len_to_char("12345", '5') == 4);
		assert(Util::len_to_char("abbba", 'a') == 4);
		assert(Util::len_to_char("fjdifosjfgidkfjsdos4295483948398492039vnkdn  ajsdn\n\0\t343A", 'A') == 56);
	}
	{ // Util::len_while_condition
		assert(Util::len_while_condition("0abc3", Util::legal_name_char) == 4);
		assert(Util::len_while_condition("q6803O", Util::is_num) == 5);
	}
	{ // Util::str_equal
		assert(Util::str_equal("abcd", "abcd", 4));
		assert(!Util::str_equal("12345", "12346", 5));
		assert(Util::str_equal("\0", "\0", 2));
	}
	{ // Util::til_right_char
		assert((Util::til_right_char<'A', 'B'>("BAdifsjdoffBdoijfdfAodifjBBA") == 26));
		assert((Util::til_right_char<'(', ')'>("((()))") == 5));
		assert((Util::til_right_char<'S', 'E'>("\nE") == 1));
	}

	// Name_Util
	{ // Name_Util::extract_name
		auto e = Name_Util::extract_name("alexia   ");
		Name n0;
		n0.chars = {"alexia"};
		assert(e.name == n0);
		assert(e.length == 6);

		e = Name_Util::extract_name("akgjdkgi+-+-+");
		Name n1;
		n1.chars = { "akgjdkg" };
		n1.chars[7] = 'i';
		assert(e.name == n1);
		assert(e.length == 8);
		e = Name_Util::extract_name("akgjdkgj\n*");
		assert(e.name != n1);
		assert(e.length == 8);
	}

	//
}
