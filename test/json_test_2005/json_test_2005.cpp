// json_test_2005.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/app/json/json.h"

namespace test_stringify
{

	void test()
	{
		wjson_document doc;
		wjson_array* ja = doc.make_json_array();
		ja->push_back(L"1234");
		std::wstring str = ja->stringify();
	};

}




int _tmain(int argc, _TCHAR* argv[])
{
	test_stringify::test();
	return 0;
}

