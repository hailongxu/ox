// mod_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/met/static_assert.h"
#include "../../src/met/mod.h"

using namespace ox;

namespace cvatom_test
{
	typedef int*const*& abc;
	typedef met::cvatom<abc>::type ab;
	___static_assert_same_type(ab,int);
}
namespace cvatom_plus_test
{
	typedef int *const * * & abc;
	typedef met::cvatom<abc,1,'+'>::type ab;
	___static_assert_same_type(ab,int*const);
	typedef met::cvatom<abc,0,'+'> result;
	typedef result::type ab2;
	___static_assert_same_type(ab2,int);
}

namespace isenum_1
{
	//1, not class;
	//2, not *,and &
	//3, 
}

int _tmain(int argc, _TCHAR* argv[])
{
	//long ss = cvatom_plus_test::result::__level_size;
	//long ss2 = cvatom_plus_test::result::__level_size_rev;
	cvatom_plus_test::result::type aa;
	return 0;
}

