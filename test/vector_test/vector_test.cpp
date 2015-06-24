// vector_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/fit/vector.h"



namespace vector_test
{
	struct act
	{
		bool operator()(int& v)
		{
			printf("%d\n",v);
			return true;
		}
	};
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
		typedef ox::fit::indirect_vector<int> IV;
		IV iv;
		iv.resize(1);
		iv.push_back(2);
		//iv.erase(1);
		
		iv.foreach(act());

		typedef ox::fit::static_indirect_vector<int,int*[3]> SIV;
		SIV siv;
		siv.push_back(1);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	vector_test::test();
	return 0;
}

