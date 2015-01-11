// filekit_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/arc/ox/io/filekit.h"

namespace eachline_test
{
	typedef char character;
	typedef ox::io::foreach_fileline<character> eachline_t;
	typedef eachline_t::line_handle line_handle;
	typedef eachline_t::std_string std_string;

	bool on_line(character const* begin,character const* end)
	{
		std_string str(begin,end-begin);
		printf ("%s\n",str.c_str());
		return true;
	}

	void test()
	{
		eachline_t eachline;
		eachline("D:\\test\\1.txt",line_handle(&on_line));
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	eachline_test::test();
	return 0;
}

