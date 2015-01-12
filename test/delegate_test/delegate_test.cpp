// delegate_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


///#include "../../src/cxx/delegate.h"
//
//using namespace ox;
//
//namespace delegate1_test
//{
//	int fun(int)
//	{
//		return 2;
//	}
//	struct Int
//	{
//		int fun(int)
//		{
//			return n;
//		}
//		int operator()()
//		{
//			return n;
//		}
//		int n;
//	};
//	typedef cxx::delegate<int(int)> dlg_t;
//	void test()
//	{
//		dlg_t dlg;
//		dlg.add(fun);
//		Int n;
//		n.n = 4;
//		dlg.add(n,&Int::fun);
//		dlg.add(&n);
//		dlg(3);
//	}
//}

#include "../../src/cxx/delegate.h"
#include "../../src/thread/delegate_closure.h"
namespace single_delegate_test
{
	struct fc0
	{
		int m()
		{
			return 4;
		}
		int mc() const
		{
			return 4;
		}
	};
	int f1(int)
	{
		return 2;
	}
	struct fc1
	{
		int operator()(int a)
		{
			return 3;
		}
		int m(int)
		{
			return 4;
		}
		int mc(int) const
		{
			return 4;
		}
	};
	int f2(int,int)
	{
		return 2;
	}
	struct fc2
	{
		int operator()(int a,int)
		{
			return 3;
		}
		int m(int,int)
		{
			return 4;
		}
		int mc(int,int) const
		{
			return 4;
		}
	};
	int f3(int,int,int)
	{
		return 2;
	}
	struct fc3
	{
		int operator()(int a,int,int)
		{
			return 3;
		}
		int m(int,int,int)
		{
			return 4;
		}
		int mc(int,int,int) const
		{
			return 4;
		}
	};

	typedef delegate<int()> d0_t;
	typedef delegate<int(int)> d1_t;
	typedef delegate<int(int,int)> d2_t;
	typedef delegate<int(int,int,int)> d3_t;
	typedef ox::delegate_closure<int(int)> dc1_t;
	typedef ox::delegate_closure<int(int,int)> dc2_t;
	void test()
	{
		//static int  value = is_const<>::value;
		//bool b = is_const<&fc1::m>::value;
		/// one
		d0_t d0(new fc0,&fc0::m);
		d0_t d0c(new fc0,&fc0::mc);
		/// one
		d1_t d(&f1);
		d(2);
		d2_t d21(&f2);
		d2_t d22(new fc2);
		d2_t d23(new fc2,&fc2::m);
		d2_t d23c(new fc2,&fc2::mc);
		//d3_t d31(&f3);
		//d3_t d32(new fc3);
		d3_t d33(new fc3,&fc3::m);
		d3_t d33c(new fc3,&fc3::mc);

		/// zero param
		d0();
		/// one param
		dc1_t dc(&f1,ox::dummy(),3);
		dc();
		dc.assign(&fc1(),ox::dummy(),3);
		dc();

		/// two param
		dc2_t dc2(&f2,ox::dummy(),3,4);
		dc2();
		dc2.assign(&fc2(),ox::dummy(),3,4);
		dc2();
		dc2.assign(&fc2(),&fc2::m,ox::dummy(),3,4);
		dc2();
		//d23c(3,4);
	}
}

#include "../../src/cxx/multi_delegate.h"
namespace multi_delegate_test
{
	int f(int)
	{
		return 0;
	}
	void vf(int)
	{
	}
	typedef multi_delegate<int(int)> md_t;
	typedef multi_delegate<void(int)> vmd_t;
	void test()
	{
		md_t d1;
		d1.add(md_t::delegate_t(&f));
		vmd_t vd1;
		vd1.add(vmd_t::delegate_t(&vf));
		vd1.add(vmd_t::delegate_t(&vf));
		vd1(3);
	}
}

namespace fpsize_test
{
	void f()
	{}
	struct interface
	{
		virtual void ff() = 0;
	};
	struct A {int a;};
	struct B {int b;};
	struct ss: A//,B
	{
		void f() {}
		virtual void ff() {}
	};
	void test()
	{
		size_t s1 = sizeof(&f);
		size_t s2 = sizeof(&ss::f);
		size_t s3 = sizeof(&ss::ff);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	fpsize_test::test();
	multi_delegate_test::test();
	//delegate1_test::test();
	single_delegate_test::test();
	return 0;
}

//template <typename t>
//struct iden
//{
//	typedef t type;
//};
//
//template <template<typename>class t>
//struct dddd
//{
//	typedef typename t<int>::type aaa;
//};
//
//dddd<iden>::aaa ssss;