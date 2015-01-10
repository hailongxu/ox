// path_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include "../../src/kernel/winop.h"
#include "../../src/utl/path_kit.h"

namespace path_test
{
	void test()
	{
		//std::string sss = sys::get_current_exe_path<char>();
		//ox::utl::localpath_kit<char>::keep_last(sss);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	path_test::test();
	return 0;
}

