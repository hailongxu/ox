// share_memory_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../src/kernel/share_file.h"


namespace share_file_test
{
	ox::shared_memory sm;
	void test()
	{
		std::string str;
		sm.create("1111111",16);
		sm.write_all("123",3);
		sm.read_all(str);
		sm.append("45abc",5);
		sm.read_all(str);
		sm.append_circle("6789012",7);
		sm.read_all(str);
		sm.append_circle("hijklmnop",8);
		sm.read_all(str);
		sm.append_circle("abcdefghijklmnopqrst",16);
		sm.read_all(str);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	share_file_test::test();
	return 0;
}

