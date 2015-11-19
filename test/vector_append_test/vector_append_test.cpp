// vector_append_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/fit/vector_append.h"


namespace buffer_append_head_test
{
	void test_char()
	{
		char buff[16];
		ox::fit::buffer_append<char> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append("12345",4);
		buffer.append("abcdef",4);
	}
	void test_wchar()
	{
		char buff[16];
		ox::fit::buffer_append<wchar_t> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append(L"12345",2);
		buffer.append(L"abcdef",2);
	}
	void test()
	{
		test_char();
		test_wchar();
	}
}


namespace buffer_append_test
{
	void test_char()
	{
		char buff[16];
		ox::fit::buffer_append<char> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append("12345",4);
		buffer.append("abcdef",4);
	}
	void test_wchar()
	{
		char buff[16];
		ox::fit::buffer_append<wchar_t> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append(L"12345",2);
		buffer.append(L"abcdef",2);
	}
	void test()
	{
		test_char();
		test_wchar();
	}
}

namespace string_append_test
{
	void test_char()
	{
		char buff[16];
		ox::fit::string_append<char> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append("12345",4);
		buffer.append("abcdef",4);
	}
	void test_wchar()
	{
		char buff[16];
		ox::fit::string_append<wchar_t> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append(L"12345",2);
		buffer.append(L"abcdef",3);
		wprintf(L"%s",buffer.data());
	}
	void test()
	{
		test_char();
		test_wchar();
	}
}

#include <string>
namespace loop_buffer_append_test
{
	void test_char()
	{
		char buff[16];
		ox::fit::loop_buffer_append<char> str;
		str.attach(ox::utl::data_t(buff));
		str.clear();
		str.append("12345",3);
		str.append("1234567890",6);
		str.append("abcdefghijklmn",10);
		printf("%s",str.data());
	}
	void test_wchar()
	{
		char buff[16];
		ox::fit::loop_buffer_append<wchar_t> str;
		str.attach(ox::utl::data_t(buff));
		str.clear();
		str.append(L"12345",2);
		str.append(L"1234567890",3);
		str.append(L"abcdefghijklmn",10);
		wprintf(L"%s",str.data());
	}
	void test()
	{
		test_char();
		test_wchar();
	}
}
namespace loop_string_append_test
{
	void test_char()
	{
		char buff[16];
		ox::fit::loop_string_append<char> str;
		str.attach(ox::utl::data_t(buff));
		str.clear();
		str.append("12345",3);
		str.append("1234567890",6);
		str.append("abcdefghijklmn",10);
		printf("%s",str.data());
	}
	void test_wchar()
	{
		char buff[16];
		ox::fit::loop_string_append<wchar_t> str;
		str.attach(ox::utl::data_t(buff));
		str.clear();
		str.append(L"12345",2);
		str.append(L"1234567890",2);
		str.append(L"abcdefghijklmn",10);
		wprintf(L"%s",str.data());
	}
	void test()
	{
		test_char();
		test_wchar();
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	switch(1) { default:
	buffer_append_test::test();break;
	loop_string_append_test::test();break;
	loop_buffer_append_test::test();break;
	string_append_test::test();break;
	};
	return 0;
}

