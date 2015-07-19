// vector_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/fit/vector.h"


struct act
{
	bool operator()(int& v)
	{
		printf("%d\n",v);
		return true;
	}
};

namespace vector_test
{
	void test()
	{
		typedef ox::fit::vector<int> V;
		V v;
		v.push_back(1);
		v.push_back(3);
		v.insert(1,2);
		v.insert(1,4);
		v.erase(1);
		typedef ox::fit::static_vector<int[4]> SV;
		SV sv;
		sv.push_back(1);
		sv.push_back(2);
		sv.push_back(3);
		sv.push_back(4);
		//sv.push_back(5);
		typedef ox::fit::string<char> S;
		S s;
		s.push_back('1');
		s.push_back('3');
		s.insert(1,'2');
		s.insert(1,'4');
		typedef ox::fit::static_string<char[4]> SS;
		SS ss;
		ss.push_back('1');
		ss.push_back('2');
		ss.push_back('3');
		ss.push_back('4');
	}
}

namespace variable_vector_test
{
	void test()
	{
		ox::fit::variable_indirect_vector<char> ddd;
		ddd.push_back(ox::utl::data_t(0,10));
	}
}

namespace indirect_vector_test
{
	void test()
	{
		typedef ox::fit::indirect_vector<int> IV;
		IV iv;
		iv.resize(1);
		iv.push_back(2);
		iv.erase(1);

		iv.foreach(act());

		typedef ox::fit::static_indirect_vector<int,int*[3]> SIV;
		SIV siv;
		siv.push_back(1);


		//typedef ox::fit::mono_indirect_vector<int> MIVR;
		//MIVR mivr;
		//mivr.allocate_object(2);

		//typedef ox::fit::mono_space_indirect_vector<char> space_t;
		//space_t space;
		//space.push_back(ox::utl::data_t(0,12));
	}
}

#include "../../src/fit/vector_append.h"

namespace vector_append_test
{
	void test()
	{
		char buff[16];
		ox::fit::buffer_append<char> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append("12345",4);
		buffer.append("abcdef",4);
	}
}

namespace string_append_test
{
	void test()
	{
		char buff[16];
		ox::fit::string_append<char> buffer;
		buffer.attach(ox::utl::data_t(buff));
		buffer.clear();
		buffer.append("12345",4);
		buffer.append("abcdef",4);
	}
}

#include <string>
namespace loop_vector_append_test
{
	void test()
	{
		char buff[16];
		ox::fit::loop_vector_append<char> buf;
		buf.attach(ox::utl::data_t(buff));
		buf.clear();
		buf.append("12345",4);
		buf.append("abcdefghijklmn",10);
		std::string str(buf.data(),buf.size());
		printf("%s",str.data());
	}
}
namespace loop_string_append_test
{
	void test()
	{
		char buff[16];
		ox::fit::loop_string_append<char> str;
		str.attach(ox::utl::data_t(buff));
		str.clear();
		str.append("12345",4);
		str.append("abcdefghijklmn",10);
		printf("%s",str.data());
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	//std::vector<int,ox::fit::stl_allocator<int>> v;
	//v.reserve(3);
	switch(1) { default:
	loop_string_append_test::test();break;
	loop_vector_append_test::test();break;
	string_append_test::test();break;
	vector_append_test::test();break;
	indirect_vector_test::test();break;
	variable_vector_test::test();break;
	vector_test::test();break;
	};
	return 0;
}

