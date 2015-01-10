// lambda_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/met/tslist_decl.h"
#include "../../src/met/tslist.h"

#include "../../src/met/lambda.h"
#include "../../src/met/lambda0.h"
#include "../../src/met/static_assert.h"


using namespace ox;
using namespace ox::met;


namespace series_same
{
	typedef tslist<int,long> tslist1;
	typedef tslist<void,void> tslist2;
	static bool const __is_same = is_same_series<tslist1,tslist2>::value;
	___static_assert(__is_same);
}
namespace series_same2
{
	typedef tslist<int,long> tslist1;
	typedef mkslist<void,int,int>::type tslist2;
	typedef is_same_series<tslist1,tslist2> is_same_type;
	
	static bool const __is_same = is_same_series<tslist1,tslist2>::value;
	___static_assert(__is_same);
}
namespace series_unsame2
{
	template<typename> struct A {};
	typedef tslist<int,long> tslist1;
	typedef A<void> tslist2;	
	static bool const __is_same = is_same_series<tslist1,tslist2>::value;
	___static_assert(!__is_same);
}
namespace call_test
{
	//template<typename> struct A {};
	typedef lambda<add_con<ox::_1>>::call<int>::type t1;
	___static_assert_same_type(t1,int const);

	typedef lambda<add_con<long>>::call<int>::type t2;
	___static_assert_same_type(t2,long const);
}

#include "../../src/met/mod.h"
namespace assert_test
{
	template <typename,typename> struct A{};
	template <typename,typename> struct B{};
	static bool const is = is_same<A<int,int>,B<int,int>>::value;
	___static_assert(!is);
	//___static_assert(!is_same<A<int,int>,B<int,int>>::value);
	static bool const is2 = is_same<void,void>::value;
	___static_assert(is2);
}

namespace lambda_is_test
{
	template <typename T> struct LL {};
	typedef ox::met::mkslist<int>::type slist;

	typedef ox::met::lambda<slist> lam;
	___static_assert(lam::is::value);

	typedef ox::met::lambda<LL<int>> lam2;
	___static_assert(lam2::is::value);
}

namespace lambdax_test
{
	template <typename,typename> struct A{typedef int type;};
	template <typename,typename> struct B{typedef short type;};

	typedef lambda2<A>::call<int,int>::type t1;
	typedef lambda2<B>::call<int,int>::type t2;
}



int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

