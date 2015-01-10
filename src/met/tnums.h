


#include "../ox/nsab.h"


#pragma once



___namespace1_begin(ox)


template<size_t n> struct tnums;

struct tnum0;
template <long>
struct tnum1;
template <long,long>
struct tnum2;
template <long,long,long>
struct tnum3;
template <long,long,long,long>
struct tnum4;
template <long,long,long,long,long>
struct tnum5;
template <long,long,long,long,long,long>
struct tnum6;
template <long,long,long,long,long,long,long>
struct tnum7;
template <long,long,long,long,long,long,long,long>
struct tnum8;
template <long,long,long,long,long,long,long,long,long>
struct tnum9;
template <long,long,long,long,long,long,long,long,long,long>
struct tnum10;
template <long,long,long,long,long,long,long,long,long,long,long>
struct tnum11;
template <long,long,long,long,long,long,long,long,long,long,long,long>
struct tnum12;
template <long,long,long,long,long,long,long,long,long,long,long,long,long>
struct tnum13;
template <long,long,long,long,long,long,long,long,long,long,long,long,long,long>
struct tnum14;
template <long,long,long,long,long,long,long,long,long,long,long,long,long,long,long>
struct tnum15;
template <long,long,long,long,long,long,long,long,long,long,long,long,long,long,long,long>
struct tnum16;



struct tnum0
{
	typedef long type;
	static size_t const size = 0;
	struct top;
	template <size_t> struct get;
	template <size_t v> struct repush { typedef tnum0 type; };
	template <size_t v> struct push { typedef tnum1<v> type; };
	struct pop { typedef void type; };
};

template <long v0>
struct tnum1
{
	typedef tnum0 base;
	typedef long type;
	static size_t const size = 1;
	struct top { static long const value = v0; };

	template <size_t> struct get;
	template <> struct get<0> { static size_t const value = v0; };
	template <size_t v> struct repush { typedef tnum1<v> type; };
	template <size_t v> struct push { typedef tnum2<v0,v> type; };
	struct pop { typedef tnum0 type; };
};

template <long v0,long v1>
struct tnum2
{
	typedef tnum1<v0> base;
	typedef long type;
	static size_t const size = 2;
	struct top { static long const value = v1; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<1> { static size_t const value = v1; };
	template <size_t v> struct repush { typedef tnum2<v1,v> type; };
	template <size_t v> struct push { typedef tnum3<v0,v1,v> type; };
	struct pop { typedef tnum1<v1> type; };
};

template <long v0,long v1,long v2>
struct tnum3
{
	typedef tnum2<v0,v1> base;
	typedef long type;
	static size_t const size = 3;
	struct top { static long const value = v2; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<2> { static size_t const value = v2; };
	template <size_t v> struct repush { typedef tnum3<v0,v1,v> type; };
	template <size_t v> struct push { typedef tnum4<v0,v1,v2,v> type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3>
struct tnum4
{
	typedef tnum3<v0,v1,v2> base;
	typedef long type;
	static size_t const size = 4;
	struct top { static long const value = v3; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<3> { static size_t const value = v3; };
	template <size_t v> struct repush { typedef tnum4<v0,v1,v2,v> type; };
	template <size_t v> struct push { typedef tnum5<v0,v1,v2,v3,v> type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4>
struct tnum5
{
	typedef tnum4<v0,v1,v2,v3> base;
	typedef long type;
	static size_t const size = 5;
	struct top { static long const value = v4; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<4> { static size_t const value = v4; };
	template <size_t v> struct repush { typedef tnum5<v0,v1,v2,v3,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5>
struct tnum6
{
	typedef tnum5<v0,v1,v2,v3,v4> base;
	typedef long type;
	static size_t const size = 6;
	struct top { static long const value = v5; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<5> { static size_t const value = v5; };
	template <size_t v> struct repush { typedef tnum6<v0,v1,v2,v3,v4,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6>
struct tnum7
{
	typedef tnum6<v0,v1,v2,v3,v4,v5> base;
	typedef long type;
	static size_t const size = 7;
	struct top { static long const value = v6; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<6> { static size_t const value = v6; };
	template <size_t v> struct repush { typedef tnum7<v0,v1,v2,v3,v4,v5,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7>
struct tnum8
{
	typedef tnum7<v0,v1,v2,v3,v4,v5,v6> base;
	typedef long type;
	static size_t const size = 8;
	struct top { static long const value = v7; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<7> { static size_t const value = v7; };
	template <size_t v> struct repush { typedef tnum8<v0,v1,v2,v3,v4,v5,v6,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8>
struct tnum9
{
	typedef tnum8<v0,v1,v2,v3,v4,v5,v6,v7> base;
	typedef long type;
	static size_t const size = 9;
	struct top { static long const value = v8; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<8> { static size_t const value = v8; };
	template <size_t v> struct repush { typedef tnum9<v0,v1,v2,v3,v4,v5,v6,v7,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8,long v9>
struct tnum10
{
	typedef tnum9<v0,v1,v2,v3,v4,v5,v6,v7,v8> base;
	typedef long type;
	static size_t const size = 10;
	struct top { static long const value = v9; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<9> { static size_t const value = v9; };
	template <size_t v> struct repush { typedef tnum10<v0,v1,v2,v3,v4,v5,v6,v7,v8,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8,long v9,long v10>
struct tnum11
{
	typedef tnum10<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9> base;
	typedef long type;
	static size_t const size = 11;
	struct top { static long const value = v10; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<10> { static size_t const value = v10; };
	template <size_t v> struct repush { typedef tnum11<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8,long v9,long v10,long v11>
struct tnum12
{
	typedef tnum11<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10> base;
	typedef long type;
	static size_t const size = 12;
	struct top { static long const value = v11; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<11> { static size_t const value = v11; };
	template <size_t v> struct repush { typedef tnum12<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8,long v9,long v10,long v11,long v12>
struct tnum13
{
	typedef tnum12<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11> base;
	typedef long type;
	static size_t const size = 13;
	struct top { static long const value = v12; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<12> { static size_t const value = v12; };
	template <size_t v> struct repush { typedef tnum13<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8,long v9,long v10,long v11,long v12,long v13>
struct tnum14
{
	typedef tnum13<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12> base;
	typedef long type;
	static size_t const size = 14;
	struct top { static long const value = v13; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<13> { static size_t const value = v13; };
	template <size_t v> struct repush { typedef tnum14<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8,long v9,long v10,long v11,long v12,long v13,long v14>
struct tnum15
{
	typedef tnum14<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13> base;
	typedef long type;
	static size_t const size = 15;
	struct top { static long const value = v14; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<14> { static size_t const value = v14; };
	template <size_t v> struct repush { typedef tnum15<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};

template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7,long v8,long v9,long v10,long v11,long v12,long v13,long v14,long v15>
struct tnum16
{
	typedef tnum15<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14> base;
	typedef long type;
	static size_t const size = 16;
	struct top { static long const value = v15; };

	template <size_t i> struct get { static size_t const value = base::get<i>::value; };
	template <> struct get<15> { static size_t const value = v15; };
	template <size_t v> struct repush { typedef tnum16<v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v> type; };
	template <size_t v> struct push { typedef void type; };
	struct pop { typedef base type; };
};



template<size_t n> struct tnums;

template<> struct tnums <1>
{
	template <long v0>
	struct apply
	{
		typedef tnum1<v0> type;
	};
};
template<> struct tnums <2>
{
	template <long v0,long v1>
	struct apply
	{
		typedef tnum2<v0,v1> type;
	};
};
template<> struct tnums <3>
{
	template <long v0,long v1,long v2>
	struct apply
	{
		typedef tnum3<v0,v1,v2> type;
	};
};
template<> struct tnums <4>
{
	template <long v0,long v1,long v2,long v3,long v4>
	struct apply
	{
		typedef tnum4<v0,v1,v2,v3> type;
	};
};
template<> struct tnums <5>
{
	template <long v0,long v1,long v2,long v3,long v4>
	struct apply
	{
		typedef tnum5<v0,v1,v2,v3,v4> type;
	};
};
template<> struct tnums <6>
{
	template <long v0,long v1,long v2,long v3,long v4,long v5>
	struct apply
	{
		typedef tnum6<v0,v1,v2,v3,v4,v5> type;
	};
};
template<> struct tnums <7>
{
	template <long v0,long v1,long v2,long v3,long v4,long v5,long v6>
	struct apply
	{
		typedef tnum7<v0,v1,v2,v3,v4,v5,v6> type;
	};
};
template<> struct tnums <8>
{
	template <long v0,long v1,long v2,long v3,long v4,long v5,long v6,long v7>
	struct apply
	{
		typedef tnum8<v0,v1,v2,v3,v4,v5,v6,v7> type;
	};
};



___namespace1_end()
