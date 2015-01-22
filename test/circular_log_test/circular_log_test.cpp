// circular_log_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/thread/alog.h"


typedef ox::win_queue_thread thread_t;

namespace log_test
{
	ox::alog_t alog;
	void on_exit(ox::win_queue_thread*)
	{
		printf ("exited");
	}
	void test()
	{
		alog.on_exit().assign(on_exit);
		alog.start("d:/1.txt");
		alog.log("123\n",0);
		//alog.stop();
	}
}


#include "../../src/io/circular_logfile.h"

namespace circular_log_test
{
	void test()
	{
		typedef ox::io::circular_logfile<char> logfile;
		logfile lf;
		lf.open("d:/1.txt",12);
		lf.log("12345\r\n");
		lf.log("abcdefg\r\n");
	}
}

#include "../../src/pkg/string_simple_kit.h"
namespace ss_test
{
	typedef ox::str::astring_kit strkit;
	void test()
	{
		std::string str;
		strkit::sprintf(str,"111%d%lf####@",9876,0.0965);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	do {
		log_test::test();
		break;
		ss_test::test();
		break;
		circular_log_test::test();
		break;
	} while(false);
	thread_t mt;
	mt.start_here();
	return 0;
}

