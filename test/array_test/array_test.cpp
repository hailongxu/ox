// array_test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "../../src/cxx/array.h"
#include "../../src/cxx/begins.h"
#include "../../src/cxx/nexts.h"
#include "../../src/cxx/ends.h"
#include "../../src/cxx/scan_basic.h"
#include "../../src/cxx/ats.h"
#include "../../src/cxx/sizes.h"
#include "../../src/cxx/scan.h"
#include "../../src/cxx/actions.h"


namespace array_test
{
	struct arrhandle
	{
		template <typename dot_tn,typename control>
		void operator()(dot_tn dot,control& ctl)
		{
			ox::mix::method<ox::tags::next>(ctl)(dot,2);
			printf ("%d\n",ox::mix::method<ox::tags::value>(dot)());
		}
	};
	struct arrhandle2
	{
		template <typename ranges>
		bool operator()(ranges& rngs)
		{
			//ox::mix::data<0>(dots);
			int& v = ox::mix::method<ox::tags::value>(ox::mix::data<ox::tags::begin>(ox::mix::data<0>(rngs)))();
			/*ox::valueof(*/ox::beginof(ox::mix::data<0>(rngs))/*)*/;
			ox::beginof<0>(rngs);
			//printf ("%d\n",ox::mix::method<ox::tags::value>(dot)());
			//ox::mix::data<ox::tags::control>(ox::mix::data<0>(rngs));
			return true;
		}
	};
	typedef ox::cxx::array<int[3]>::type intarray;
	typedef ox::cxx::dots<ox::tags::success,int>::type arrdot;
	typedef intarray::metas::layout lay;
	ox::cxx::scan_basic<'+',ox::tags::homogeneity> arrscan;

	template <typename t>
	void ff(t const& v)
	{
		int i = 9;
	}
	void test()
	{
		intarray arr1 = {{1,2,3}};
		arrdot ddd;
		arrdot dot_begin = ox::cxx::begin<ox::tags::static_array>()(arr1);
		arrdot dot_next = ox::cxx::nexts<lay>()(dot_begin);
		arrdot dot_end = ox::cxx::end<ox::tags::static_array>()(arr1);
		arrscan(dot_begin,dot_end,arrhandle());

		arrdot dot_at = ox::cxx::at<ox::tags::static_array>()(arr1,1);
		size_t size = ox::cxx::size<ox::tags::static_array>()(arr1);
		ox::cxx::scan<>()(arr1,arrhandle());

		typedef ox::cxx::untiln<arrdot>::type range1;
		typedef ox::cxx::untild<arrdot>::type range2;
		typedef ox::cxx::objects_scaned<range1,range2>::type objs_scaned;

		range1 r1(dot_begin,3);
		range2 r2(dot_begin,dot_end);
		objs_scaned objs(r1,r2);
		ff(ox::mix::data<ox::tags::begin>(r1));

		arrscan.test(objs,arrhandle2());
	}
}





//
//#include "../../src/cxx/dots.h"
//#include "../../src/cxx/setops.h"
//
//namespace setop_test
//{
//	typedef ox::cxx::dots<ox::tags::success,int>::type dot;
//	void test()
//	{
//		dot d1(0), d2((int*)10);
//		dot d3((int*)4), d4((int*)7);
//		dot b;
//		dot e = ox::cxx::intersect<ox::tags::bound,dot>()(d1,d2,d3,d4,b);
//		bool ccc = (int*)(9) > ox::identity<wchar_t>();
//	}
//}
//
//#include <vector>
//
//char c = 'a';
//struct S {S(){s = new char[3]; s[0]=c++; s[1]=0; }char*s;};
int _tmain(int argc, _TCHAR* argv[])
{
//	setop_test::test();
//	std::vector<S> abc;
//	abc.resize(5);
//	abc.insert(abc.begin(),abc.begin(),abc.end());
//
//	for (std::vector<S>::iterator i=abc.begin(); i!=abc.end(); ++i)
//	{
//		char * s = i->s;
//	}
	//vector_test::test();
	array_test::test();
	return 0;
}

