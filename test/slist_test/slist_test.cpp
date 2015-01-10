// slist_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"




#include <new.h>
#include "../../src/cxx/slist.h"
#include "../../src/cxx/insert_into_list.h"
#include "../../src/cxx/scan.h"
#include "../../src/cxx/erase.h"
#include "../../src/cxx/resize.h"
#include "../../src/cxx/nexts.h"

namespace slist_print
{
	//ox::cxx::scan_basic<'+',ox::tags::homogeneity> arrscan;

	struct scanhandle
	{
		template <typename dot_tn,typename control>
		void operator()(dot_tn dot,control& ctl)
		{
			printf ("%d\n",dot.value<int>());
		}
	};
	template <typename C>
	void print(C& c)
	{
		ox::cxx::scan<>()(c,scanhandle());
	}
}

#include "../../src/cxx/clear.h"
#include "../../src/cxx/push_back.h"
namespace slist_test
{
	typedef ox::cxx::slist<int> apply;
	typedef apply::type slist;
	typedef slist::metas::layout layout;
	typedef slist::metas::dot dot;
	void test()
	{
		apply::node node;
		slist list;
		
		ox::cxx::insert<ox::tags::slist>()(list,dot(),2,4);
		ox::cxx::insert<ox::tags::slist>()(list,dot(),1);
		ox::cxx::insert<ox::tags::slist>()(list,dot(),2);
		ox::cxx::insert<ox::tags::slist>()(list,dot(),6);
		ox::cxx::insert<ox::tags::slist>()(list,dot(),7);
		ox::cxx::push_back<ox::tags::slist>()(list,20);
		slist_print::print(list);
		dot b = ox::cxx::begin<ox::tags::slist>()(list);
		dot e = ox::cxx::end<ox::tags::slist>()(list);
		ox::cxx::nexts<ox::tags::linelink>()(b);
		slist_print::print(list);
		printf("\n\n");
		//int ab;
		//ox::cxx::allocator_basic_static::ctor<int>(&ab);
		ox::cxx::nexts<layout>()(b);
		ox::cxx::nexts<layout>()(b,2);

		ox::cxx::begin<ox::tags::slist>()(list);
		ox::cxx::end<ox::tags::slist>()(list);
		ox::cxx::back<ox::tags::slist>()(list);
		ox::cxx::erase<ox::tags::slist>()(list,b,2);
		slist_print::print(list);

		ox::cxx::clear<ox::tags::slist>()(list);
		ox::cxx::resize<ox::tags::slist>()(list,3);
		ox::cxx::push_back<ox::tags::slist>()(list,5);
	}
}



#include "../../src/cxx/list.h"
namespace list_test
{
	typedef ox::cxx::list<int> apply;
	typedef apply::type range;
	typedef range::metas::layout layout;
	typedef range::metas::id id;
	typedef range::metas::dot dot;
	void test()
	{
		apply::node node;
		range rng;
		
		ox::cxx::insert<id>()(rng,dot(),2,9);
		ox::cxx::insert<id>()(rng,dot(),1);
		ox::cxx::insert<id>()(rng,dot(),2);
		ox::cxx::insert<id>()(rng,dot(),6);
		ox::cxx::insert<id>()(rng,dot(),7);
		//slist_print::print(rng);
		ox::cxx::erase<id>()(rng,dot(),1);
	}
}




int _tmain(int argc, _TCHAR* argv[])
{
	slist_test::test();
	list_test::test();
	return 0;
}

