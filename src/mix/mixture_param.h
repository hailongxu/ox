

/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
*/

#include "../ox/nsab.h"
#include "../met/mod.h"



#pragma once



___namespace2_begin(ox,mix)



//// each tx is a reference type


template <typename tt0>
struct tparam1
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	tparam1(t0 p0): d0(p0) {}
	t0 d0;
	static const size_t n = 1;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
};
template <typename tt0,typename tt1>
struct tparam2
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	tparam2(t0 p0,t1 p1): d0(p0),d1(p1) {
		(void)p0;
		(void)p1;
	}
	tparam2(tparam2 const& p) : d0(p.d0),d1(p.d1)
	{}
	t0 d0;t1 d1;
	static const size_t n = 2;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() {
		return d0; }
	template<> t1 v<1>() { return d1; }
};
template <typename tt0,typename tt1,typename tt2>
struct tparam3
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	tparam3(t0 p0,t1 p1,t2 p2): d0(p0),d1(p1),d2(p2) {}
	t0 d0;t1 d1;t2 d2;
	static const size_t n = 3;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
};
template <typename tt0,typename tt1,typename tt2,typename tt3>
struct tparam4
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	tparam4(t0 p0,t1 p1,t2 p2,t3 p3): d0(p0),d1(p1),d2(p2),d3(p3) {}
	t0 d0;t1 d1;t2 d2;t3 d3;
	static const size_t n = 4;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4>
struct tparam5
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	tparam5(t0 p0,t1 p1,t2 p2,t3 p3,t4 p4): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	t0 d0;t1 d1;t2 d2;t3 d3;t4 d4;
	static const size_t n = 5;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5>
struct tparam6
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::add_ref<tt5>::type t5;
	tparam6(t0 p0,t1 p1,t2 p2,t3 p3,t4 p4,t5 p5): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	t0 d0;t1 d1;t2 d2;t3 d3;t4 d4;t5 d5;
	static const size_t n = 6;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<> t5 v<5>() { return d5; }
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6>
struct tparam7
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::add_ref<tt5>::type t5;
	typedef typename ox::met::add_ref<tt6>::type t6;
	tparam7(t0 p0,t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6) {}
	t0 d0;t1 d1;t2 d2;t3 d3;t4 d4;t5 d5;t6 d6;
	static const size_t n = 7;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<> struct retype<6> { typedef t6 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<> t5 v<5>() { return d5; }
	template<> t6 v<6>() { return d6; }
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6,typename tt7>
struct tparam8
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::add_ref<tt5>::type t5;
	typedef typename ox::met::add_ref<tt6>::type t6;
	typedef typename ox::met::add_ref<tt7>::type t7;
	tparam8(t0 p0,t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6),d7(p7) {}
	t0 d0;t1 d1;t2 d2;t3 d3;t4 d4;t5 d5;t6 d6;t7 d7;
	static const size_t n = 8;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<> struct retype<6> { typedef t6 type; };
	template<> struct retype<7> { typedef t7 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<> t5 v<5>() { return d5; }
	template<> t6 v<6>() { return d6; }
	template<> t7 v<7>() { return d7; }
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6,typename tt7,typename tt8>
struct tparam9
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::add_ref<tt5>::type t5;
	typedef typename ox::met::add_ref<tt6>::type t6;
	typedef typename ox::met::add_ref<tt7>::type t7;
	typedef typename ox::met::add_ref<tt8>::type t8;
	tparam9(t0 p0,t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6),d7(p7),d8(p8) {}
	t0 d0;t1 d1;t2 d2;t3 d3;t4 d4;t5 d5;t6 d6;t7 d7;t8 d8;
	static const size_t n = 9;
	template<size_t i> struct retype { typedef void type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<> struct retype<6> { typedef t6 type; };
	template<> struct retype<7> { typedef t7 type; };
	template<> struct retype<8> { typedef t8 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<> t5 v<5>() { return d5; }
	template<> t6 v<6>() { return d6; }
	template<> t7 v<7>() { return d7; }
	template<> t8 v<8>() { return d8; }
};

template <typename px>
struct tparam
{
	tparam(px const& p): o(p) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	tparam(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7,t8& p8) 
		: o(p0,p1,p2,p3,p4,p5,p6,p7,p8) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
	tparam(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7) 
		: o(p0,p1,p2,p3,p4,p5,p6,p7) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
	tparam(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6) 
		: o(p0,p1,p2,p3,p4,p5,p6) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5>
	tparam(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5) 
		: o(p0,p1,p2,p3,p4,p5) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4>
	tparam(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4) 
		: o(p0,p1,p2,p3,p4) {}
	template <typename t0,typename t1,typename t2,typename t3>
	tparam(t0& p0,t1& p1,t2& p2,t3& p3) 
		: o(p0,p1,p2,p3) {}
	template <typename t0,typename t1,typename t2>
	tparam(t0& p0,t1& p1,t2& p2) 
		: o(p0,p1,p2) {}
	template <typename t0,typename t1>
	tparam(t0& p0,t1& p1) 
		: o(p0,p1) {}
	template <typename t0>
	tparam(t0& p0) 
		: o(p0) {}
	px o;
};
template <typename px>
struct tparam <px&>
{
	tparam(px& p): o(p) {}
	px& o;
};

template <typename t>
struct tallparams
{
	tallparams(t& p): o(p) {}
	//template<size_t i> typename retype<i>::type value() { return o.x.v<i>(); }
	t& o;
};
template <typename t>
struct tallparams <t&>
{
	tallparams(t& p): o(p) {}
	//template<size_t i> typename retype<i>::type value() { return o.x.v<i>(); }
	t& o;
};




template <typename t1>
tparam1<t1&> paramx(t1& p1)
{
	return tparam1<t1&>(p1);
}
template <typename t1,typename t2>
tparam2<t1&,t2&> paramx(t1& p1,t2& p2)
{
	return tparam2<t1&,t2&>(p1,p2);
}
template <typename t1,typename t2,typename t3>
tparam3<t1&,t2&,t3&> paramx(t1& p1,t2& p2,t3& p3)
{
	return tparam3<t1&,t2&,t3&>(p1,p2,p3);
}
template <typename t1,typename t2,typename t3,typename t4>
tparam4<t1&,t2&,t3&,t4&> paramx(t1& p1,t2& p2,t3& p3,t4& p4)
{
	return tparam4<t1&,t2&,t3&,t4&>(p1,p2,p3,p4);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5>
tparam5<t1&,t2&,t3&,t4&,t5&> paramx(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5)
{
	return tparam5<t1&,t2&,t3&,t4&,t5&>(p1,p2,p3,p4,p5);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
tparam6<t1&,t2&,t3&,t4&,t5&,t6&> paramx(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6)
{
	return tparam6<t1&,t2&,t3&,t4&,t5&,t6&>(p1,p2,p3,p4,p5,p6);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
tparam7<t1&,t2&,t3&,t4&,t5&,t6&,t7&> paramx(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7)
{
	return tparam7<t1&,t2&,t3&,t4&,t5&,t6&,t7&>(p1,p2,p3,p4,p5,p6,p7);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
tparam8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&> paramx(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7,t8& p8)
{
	return tparam8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&>(p1,p2,p3,p4,p5,p6,p7,p8);
}

/// make tparam
template <typename t1>
tparam<tparam1<t1&>> param(t1& p1)
{
	return tparam<tparam1<t1&>>(p1);
}
template <typename t1,typename t2>
tparam<tparam2<t1&,t2&>> param(t1& p1,t2& p2)
{
	return tparam<tparam2<t1&,t2&>>(p1,p2);
}
template <typename t1,typename t2,typename t3>
tparam<tparam3<t1&,t2&,t3&>> param(t1& p1,t2& p2,t3& p3)
{
	return tparam<tparam3<t1&,t2&,t3&>>(p1,p2,p3);
}
template <typename t1,typename t2,typename t3,typename t4>
tparam<tparam4<t1&,t2&,t3&,t4&>> param(t1& p1,t2& p2,t3& p3,t4& p4)
{
	return tparam<tparam4<t1&,t2&,t3&,t4&>>(p1,p2,p3,p4);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5>
tparam<tparam5<t1&,t2&,t3&,t4&,t5&>> param(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5)
{
	return tparam<tparam5<t1&,t2&,t3&,t4&,t5&>>(p1,p2,p3,p4,p5);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
tparam<tparam6<t1&,t2&,t3&,t4&,t5&,t6&>> param(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6)
{
	return tparam<tparam6<t1&,t2&,t3&,t4&,t5&,t6&>>(p1,p2,p3,p4,p5,p6);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
tparam<tparam7<t1&,t2&,t3&,t4&,t5&,t6&,t7&>> param(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7)
{
	return tparam<tparam7<t1&,t2&,t3&,t4&,t5&,t6&,t7&>>(p1,p2,p3,p4,p5,p6,p7);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
tparam<tparam8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&>> param(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7,t8& p8)
{
	return tparam<tparam8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&>>(p1,p2,p3,p4,p5,p6,p7,p8);
}


template <typename t>
tparam<t&> param_asref(t& p)
{
	return tparam<t&>(p);
}
template <typename t>
tparam<t> param_asval(t& p)
{
	return tparam<t>(p);
}

template <typename t>
tallparams<t> allparams(t& p)
{
	return tallparams<t>(p);
}





___namespace2_end()





