// datetime_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"

#include "../../src/thread/log_print.h"


namespace datetime_test
{
	void test()
	{
		ox::communication_log log;
		log.init();
		log.logln("111");
		log.logln("22222");
		log.logln("33333");
		log.logln("4444");

		std::string path = ox::communication_log::get_log_path();

		SYSTEMTIME st;
		GetLocalTime(&st);
		SYSTEMTIME st2;
		GetSystemTime(&st2);
		FILETIME ft;
		SystemTimeToFileTime(&st2,&ft);
		//SystemTimeToTzSpecificLocalTime();
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	datetime_test::test();
	return 0;
}

