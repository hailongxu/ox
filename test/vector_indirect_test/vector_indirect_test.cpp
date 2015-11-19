// vector_indirect_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/fit/vector_indirect.h"



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


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

