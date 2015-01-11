

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


/// a0~a9 ==> any0~any9
//// each tx is a reference type

template <typename tt0>
struct tuple1;
template <typename tt0,typename tt1>
struct tuple2;
template <typename tt0,typename tt1,typename tt2>
struct tuple3;
template <typename tt0,typename tt1,typename tt2,typename tt3>
struct tuple4;
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4>
struct tuple5;
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5>
struct tuple6;
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6>
struct tuple7;
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6,typename tt7>
struct tuple8;
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6,typename tt7,typename tt8>
struct tuple9;

template <typename tt0>
struct tuple1
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::param<tt0>::type i0;
	tuple1() {}
	tuple1(i0 p0): d0(p0) {}
	template<typename a1>
	tuple1(i0 p0,a1): d0(p0) {}
	template<typename a1,typename a2>
	tuple1(i0 p0,a1,a2): d0(p0) {}
	template<typename a1,typename a2,typename a3>
	tuple1(i0 p0,a1,a2,a3): d0(p0) {}
	template<typename a1,typename a2,typename a3,typename a4>
	tuple1(i0 p0,a1,a2,a3,a4): d0(p0) {}
	template<typename a1,typename a2,typename a3,typename a4,typename a5>
	tuple1(i0 p0,a1,a2,a3,a4,a5): d0(p0) {}
	template<typename a1,typename a2,typename a3,typename a4,typename a5,typename a6>
	tuple1(i0 p0,a1,a2,a3,a4,a5,a6): d0(p0) {}
	template<typename a1,typename a2,typename a3,typename a4,typename a5,typename a6,typename a7>
	tuple1(i0 p0,a1,a2,a3,a4,a5,a6,a7): d0(p0) {}
	template<typename a1,typename a2,typename a3,typename a4,typename a5,typename a6,typename a7,typename a8>
	tuple1(i0 p0,a1,a2,a3,a4,a5,a6,a7,a8): d0(p0) {}
	tt0 d0;
	static const size_t n = 1;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<typename t,bool> struct push_front { typedef tuple1 type; };
	template<typename t,bool> struct push_back { typedef tuple1 type; };
	template<typename t> struct push_front<t,true> { typedef tuple2<t,tt0> type; };
	template<typename t> struct push_back<t,true> { typedef tuple2<tt0,t> type; };
	template<bool> struct pop_front { typedef tuple1 type;};
	template<> struct pop_front<true> { typedef void type;};
};
template <typename tt0,typename tt1>
struct tuple2
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	tuple2(tuple2 const& p) : d0(p.d0),d1(p.d1)	{}
	tuple2() {}
	tuple2(i0 p0): d0(p0) {}
	tuple2(i0 p0,i1 p1): d0(p0),d1(p1) {}
	template<typename a2>
	tuple2(i0 p0,i1 p1,a2): d0(p0),d1(p1) {}
	template<typename a2,typename a3>
	tuple2(i0 p0,i1 p1,a2,a3): d0(p0),d1(p1) {}
	template<typename a2,typename a3,typename a4>
	tuple2(i0 p0,i1 p1,a2,a3,a4): d0(p0),d1(p1) {}
	template<typename a2,typename a3,typename a4,typename a5>
	tuple2(i0 p0,i1 p1,a2,a3,a4,a5): d0(p0),d1(p1) {}
	template<typename a2,typename a3,typename a4,typename a5,typename a6>
	tuple2(i0 p0,i1 p1,a2,a3,a4,a5,a6): d0(p0),d1(p1) {}
	template<typename a2,typename a3,typename a4,typename a5,typename a6,typename a7>
	tuple2(i0 p0,i1 p1,a2,a3,a4,a5,a6,a7): d0(p0),d1(p1) {}
	template<typename a2,typename a3,typename a4,typename a5,typename a6,typename a7,typename a8>
	tuple2(i0 p0,i1 p1,a2,a3,a4,a5,a6,a7,a8): d0(p0),d1(p1) {}
	tt0 d0;tt1 d1;
	static const size_t n = 2;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<typename t,bool> struct push_front { typedef tuple1 type; };
	template<typename t,bool> struct push_back { typedef tuple1 type; };
	template<typename t> struct push_front<t,true> { typedef tuple3<t,tt0,tt1> type; };
	template<typename t> struct push_back<t,true> { typedef tuple3<tt0,tt1,t> type; };
	template<bool> struct pop_front { typedef tuple1 type;};
	template<> struct pop_front<true> { typedef tuple1<tt1> type;};
};
template <typename tt0,typename tt1,typename tt2>
struct tuple3
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	typedef typename ox::met::param<tt2>::type i2;
	tuple3() {}
	tuple3(i0 p0): d0(p0) {}
	tuple3(i0 p0,i1 p1): d0(p0),d1(p1) {}
	tuple3(i0 p0,i1 p1,i2 p2): d0(p0),d1(p1),d2(p2) {}
	template<typename a3>
	tuple3(i0 p0,i1 p1,i2 p2,a3): d0(p0),d1(p1),d2(p2) {}
	template<typename a3,typename a4>
	tuple3(i0 p0,i1 p1,i2 p2,a3,a4): d0(p0),d1(p1),d2(p2) {}
	template<typename a3,typename a4,typename a5>
	tuple3(i0 p0,i1 p1,i2 p2,a3,a4,a5): d0(p0),d1(p1),d2(p2) {}
	template<typename a3,typename a4,typename a5,typename a6>
	tuple3(i0 p0,i1 p1,i2 p2,a3,a4,a5,a6): d0(p0),d1(p1),d2(p2) {}
	template<typename a3,typename a4,typename a5,typename a6,typename a7>
	tuple3(i0 p0,i1 p1,i2 p2,a3,a4,a5,a6,a7): d0(p0),d1(p1),d2(p2) {}
	template<typename a3,typename a4,typename a5,typename a6,typename a7,typename a8>
	tuple3(i0 p0,i1 p1,i2 p2,a3,a4,a5,a6,a7,a8): d0(p0),d1(p1),d2(p2) {}
	tt0 d0;tt1 d1;tt2 d2;
	static const size_t n = 3;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct paramt<2> { typedef i2 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<> struct typeat<2> { typedef tt2 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<typename t,bool> struct push_front { typedef tuple3 type; };
	template<typename t,bool> struct push_back { typedef tuple3 type; };
	template<typename t> struct push_front<t,true> { typedef tuple4<t,tt0,tt1,tt2> type; };
	template<typename t> struct push_back<t,true> { typedef tuple4<tt0,tt1,tt2,t> type; };
	template<bool> struct pop_front { typedef tuple3 type;};
	template<> struct pop_front<true> { typedef tuple2<tt1,tt2> type;};
};
template <typename tt0,typename tt1,typename tt2,typename tt3>
struct tuple4
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	typedef typename ox::met::param<tt2>::type i2;
	typedef typename ox::met::param<tt3>::type i3;
	tuple4() {}
	tuple4(i0 p0): d0(p0) {}
	tuple4(i0 p0,i1 p1): d0(p0),d1(p1) {}
	tuple4(i0 p0,i1 p1,i2 p2): d0(p0),d1(p1),d2(p2) {}
	tuple4(i0 p0,i1 p1,i2 p2,i3 p3): d0(p0),d1(p1),d2(p2),d3(p3) {}
	template<typename a4>
	tuple4(i0 p0,i1 p1,i2 p2,i3 p3,a4): d0(p0),d1(p1),d2(p2),d3(p3) {}
	template<typename a4,typename a5>
	tuple4(i0 p0,i1 p1,i2 p2,i3 p3,a4,a5): d0(p0),d1(p1),d2(p2),d3(p3) {}
	template<typename a4,typename a5,typename a6>
	tuple4(i0 p0,i1 p1,i2 p2,i3 p3,a4,a5,a6): d0(p0),d1(p1),d2(p2),d3(p3) {}
	template<typename a4,typename a5,typename a6,typename a7>
	tuple4(i0 p0,i1 p1,i2 p2,i3 p3,a4,a5,a6,a7): d0(p0),d1(p1),d2(p2),d3(p3) {}
	template<typename a4,typename a5,typename a6,typename a7,typename a8>
	tuple4(i0 p0,i1 p1,i2 p2,i3 p3,a4,a5,a6,a7,a8): d0(p0),d1(p1),d2(p2),d3(p3) {}
	tt0 d0;tt1 d1;tt2 d2;tt3 d3;
	static const size_t n = 4;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct paramt<2> { typedef i2 type; };
	template<> struct paramt<3> { typedef i3 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<> struct typeat<2> { typedef tt2 type; };
	template<> struct typeat<3> { typedef tt3 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<typename t,bool> struct push_front { typedef tuple4 type; };
	template<typename t,bool> struct push_back { typedef tuple4 type; };
	template<typename t> struct push_front<t,true> { typedef tuple5<t,tt0,tt1,tt2,tt3> type; };
	template<typename t> struct push_back<t,true> { typedef tuple5<tt0,tt1,tt2,tt3,t> type; };
	template<bool> struct pop_front { typedef tuple4 type;};
	template<> struct pop_front<true> { typedef tuple3<tt1,tt2,tt3> type;};
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4>
struct tuple5
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	typedef typename ox::met::param<tt2>::type i2;
	typedef typename ox::met::param<tt3>::type i3;
	typedef typename ox::met::param<tt4>::type i4;
	tuple5() {}
	tuple5(i0 p0): d0(p0) {}
	tuple5(i0 p0,i1 p1): d0(p0),d1(p1) {}
	tuple5(i0 p0,i1 p1,i2 p2): d0(p0),d1(p1),d2(p2) {}
	tuple5(i0 p0,i1 p1,i2 p2,i3 p3): d0(p0),d1(p1),d2(p2),d3(p3) {}
	tuple5(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	template<typename a5>
	tuple5(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,a5): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	template<typename a5,typename a6>
	tuple5(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,a5,a6): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	template<typename a5,typename a6,typename a7>
	tuple5(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,a5,a6,a7): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	template<typename a5,typename a6,typename a7,typename a8>
	tuple5(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,a5,a6,a7,a8): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	tt0 d0;tt1 d1;tt2 d2;tt3 d3;tt4 d4;
	static const size_t n = 5;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct paramt<2> { typedef i2 type; };
	template<> struct paramt<3> { typedef i3 type; };
	template<> struct paramt<4> { typedef i4 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<> struct typeat<2> { typedef tt2 type; };
	template<> struct typeat<3> { typedef tt3 type; };
	template<> struct typeat<4> { typedef tt4 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<typename t,bool> struct push_front { typedef tuple5 type; };
	template<typename t,bool> struct push_back { typedef tuple5 type; };
	template<typename t> struct push_front<t,true> { typedef tuple6<t,tt0,tt1,tt2,tt3,tt4> type; };
	template<typename t> struct push_back<t,true> { typedef tuple6<tt0,tt1,tt2,tt3,tt4,t> type; };
	template<bool> struct pop_front { typedef tuple5 type;};
	template<> struct pop_front<true> { typedef tuple4<tt1,tt2,tt3,tt4> type;};
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5>
struct tuple6
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::add_ref<tt5>::type t5;
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	typedef typename ox::met::param<tt2>::type i2;
	typedef typename ox::met::param<tt3>::type i3;
	typedef typename ox::met::param<tt4>::type i4;
	typedef typename ox::met::param<tt5>::type i5;
	tuple6() {}
	tuple6(i0 p0): d0(p0) {}
	tuple6(i0 p0,i1 p1): d0(p0),d1(p1) {}
	tuple6(i0 p0,i1 p1,i2 p2): d0(p0),d1(p1),d2(p2) {}
	tuple6(i0 p0,i1 p1,i2 p2,i3 p3): d0(p0),d1(p1),d2(p2),d3(p3) {}
	tuple6(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	tuple6(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	template<typename a6>
	tuple6(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,a6): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	template<typename a6,typename a7>
	tuple6(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,a6,a7): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	template<typename a6,typename a7,typename a8>
	tuple6(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,a6,a7,a8): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	tt0 d0;tt1 d1;tt2 d2;tt3 d3;tt4 d4;tt5 d5;
	static const size_t n = 6;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct paramt<2> { typedef i2 type; };
	template<> struct paramt<3> { typedef i3 type; };
	template<> struct paramt<4> { typedef i4 type; };
	template<> struct paramt<5> { typedef i5 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<> struct typeat<2> { typedef tt2 type; };
	template<> struct typeat<3> { typedef tt3 type; };
	template<> struct typeat<4> { typedef tt4 type; };
	template<> struct typeat<5> { typedef tt5 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<> t5 v<5>() { return d5; }
	template<typename t,bool> struct push_front { typedef tuple6 type; };
	template<typename t,bool> struct push_back { typedef tuple6 type; };
	template<typename t> struct push_front<t,true> { typedef tuple7<t,tt0,tt1,tt2,tt3,tt4,tt5> type; };
	template<typename t> struct push_back<t,true> { typedef tuple7<tt0,tt1,tt2,tt3,tt4,tt5,t> type; };
	template<bool> struct pop_front { typedef tuple6 type;};
	template<> struct pop_front<true> { typedef tuple5<tt1,tt2,tt3,tt4,tt5> type;};
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6>
struct tuple7
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::add_ref<tt5>::type t5;
	typedef typename ox::met::add_ref<tt6>::type t6;
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	typedef typename ox::met::param<tt2>::type i2;
	typedef typename ox::met::param<tt3>::type i3;
	typedef typename ox::met::param<tt4>::type i4;
	typedef typename ox::met::param<tt5>::type i5;
	typedef typename ox::met::param<tt6>::type i6;
	tuple7() {}
	tuple7(i0 p0): d0(p0) {}
	tuple7(i0 p0,i1 p1): d0(p0),d1(p1) {}
	tuple7(i0 p0,i1 p1,i2 p2): d0(p0),d1(p1),d2(p2) {}
	tuple7(i0 p0,i1 p1,i2 p2,i3 p3): d0(p0),d1(p1),d2(p2),d3(p3) {}
	tuple7(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	tuple7(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	tuple7(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6) {}
	template<typename a7>
	tuple7(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6,a7): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6) {}
	template<typename a7,typename a8>
	tuple7(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6,a7,a8): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6) {}
	tt0 d0;tt1 d1;tt2 d2;tt3 d3;tt4 d4;tt5 d5;tt6 d6;
	static const size_t n = 7;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct paramt<2> { typedef i2 type; };
	template<> struct paramt<3> { typedef i3 type; };
	template<> struct paramt<4> { typedef i4 type; };
	template<> struct paramt<5> { typedef i5 type; };
	template<> struct paramt<6> { typedef i6 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<> struct retype<6> { typedef t6 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<> struct typeat<2> { typedef tt2 type; };
	template<> struct typeat<3> { typedef tt3 type; };
	template<> struct typeat<4> { typedef tt4 type; };
	template<> struct typeat<5> { typedef tt5 type; };
	template<> struct typeat<6> { typedef tt6 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<> t5 v<5>() { return d5; }
	template<> t6 v<6>() { return d6; }
	template<typename t,bool> struct push_front { typedef tuple7 type; };
	template<typename t,bool> struct push_back { typedef tuple7 type; };
	template<typename t> struct push_front<t,true> { typedef tuple8<t,tt0,tt1,tt2,tt3,tt4,tt5,tt6> type; };
	template<typename t> struct push_back<t,true> { typedef tuple8<tt0,tt1,tt2,tt3,tt4,tt5,tt6,t> type; };
	template<bool> struct pop_front { typedef tuple7 type;};
	template<> struct pop_front<true> { typedef tuple6<tt1,tt2,tt3,tt4,tt5,tt6> type;};
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6,typename tt7>
struct tuple8
{
	typedef typename ox::met::add_ref<tt0>::type t0;
	typedef typename ox::met::add_ref<tt1>::type t1;
	typedef typename ox::met::add_ref<tt2>::type t2;
	typedef typename ox::met::add_ref<tt3>::type t3;
	typedef typename ox::met::add_ref<tt4>::type t4;
	typedef typename ox::met::add_ref<tt5>::type t5;
	typedef typename ox::met::add_ref<tt6>::type t6;
	typedef typename ox::met::add_ref<tt7>::type t7;
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	typedef typename ox::met::param<tt2>::type i2;
	typedef typename ox::met::param<tt3>::type i3;
	typedef typename ox::met::param<tt4>::type i4;
	typedef typename ox::met::param<tt5>::type i5;
	typedef typename ox::met::param<tt6>::type i6;
	typedef typename ox::met::param<tt7>::type i7;
	tuple8() {}
	tuple8(i0 p0): d0(p0) {}
	tuple8(i0 p0,i1 p1): d0(p0),d1(p1) {}
	tuple8(i0 p0,i1 p1,i2 p2): d0(p0),d1(p1),d2(p2) {}
	tuple8(i0 p0,i1 p1,i2 p2,i3 p3): d0(p0),d1(p1),d2(p2),d3(p3) {}
	tuple8(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	tuple8(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	tuple8(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6) {}
	tuple8(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6,i7 p7): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6),d7(p7) {}
	template<typename a8>
	tuple8(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6,i7 p7,a8): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6),d7(p7) {}

	tt0 d0;tt1 d1;tt2 d2;tt3 d3;tt4 d4;tt5 d5;tt6 d6;tt7 d7;
	static const size_t n = 8;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct paramt<2> { typedef i2 type; };
	template<> struct paramt<3> { typedef i3 type; };
	template<> struct paramt<4> { typedef i4 type; };
	template<> struct paramt<5> { typedef i5 type; };
	template<> struct paramt<6> { typedef i6 type; };
	template<> struct paramt<7> { typedef i7 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<> struct retype<6> { typedef t6 type; };
	template<> struct retype<7> { typedef t7 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<> struct typeat<2> { typedef tt2 type; };
	template<> struct typeat<3> { typedef tt3 type; };
	template<> struct typeat<4> { typedef tt4 type; };
	template<> struct typeat<5> { typedef tt5 type; };
	template<> struct typeat<6> { typedef tt6 type; };
	template<> struct typeat<7> { typedef tt7 type; };
	template<size_t i> typename retype<i>::type v() {}
	template<> t0 v<0>() { return d0; }
	template<> t1 v<1>() { return d1; }
	template<> t2 v<2>() { return d2; }
	template<> t3 v<3>() { return d3; }
	template<> t4 v<4>() { return d4; }
	template<> t5 v<5>() { return d5; }
	template<> t6 v<6>() { return d6; }
	template<> t7 v<7>() { return d7; }
	template<typename t,bool> struct push_front { typedef tuple8 type; };
	template<typename t,bool> struct push_back { typedef tuple8 type; };
	template<typename t> struct push_front<t,true> { typedef tuple9<t,tt0,tt1,tt2,tt3,tt4,tt5,tt6,tt7> type; };
	template<typename t> struct push_back<t,true> { typedef tuple9<tt0,tt1,tt2,tt3,tt4,tt5,tt6,tt7,t> type; };
	template<bool> struct pop_front { typedef tuple8 type;};
	template<> struct pop_front<true> { typedef tuple7<tt1,tt2,tt3,tt4,tt5,tt6,tt7> type;};
};
template <typename tt0,typename tt1,typename tt2,typename tt3,typename tt4,typename tt5,typename tt6,typename tt7,typename tt8>
struct tuple9
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
	typedef typename ox::met::param<tt0>::type i0;
	typedef typename ox::met::param<tt1>::type i1;
	typedef typename ox::met::param<tt2>::type i2;
	typedef typename ox::met::param<tt3>::type i3;
	typedef typename ox::met::param<tt4>::type i4;
	typedef typename ox::met::param<tt5>::type i5;
	typedef typename ox::met::param<tt6>::type i6;
	typedef typename ox::met::param<tt7>::type i7;
	typedef typename ox::met::param<tt8>::type i8;
	tuple9() {}
	tuple9(i0 p0): d0(p0) {}
	tuple9(i0 p0,i1 p1): d0(p0),d1(p1) {}
	tuple9(i0 p0,i1 p1,i2 p2): d0(p0),d1(p1),d2(p2) {}
	tuple9(i0 p0,i1 p1,i2 p2,i3 p3): d0(p0),d1(p1),d2(p2),d3(p3) {}
	tuple9(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4) {}
	tuple9(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5) {}
	tuple9(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6) {}
	tuple9(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6,i7 p7): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6),d7(p7) {}
	tuple9(i0 p0,i1 p1,i2 p2,i3 p3,i4 p4,i5 p5,i6 p6,i7 p7,i8 p8): d0(p0),d1(p1),d2(p2),d3(p3),d4(p4),d5(p5),d6(p6),d7(p7),d8(p8) {}
	tt0 d0;tt1 d1;tt2 d2;tt3 d3;tt4 d4;tt5 d5;tt6 d6;tt7 d7;tt8 d8;
	static const size_t n = 9;
	template<size_t i> struct paramt { typedef void type; };
	template<size_t i> struct retype { typedef void type; };
	template<size_t i> struct typeat { typedef void type; };
	template<> struct paramt<0> { typedef i0 type; };
	template<> struct paramt<1> { typedef i1 type; };
	template<> struct paramt<2> { typedef i2 type; };
	template<> struct paramt<3> { typedef i3 type; };
	template<> struct paramt<4> { typedef i4 type; };
	template<> struct paramt<5> { typedef i5 type; };
	template<> struct paramt<6> { typedef i6 type; };
	template<> struct paramt<7> { typedef i7 type; };
	template<> struct paramt<8> { typedef i8 type; };
	template<> struct retype<0> { typedef t0 type; };
	template<> struct retype<1> { typedef t1 type; };
	template<> struct retype<2> { typedef t2 type; };
	template<> struct retype<3> { typedef t3 type; };
	template<> struct retype<4> { typedef t4 type; };
	template<> struct retype<5> { typedef t5 type; };
	template<> struct retype<6> { typedef t6 type; };
	template<> struct retype<7> { typedef t7 type; };
	template<> struct retype<8> { typedef t8 type; };
	template<> struct typeat<0> { typedef tt0 type; };
	template<> struct typeat<1> { typedef tt1 type; };
	template<> struct typeat<2> { typedef tt2 type; };
	template<> struct typeat<3> { typedef tt3 type; };
	template<> struct typeat<4> { typedef tt4 type; };
	template<> struct typeat<5> { typedef tt5 type; };
	template<> struct typeat<6> { typedef tt6 type; };
	template<> struct typeat<7> { typedef tt7 type; };
	template<> struct typeat<8> { typedef tt8 type; };
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
	template<typename t,bool> struct push_front { typedef tuple9 type; };
	template<typename t,bool> struct push_back { typedef tuple9 type; };
	template<typename t> struct push_front<t,true> {};
	template<typename t> struct push_back<t,true> {};
	template<bool> struct pop_front { typedef tuple9 type;};
	template<> struct pop_front<true> { typedef tuple8<tt1,tt2,tt3,tt4,tt5,tt6,tt7,tt8> type;};
};

template <typename tx>
struct tuplet : tx
{
	tuplet(tx const& p): tx(p) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
	tuplet(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7,t8& p8) 
		: tx(p0,p1,p2,p3,p4,p5,p6,p7,p8) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
	tuplet(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7) 
		: tx(p0,p1,p2,p3,p4,p5,p6,p7) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
	tuplet(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6) 
		: tx(p0,p1,p2,p3,p4,p5,p6) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4,typename t5>
	tuplet(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4,t5& p5) 
		: tx(p0,p1,p2,p3,p4,p5) {}
	template <typename t0,typename t1,typename t2,typename t3,typename t4>
	tuplet(t0& p0,t1& p1,t2& p2,t3& p3,t4& p4) 
		: tx(p0,p1,p2,p3,p4) {}
	template <typename t0,typename t1,typename t2,typename t3>
	tuplet(t0& p0,t1& p1,t2& p2,t3& p3) 
		: tx(p0,p1,p2,p3) {}
	template <typename t0,typename t1,typename t2>
	tuplet(t0& p0,t1& p1,t2& p2) 
		: tx(p0,p1,p2) {}
	template <typename t0,typename t1>
	tuplet(t0& p0,t1& p1) 
		: tx(p0,p1) {}
	template <typename t0>
	tuplet(t0& p0) 
		: tx(p0) {}
	//tx& value() {return *this;}
};
template <typename tx>
struct tuplet <tx&>
{
	tuplet(tx& p): o(p) {}
	tx& o;
};

template <typename t>
struct talltuplet
{
	talltuplet(t& p): o(p) {}
	//template<size_t i> typename retype<i>::type value() { return o.x.v<i>(); }
	t& o;
};
template <typename t>
struct talltuplet <t&>
{
	talltuplet(t& p): o(p) {}
	//template<size_t i> typename retype<i>::type value() { return o.x.v<i>(); }
	t& o;
};




template <typename t1>
tuple1<t1&> tupleiv(t1& p1)
{
	return tuple1<t1&>(p1);
}
template <typename t1,typename t2>
tuple2<t1&,t2&> tupleiv(t1& p1,t2& p2)
{
	return tuple2<t1&,t2&>(p1,p2);
}
template <typename t1,typename t2,typename t3>
tuple3<t1&,t2&,t3&> tupleiv(t1& p1,t2& p2,t3& p3)
{
	return tuple3<t1&,t2&,t3&>(p1,p2,p3);
}
template <typename t1,typename t2,typename t3,typename t4>
tuple4<t1&,t2&,t3&,t4&> tupleiv(t1& p1,t2& p2,t3& p3,t4& p4)
{
	return tuple4<t1&,t2&,t3&,t4&>(p1,p2,p3,p4);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5>
tuple5<t1&,t2&,t3&,t4&,t5&> tupleiv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5)
{
	return tuple5<t1&,t2&,t3&,t4&,t5&>(p1,p2,p3,p4,p5);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
tuple6<t1&,t2&,t3&,t4&,t5&,t6&> tupleiv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6)
{
	return tuple6<t1&,t2&,t3&,t4&,t5&,t6&>(p1,p2,p3,p4,p5,p6);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
tuple7<t1&,t2&,t3&,t4&,t5&,t6&,t7&> tupleiv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7)
{
	return tuple7<t1&,t2&,t3&,t4&,t5&,t6&,t7&>(p1,p2,p3,p4,p5,p6,p7);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
tuple8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&> tupleiv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7,t8& p8)
{
	return tuple8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&>(p1,p2,p3,p4,p5,p6,p7,p8);
}

/// make tuplet
template <typename t1>
tuplet<tuple1<t1&>> tupletv(t1& p1)
{
	return tuplet<tuple1<t1&>>(p1);
}
template <typename t1,typename t2>
tuplet<tuple2<t1&,t2&>> tupletv(t1& p1,t2& p2)
{
	return tuplet<tuple2<t1&,t2&>>(p1,p2);
}
template <typename t1,typename t2,typename t3>
tuplet<tuple3<t1&,t2&,t3&>> tupletv(t1& p1,t2& p2,t3& p3)
{
	return tuplet<tuple3<t1&,t2&,t3&>>(p1,p2,p3);
}
template <typename t1,typename t2,typename t3,typename t4>
tuplet<tuple4<t1&,t2&,t3&,t4&>> tupletv(t1& p1,t2& p2,t3& p3,t4& p4)
{
	return tuplet<tuple4<t1&,t2&,t3&,t4&>>(p1,p2,p3,p4);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5>
tuplet<tuple5<t1&,t2&,t3&,t4&,t5&>> tupletv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5)
{
	return tuplet<tuple5<t1&,t2&,t3&,t4&,t5&>>(p1,p2,p3,p4,p5);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6>
tuplet<tuple6<t1&,t2&,t3&,t4&,t5&,t6&>> tupletv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6)
{
	return tuplet<tuple6<t1&,t2&,t3&,t4&,t5&,t6&>>(p1,p2,p3,p4,p5,p6);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7>
tuplet<tuple7<t1&,t2&,t3&,t4&,t5&,t6&,t7&>> tupletv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7)
{
	return tuplet<tuple7<t1&,t2&,t3&,t4&,t5&,t6&,t7&>>(p1,p2,p3,p4,p5,p6,p7);
}
template <typename t1,typename t2,typename t3,typename t4,typename t5,typename t6,typename t7,typename t8>
tuplet<tuple8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&>> tupletv(t1& p1,t2& p2,t3& p3,t4& p4,t5& p5,t6& p6,t7& p7,t8& p8)
{
	return tuplet<tuple8<t1&,t2&,t3&,t4&,t5&,t6&,t7&,t8&>>(p1,p2,p3,p4,p5,p6,p7,p8);
}


template <typename t>
tuplet<t&> tupletv_asref(t& p)
{
	return tuplet<t&>(p);
}
template <typename t>
tuplet<t> tupletv_asval(t& p)
{
	return tuplet<t>(p);
}

template <typename t>
talltuplet<t> alltupletvs(t& p)
{
	return talltuplet<t>(p);
}



template<typename tup,size_t n,bool b>
struct erase_all_when_true
{
	typedef typename tup::pop_front<b>::type next_tuple;
	typedef typename erase_all_when_true<next_tuple,(n>>1),n&1>::type type;
};
template<typename tup>
struct erase_all_when_true <tup,0,0>
{
	typedef tup type;
};
template<size_t n,bool b>
struct erase_all_when_true <void,n,b>
{
	typedef void type;
};
template<>
struct erase_all_when_true <void,0,0>
{
	typedef void type;
};

template <typename tup,size_t n>
struct erase_all
{
	typedef typename erase_all_when_true<tup,(n>>1),n&1>::type type;
};

template <typename tuple,typename t,bool b>
struct tuple_push_front
{
	typedef typename tuple::push_front<t,b>::type type;
};
template <typename t,bool b>
struct tuple_push_front<void,t,b>
{
	typedef typename ox::met::iff<b,tuple1<t>,void>::type type;
};


___namespace2_end()





