
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"
#include "mixture_feature_def.h"
#include "mixture.h"
#include "tuple_decl.h"


#pragma once


___namespace2_begin(ox,mix)



/**
	tuple, a different type list
*/



template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
struct tuple
{
	___def_8_features(
		list,(t1),(t2),(t3),(t4),(t5),(t6),(t7),(t8)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
struct tuple <t1,t2,t3,t4,t5,t6,t7>
{
	___def_7_features(
		list,(t1),(t2),(t3),(t4),(t5),(t6),(t7)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
struct tuple <t1,t2,t3,t4,t5,t6>
{
	___def_6_features(
		list,(t1),(t2),(t3),(t4),(t5),(t6)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <typename t1,typename t2,typename t3,typename t4,typename t5>
struct tuple <t1,t2,t3,t4,t5>
{
	___def_5_features(
		list,(t1),(t2),(t3),(t4),(t5)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <typename t1,typename t2,typename t3,typename t4>
struct tuple <t1,t2,t3,t4>
{
	___def_4_features(
		list,(t1),(t2),(t3),(t4)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <typename t1,typename t2,typename t3>
struct tuple <t1,t2,t3>
{
	___def_3_features(
		list,(t1),(t2),(t3)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <typename t1,typename t2>
struct tuple <t1,t2>
{
	___def_2_features(
		list,(t1),(t2)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <typename t1>
struct tuple <t1>
{
	___def_1_features(
		list,(t1)
	);
	typedef typename list<0>::type tlist;
	typedef mixture<tlist> type;
};

template <>
struct tuple <>
{
	___def_0_features(
		list
	);
	typedef list<0>::type tlist;
	typedef mixture<tlist> type;
};


//// tup::le


template <>
struct tup<8>
{
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	struct le
	{
		___def_8_features(
			list,(t1),(t2),(t3),(t4),(t5),(t6),(t7),(t8)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};


template <>
struct tup<7>
{
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
	struct le
	{
		___def_7_features(
			list,(t1),(t2),(t3),(t4),(t5),(t6),(t7)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};

template <>
struct tup<6>
{
	template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
	struct le
	{
		___def_6_features(
			list,(t1),(t2),(t3),(t4),(t5),(t6)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};

template <>
struct tup<5>
{
	template <typename t1,typename t2,typename t3,typename t4,typename t5>
	struct le
	{
		___def_5_features(
			list,(t1),(t2),(t3),(t4),(t5)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};

template <>
struct tup<4>
{
	template <typename t1,typename t2,typename t3,typename t4>
	struct le
	{
		___def_4_features(
			list,(t1),(t2),(t3),(t4)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};

template <>
struct tup<3>
{
	template <typename t1,typename t2,typename t3>
	struct le
	{
		___def_3_features(
			list,(t1),(t2),(t3)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};

template <>
struct tup<2>
{
	template <typename t1,typename t2>
	struct le
	{
		___def_2_features(
			list,(t1),(t2)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};

template <>
struct tup<1>
{
	template <typename t1>
	struct le
	{
		___def_1_features(
			list,(t1)
		);
		typedef typename list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};

template <>
struct tup<0>
{
	//template <>
	struct le// <void,void,void,void>
	{
		___def_0_features(
			list
		);
		typedef list<0>::type tlist;
		typedef mixture<tlist> type;
	};
};



___namespace2_end()