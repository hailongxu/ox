// mixture_scan_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/mix/mixture.h"

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
		2(char,(x(int*,int**,2)))
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



#include <iostream>
#include "../../src/mix/mixture_scan.h"

namespace scan_test
{
	template <typename path,typename i,typename tag>
	struct print
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(int& m) {}
		template <typename type,typename owner,typename origin>
		ox::treectrlid operator()(type& v,owner& o,origin& or)
		{
			if (i::value==0) printf("\n----\n");
			//printf ("%c,",v.value());
			std::cout<<v.value()<<',';
			return ox::treectrlid::make_continue();
		}
	};
	template <typename path,typename i>
	struct print <path,i,ox::tags::begin>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(int& m) {}
		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			printf ("{");
			return ox::treectrlid::make_continue();
		}
	};
	template <typename path,typename i>
	struct print <path,i,ox::tags::end>
	{
		static size_t const controlid = ox::treectrlid::__continue;
		print(int& m) {}
		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			printf ("}");
			return ox::treectrlid::make_continue();
		}
	};

	/// 
	template <typename path,typename i,typename tag>
	struct print0
	{
		static size_t const controlid = ox::treectrlid::__continue;

		template <typename type,typename owner,typename origin>
		ox::treectrlid operator()(type& v,owner& o,origin& or)
		{
			if (i::value==0) printf("\n----\n");
			//printf ("%c,",v.value());
			std::cout<<v.value()<<',';
			return ox::treectrlid::make_continue();
		}
	};
	template <typename path,typename i>
	struct print0 <path,i,ox::tags::begin>
	{
		static size_t const controlid = ox::treectrlid::__continue;

		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			printf ("{");
			return ox::treectrlid::make_continue();
		}
	};
	template <typename path,typename i>
	struct print0 <path,i,ox::tags::end>
	{
		static size_t const controlid = ox::treectrlid::__continue;

		template <typename mixture,typename parent,typename origin>
		ox::treectrlid operator()(mixture& o,parent& p,origin& or)
		{
			printf ("}");
			return ox::treectrlid::make_continue();
		}
	};


	void test()
	{
		//size_t size = ox::mix::sizeof_mixture<mixture3>::value;
		test_0::mixture7 t3('3','4',5);
		int a = 9;
		test_0::mixture7 const& t4 = t3;
		ox::mix::mixture_scan<ox::met::lambda3<print0>::type>()(t3);
		ox::mix::mixture_scan<ox::met::lambda3<print0>::type>()(t4);
		ox::mix::mixture_scan<ox::met::lambda3<print>::type>()(t3,a);
		ox::mix::mixture_scan<ox::met::lambda3<print>::type>()(t4,a);

		ox::mix::mixture_scan<ox::met::lambda3<print0>::type,'-'>()(t3);
		ox::mix::mixture_scan<ox::met::lambda3<print>::type,'-'>()(t3,a);
	}
}

#include "../../src/cxx/to_string.h"

namespace to_string_test
{
	void test()
	{
		char str[128];
		ox::cxx::obj_to_string<size_t>()(3,str);
		test_0::mixture9 t3(3,5);
		size_t s = ox::cxx::to_string(t3,str,'@');
		printf ("\n%s\n",str);
	}
}


//namespace scan_type_test
//{
//	struct type_print
//	{
//		void print(size_t n)
//		{
//			size_t i=0;
//			while (i++<n) printf("    ");
//		}
//		/// data
//		template <typename unit,size_t depth,size_t i>
//		ox::treectrlid begin()
//		{
//			print(depth);
//			printf ("{\n");
//			return ox::treectrlid::make_continue();
//		}
//		template <typename unit,size_t depth,size_t i>
//		ox::treectrlid end()
//		{
//			print(depth);
//			printf ("}\n");
//			return ox::treectrlid::make_continue();
//		}
//		template <typename unit,size_t depth,size_t i>
//		ox::treectrlid operator()()
//		{
//			print(depth+1);
//			printf ("%s:\n",unit::ifeature::to_string());
//			return ox::treectrlid::make_continue();
//		}
//		/// method
//		template <typename method,size_t depth,size_t i,size_t j>
//		ox::treectrlid begin()
//		{
//			print(depth+1);
//			printf ("[");
//			return ox::treectrlid::make_continue();
//		}
//		template <typename method,size_t depth,size_t i,size_t j>
//		ox::treectrlid end()
//		{
//			printf ("\n");
//			print(depth+1);
//			printf ("],\n");
//			return ox::treectrlid::make_continue();
//		}
//		template <typename method,size_t depth,size_t i,size_t j>
//		ox::treectrlid operator()()
//		{
//			printf ("\n");
//			print(depth+2);
//			printf ("%s",method::ifeature::to_string());
//			printf (method::ifeature::idiff::value?" !":"");
//			return ox::treectrlid::make_continue();
//		}
//	};
//	___def_2_features(
//		list,
//		2(void,((int*))),
//		/*2*/(char/*,2(d(char*),(long const*))*/)//,
//		//2(void,2((char*),(int const*)))
//	);
//	typedef ox::mix::mixture<list<0>::type,ox::mix::unitize> mixture3;
//
//	void test()
//	{
//		printf ("\n----mixture type print ------\n");
//		ox::mix::mixture_type_scan<mixture3>()(type_print());
//	}
//}
//
int _tmain(int argc, _TCHAR* argv[])
{
	scan_test::test();
	to_string_test::test();
	return 0;
}
