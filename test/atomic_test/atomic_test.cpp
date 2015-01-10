// atomic_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/kernel/atomic_number.h"

namespace atomic_test
{
	void test()
	{
		atomic_long atomic(0);
		atomic++;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	atomic_test::test();
	return 0;
}

