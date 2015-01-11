// mixture_test.cpp : Defines the entry point for the console application.
//

// mixture_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../../src/met/tslist.h"
#include "../../src/mix/mixture.h"
#include "../../src/mix/mixture_feature_def.h"



#include "../../src/mix/mixture_type_access.h"
#include "../../src/mix/mixture_access.h"

namespace test_0
{
	namespace met = ox::met;
	namespace mix = ox::mix;
	namespace tag = ox::tags;


	template <typename sym,size_t i,size_t iow,size_t b,typename inf_lmd>
	struct test_def : inf_lmd::call<sym>::base
	{
		___def_method_forward(test_def,inf_lmd,sym,inf)

		size_t operator()()
		{
			size_t k = i;
			size_t j = iow;
			return k;
		}
		ox::itot<inf::jfeature::idiff::value> iiii;
		ox::identity<typename inf::jfeature::unit> jjjj;
	};

	template <typename sym,size_t i,size_t iow,size_t b,typename inf_lmd>
	struct test2_def : inf_lmd::call<sym>::base
	{
		___def_method_forward(test2_def,inf_lmd,sym,inf)

		typename inf::reference
		operator()()
		{
			inf::method<iow>(this);
			return inf::data<iow>(this);

			typedef typename inf::owner_type owner_type;
			owner_type* o = static_cast<owner_type*>(this);

			//typedef ox::mix::data_addrat<ox::mix::index<iow>> at;
			//typedef ox::mix::data_typeat<owner,ox::mix::index<iow>,int&> tat;
			//typename tat::type m;
			//addr::type c;
			return ox::mix::data<iow>(*o);
			//return 3;
		}
	};

	struct unitize : ox::mix::unitize
	{
		typedef test_def<void,0,0,0,void> method_unit;
	};


	typedef test2_def<void,0,0,0,void> method_unit;

	___def_0_features(
		list0
	);
	___def_3_features(
		list3,
		2(void,((int*))),
		2(char,2((char*),(long const*))),
		2(void,2((char*),(int const*)))
	);
	___def_3_features(
		list4,
		2(char,((int*))),
		2(char /*const*/,2(d(char*),(long const*))),
		2(void,2((char*const),(int const*)))
	);
	___def_2_features(
		list5,
		(char),
		2(char,(2(char*,method_unit)))
	);
	___def_1_features(
		list6,
		2(ox::tags::owner,(d(int*)))
	);
	___def_4_features(
		list7,
		2(char,((int*))),
		2(char /*const*/,2(d(char*),(long const*))),
		2(void,2((char*const),(int const*))),
		2(int,2((char*const),(int const*)))
	);

	___def_2_features(
		list8,
		(long long),
		(int)
	);
	___def_4_features(
		list9,
		(void),
		(int),
		(void),
		(int)
	);

	___def_1_features(
		list10,
		2(int,(x(int*,int**,2)))
	);


	typedef ox::mix::mixture<list0<0>::type,unitize> mixture0;
	typedef ox::mix::mixture<list3<0>::type,unitize> mixture3;
	typedef ox::mix::mixture<list4<0>::type,unitize> mixture4;
	typedef ox::mix::mixture<list5<0>::type,unitize> mixture5;
	typedef ox::mix::mixture<list6<0>::type,unitize> mixture6;
	typedef ox::mix::mixture<list7<0>::type,unitize> mixture7;
	typedef ox::mix::mixture<list8<0>::type,unitize> mixture8;
	typedef ox::mix::mixture<list9<0>::type,unitize> mixture9;
	typedef ox::mix::mixture<list10<0>::type,unitize> mixture10;
}

namespace construct_test
{
	using namespace test_0;
	// ????????????????????????????
	// bugs happen in data method
	//?????????????????????????????????

	void test()
	{
		mixture6 t2;
		mixture5 t5(ox::ref('a'),ox::fwd('b'));
		mixture4 t4(ox::ref('1'));
		mixture5 t3(mix::tcc(t4));
		mixture4 t0 = mix::tcc(t5);
		mixture4 t_1(mix::tcc(t5));
	}
}

namespace copy_assign_test
{
	using namespace test_0;
	void test()
	{
		mixture5 t5(ox::ref('a'),ox::ref('b'));
		mixture4 t4(ox::ref('1'),ox::ref('2'));
		mixture7 t7(ox::ref('a'),ox::ref('b'),ox::ref(9));
		mixture3 t3(ox::ref('e'));
		mixture0 t0;
		t4 = mix::tcc(t5);
		t4 = mix::tcc(t3);
		mixture9 t9(ox::ref(4),ox::ref(5));
		t4 = mix::tcc(t9);
		mixture4 t4_2(mix::tcc(t9));
	//	//t4 = '5';
	}
}

namespace access_test
{
	using namespace test_0;
	using namespace ox;
	void test()
	{
		mixture4 t4;
		mixture3 t3;
		int ii = sizeof(t3);
		mix::owner_addrat<mix::index<1>>()(t4).value();// = 'r';
		mix::owner_addrat<mix::saddr<char*>>()(t4).value();// = 'f';
		typedef mix::owner_addrat<mix::saddr<char*>> owner_addrat;
		owner_addrat::typeat<mixture4>::type bref = owner_addrat()(&t4);

		typedef mix::owner_addrat<mix::saddr<char*> const*> ownerp_addrat;
		ownerp_addrat::typeat<mixture4>::type bptr = ownerp_addrat()(&t4);
		bool b = ownerp_addrat::__is_ptr;
		ownerp_addrat::addr c;
		typedef met::rmv<int const*const&>::from<long*>::type addr;
		___static_assert_same_type(addr,long);

		ox::mix::owner_addrat<ox::mix::saddr<char*,0>>()(t4);
		//mixture4 const& t5 = t4;
		mix::owner<char*>(t4);
		mix::owner<1>(t4);
		typedef mix::data_addrat<ox::mix::saddr<char*>> data_addrat;
		data_addrat()(&t4);
		mix::data<char*>(t4);
		mix::data<1>(t4);
		data_addrat::typeat<mixture4>::type cref = data_addrat()(t4);
		mix::method_addrat<ox::mix::saddr<char*>>()(t4);
		mix::method_addrat<ox::mix::saddr<char*>>()(&t4);
		mix::method<char*>(t4)();
		mix::method<1>(t4)();
		mix::method<1,1>(t4)();

		mix::unit_addrat<ox::mix::index<1>>()(t4);
		mix::unit_addrat<ox::mix::saddr<char*>>()(t4);
		mix::unit_addrat<ox::mix::saddr<char*,0>>()(t4)();
		mix::unit<char*>(t4);
		//mix::unit<char*,0>(t4)();

		mixture5 t5(ox::ref('3'),ox::ref('4'));
		typedef mix::method_typeat<mixture5,mix::saddr<char*>>::type m5t;
		char& cc2 = mix::method_addrat<mix::saddr<char*>>()(t5)();
		cc2 = '8';
	}
}


//namespace macro_string_test
//{
//	void test()
//	{
//		char* s = ___def_to_string(___def_inner_method_class(meth,((jjj))));
//		s = ___def_to_string(___def_inner_tul_class_name_1(list,((a))));
//		s = ___def_to_string(___def_1_features(list,2(void,((int*)))));
//		s = ___def_to_string(___def_get_param_count((P)));
//		s = ___def_to_string(___def_inner_method_class(name,void));
//	}
//}
//
//#include "../../src/mix/mixture_param.h"
//
//namespace param_test
//{
//	typedef ox::mix::param2<int const,long const> param;
//
//	void test()
//	{
//		param p(ox::ref(1),ox::ref(2));
//	}
//}

//struct M3 {size_t d3;};
//struct M2 {/*char* d2;*/};
//struct M1 {int* d1;};
//
//struct D3 : M3
//{
//	//size_t d3;
//};
//struct D2 : M2
//{
//	//int* d2;
//};
//
//struct D1 : M1
//{
//	//int* d1;
//};
//
//struct A3: D3
//{
//};
//struct A2 : D2,A3
//{
//};
//struct A1: D1, A2
//{
//};
//


struct Class {
	Class()
	{
		int i = 9;
	}
};
___def_2_features(
	list8,
	2(int,((int*))),
	(int /*const*//*,2(d(char*),(long const*))*/)//,
	//2(void,2((char*const),(int const*)))
);

//___def_2_features(
//	list8,
//	2(int,2((long*),d(int*))),
//	2(Class,((char*)))/*,
//	2(void,((void*)))*/
//);
typedef ox::mix::mixture<list8<0>::type> mixture8;
//typedef ox::mix::mixture<list4<0>::type,unitize> mixture4;

////struct F
////{
////	F()
////	{
////	}
////	F(int i){
////	a=i;
////} int a;};
////struct F2 : virtual F
////{int b;};
////struct F3 : F2
////{
////	F3() : F(4) {}
////	int c;
////};
////struct F4: F3
////{
////	F4() : F(3) {}
////};


//namespace mixture_tlist_1
//{
//	typedef ox::tslist<void,ox::mix::mlist<void>,void,void> slist;
//	typedef ox::mix::tlist<slist> feature;
//	typedef ox::mix::mixture<feature> mixture1;
//
//	void test()
//	{
//		mixture1 m1;
//	}
//}


//namespace mixture_tlist_2
//{
//	typedef ox::tslist<int,int,void,ox::tslist<long,long,void,void>> slist;
//	typedef ox::mix::tlist<slist> feature;
//	typedef ox::mix::mixture<feature> mixture1;
//
//	void test()
//	{
//		mixture1 m1;
//	}
//}


#include <memory.h>
int _tmain(int argc, _TCHAR* argv[])
{
	//size_t ii = sizeof(F4::F2);
	//F4 f4;
	//A1 a1;
	//mixture8 m8(ox::mix::make_param(M3()),ox::itot<1>());
	mixture8 m82(ox::ref(1),ox::ref(2));
	//m8 == m82;
	//memset(&m8,0,sizeof(m8));
	//size_t ss = sizeof(A1);
	//size_t s = sizeof(mixture8);

	//macro_string_test::test();
	//to_string_test::test();
	construct_test::test();
	copy_assign_test::test();
	access_test::test();


	//mixture_tlist_1::test();
	////mixture_tlist_2::test();

	return 0;
}

