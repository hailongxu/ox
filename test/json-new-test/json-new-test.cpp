// json_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"
#include "windows.h"
#include <io.h>
#include <algorithm>
#include <iostream>
#include "assert.h"
#include "../../src/app/json/json.h"
#include "../../src/app/json/json_use.h"

#define JSON_FILE_DIR "j:/proj/oxlib.git/ox/test"

char*jsonfiles2[8] =
{
	JSON_FILE_DIR"/json.file/webapp.json",
	JSON_FILE_DIR"/json.file/test0-u.json",
	"",
	JSON_FILE_DIR"/json.file/test3-u.json", ///   46k
	JSON_FILE_DIR"/json.file/test4-u.json", ///  138k
	JSON_FILE_DIR"/json.file/test5-u.json", ///  484k
	JSON_FILE_DIR"/json.file/test6-u.json", /// 2238k
	JSON_FILE_DIR"/json.file/test7-u.json", /// 1147k
};
char*jsonfiles1[8] =
{
	JSON_FILE_DIR"/json.file/test$.json",
	JSON_FILE_DIR"/json.file/test0.json",
	"",
	JSON_FILE_DIR"/json.file/test3.json", ///   46k
	JSON_FILE_DIR"/json.file/test4.json", ///  138k
	JSON_FILE_DIR"/json.file/test5.json", ///  484k
	JSON_FILE_DIR"/json.file/test6.json", /// 2238k
	JSON_FILE_DIR"/json.file/test7.json", /// 1147k
};
typedef char* (aaaatype)[8];

aaaatype* bbbbs[] =
{
	0,
	&jsonfiles1,
	&jsonfiles2
};

#include <atlstr.h>

//struct TT
//{
//	void operator[](size_t)
//	{
//
//	}
//	void operator[](json::str_tt<char>)
//	{
//
//	}
//};


int _tmain(int argc, _TCHAR* argv[])
{
	typedef wchar_t character;
	FILE * pFile;
	character* mystring = 0;

	//pFile = fopen ("C:/Users/Anming/Desktop/rapidjson-0.11/rapidjson/bin/data/webapp.json" , "r");
	char const* path = (*bbbbs[sizeof(character)])[1];
	pFile = fopen ( path, "r");
	long length = 0;
	if (pFile != NULL)
	{
		length = filelength(fileno(pFile))/sizeof(character)+2;
		mystring = new character [length];
		size_t size = fread (mystring,sizeof(character),length,pFile);
		mystring[size] = 0;
		fclose (pFile);
	}


	size_t time_sum = 0;
	static size_t const test_count = 1;
	//for (int i=0; i<test_count; ++i)
	//{
	{
		////doc.push_back(L"abc");
		////std::wstring str2 = doc.root().AsString();
		//wchar_t const* str = L"\"\\\"str\\\"\"";
		//wchar_t const* str = L"{\"address\":[{\"address1\":[{\"email1\":[{\"ll\":\"elite_yang@163.com\"},{\"ll\":\"elite_yang@163.com\"},{\"ll\":\"elite_yang@163.com\"}]}]}]}";
		//wchar_t const* str = L"[\"ke\\\/y1\",\"value1\"]";
		//wchar_t const* str = L"{\"ke\\\/y1\":\"value1\"}";
		//wchar_t const* str = L"{ \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3] } ";
		DWORD s = GetTickCount();
		character const* str = mystring;
		json::json_document<character> doc;
		json::json_value<character> const& jb = doc.parse(str);
		JSONObject const& dd = jb.as_object();
		int cccc;
		json::get_num_value(dd,L"frameid",cccc);
		JSONObject::const_iterator uu = dd.find(L"frameid");
		std::wstring sss = uu->second->as_string();
		

		DWORD e = GetTickCount();
		time_sum += (e-s);
		//std::basic_ostream<character> cout;
		printf("%d\n",time_sum);
	}
	//}


	//std::wstring& str2 = json.Stringify();
	//printf("===%d\n===%d",length-2,str2.length());

	/// 
	//if (0)
	//{
	//	std::string outfilename = jsonfiles[1];
	//	outfilename += ".out";
	//	pFile = fopen(outfilename.c_str(),"w");
	//	fwrite(str2.data(),str2.length(),sizeof(wchar_t),pFile);
	//	fclose(pFile);
	//}

	//json.~JSONValue();
	//bool b = jb->IsString();
	getch();
	return 0;
}

