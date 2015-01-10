
/**
	×÷Õß£ºÐìº£Áú
	2010-11-10
	a met binary type, and some relevant operations
*/

#include "../ox/nsab.h"
#include "./static_assert.h"
#include "./math.h"


#pragma once


___namespace2_begin(ox,met)



template<size_t N,size_t b=1>
struct countof_bin
{
	static size_t const N0 = b?N:~N;
	static size_t const N1 = (0x55555555&N0) + ((0xaaaaaaaa&N0) >> 1);
	static size_t const N2 = (0x33333333&N1) + ((0xcccccccc&N1) >> 2);
	static size_t const N3 = (0x0f0f0f0f&N2) + ((0xf0f0f0f0&N2) >> 4);
	static size_t const N4 = (0x00ff00ff&N3) + ((0xff00ff00&N3) >> 8);
	static size_t const N5 = (0x0000ffff&N4) + ((0xffff0000&N4) >> 16);
	static size_t const value = N5;
};

template<size_t N,size_t b=0>
struct leadingcountof_bin
{
	static size_t const N0 = b?~N:N;
	static size_t const N1 = N0 | (N0>>1);
	static size_t const N2 = N1 | (N1>>2);
	static size_t const N3 = N2 | (N2>>4);
	static size_t const N4 = N3 | (N3>>8);
	static size_t const N5 = N4 | (N4>>16);
	static size_t const value = countof_bin<N5,0>::value;
};

template<size_t N,size_t b=0>
struct rearcountof_bin
{
	static size_t const N1 = b?N+1:N-1;
	static size_t const value = countof_bin<N^N1,1>::value-1;
};

template<size_t N,size_t b=1>
struct keep_lowest
{
	static size_t const N1 = b?N-1:N+1;
	static size_t const N2 = N^N1;
	static size_t const N3 = (~N2)>>1;
	static size_t const N4 = N2&N3;
	static size_t const value = b?N4:~N4;
};

template<size_t N,size_t b=1>
struct keep_highest
{
	static size_t const N0 = b?N:~N;
	static size_t const N1 = N0 | (N0>>1);
	static size_t const N2 = N1 | (N1>>2);
	static size_t const N3 = N2 | (N2>>4);
	static size_t const N4 = N3 | (N3>>8);
	static size_t const N5 = N4 | (N4>>16);

	static size_t const N6 = (~N5)>>1;
	static size_t const N7 = N5&N6;
	static size_t const value = b?N7:~N7;
};


template <size_t N,bool c=true, size_t suff=0>
struct suffix_of_lowbit
{
private:
	template <size_t N, size_t suff, bool hit>
	struct suffix_lowd;

	template <size_t N, size_t suff>
	struct suffix_lowd<N, suff, true>
	{
		static size_t const value = suff;
	};
	template <size_t N, size_t suff>
	struct suffix_lowd<N, suff, false>
	{
		static size_t const value = suffix_lowd<(N>>1),suff+1,((N&2)?true:false)>::value;
	};
private:
	static size_t const n = N>>suff;
	struct get_suffix { static size_t const value = -1; };
public:
	static size_t const value
		= 
		met::iff<n==0,get_suffix,suffix_lowd<n,0,n&1>>::type::value;
};

template <size_t N,bool c=true, size_t suff=0>
struct suffix_of_highbit
{
private:
	template <size_t N, size_t suff, bool hit>
	struct suffix_highd;

	template <size_t N, size_t suff>
	struct suffix_highd<N, suff, true>
	{
		static size_t const value = suff;
	};
	template <size_t N, size_t suff>
	struct suffix_highd<N, suff, false>
	{
		static size_t const value = suffix_highd<(N<<1),suff+1,((N&2)?true:false)>::value;
	};
private:
	static size_t const n = N>>suff;
	struct get_suffix { static size_t const value = -1; };
public:
	static size_t const value
		= 
		met::iff<n==0,get_suffix,suffix_lowd<n,0,n&1>>::type::value;
};

/// we do not care who is max between the begin and the end number
/// "[" suffix_begin_tc, suffix_end_tc "]" is given as closed interval form
/// <2,3> = ...0001100
template <unsigned int bit, unsigned int suff_begin, unsigned int suff_end=suff_begin>
struct mk_intv
{
private:
	static unsigned long const _1value
		=
		(-1<< (maxv<unsigned long,suff_begin,suff_end>::value+1) )
		^
		(-1<< (minv<unsigned long,suff_begin,suff_end>::value) );
public:
	static unsigned long const value = bit?_1value:~_1value;
};

//suffix_tc based from <0> ZERO, whether the bit at suffix is true
// [begin,end]
template <unsigned long n, unsigned char suff_begin, unsigned char suff_end = suff_begin>
struct test_intv
{
	static bool const value
		= n & mk_intv<1,suff_begin,suff_end>::value;
};


/**
	[begin,end]
*/
template <unsigned int n, bool bit, unsigned int suff_begin, unsigned int suff_end = suff_begin>
struct set_intv
{
	static unsigned int const value
		= bit
		? (n | mk_intv<1,suff_begin,suff_end>::value)
		: (n & mk_intv<1,suff_begin,suff_end>::value)
	;
};

template <unsigned int n, unsigned int suff_begin, unsigned int suff_end = suff_begin>
struct get_intv
{
	static unsigned int const value
		= n & mk_intv<1,suff_begin,suff_end>::value
	;
};

/**
	[begin,end]
*/
template <unsigned int n, unsigned int suff_begin, unsigned int suff_end = suff_begin>
struct cmc_intv
{
	static unsigned int const value
		=
		set_intv<n,0,suff_begin,suff_end>::value |
		get_intv<~n,suff_begin,suff_end>::value
	;
};


/// oct. 1101 = 0b1101
/// normal cbin
template <size_t n, size_t v=0, size_t l=0>
struct ncbin
{
	static size_t const bit = n%10?1:0;
	static size_t const value_of_curr = (bit<<l)|v;
	static size_t const value = ncbin<n/10,value_of_curr,l+1>::value;
};
template <size_t v, size_t l>
struct ncbin <0,v,l>
{
	static size_t const value = v;
};

/// 9<=>0, but 9 will not be ignored at the front, as a valid bit
/// result (value,size)
/// oct. 31 = (0b111,3) = 1x3
/// oct. 11 = (0b11,1)
/// oct. 19 = (0b0,1)
/// oct. 99 = (0b0,2)
/// just two bits, one unit
/// single combine binary
template <size_t b1,size_t b0>
struct scbin
{
private:
	static size_t const c = b1;//a/10;
	static size_t const b = b0==9?0:b0;//a%10;
	___static_assert(b<2);
	template <bool than1>
	struct scatter
	{
		static size_t const value = b?mk_intv<1,c-1,0>::value:0;
	};
	template <>
	struct scatter <false>
	{
		static size_t const value = /*c<<1|*/b;
	};
	static size_t const s = (c>1 && c<9)?c:1;
public:
	static size_t const value = scatter<(s>1)>::value;
	struct size { static size_t const value = s; };
};


/// oct. 203110 = 0b0011110 = 0x2,1x3,1,0
/// combined binary
/// value : the binary
/// size::value : the valid size of binary
/// e.g. 110 s=3; 001010 s=4; 3010 s=5
template <size_t n, size_t v=0, size_t s=0>
struct cbin
{
private:
	static size_t const c = (n%100)/10;
	static size_t const b = n%10;
	___static_assert(b==0||b==1||b==9);

	typedef scbin<c,b> sb;

	static size_t const nn = sb::size::value>1 ? n/100 : n/10;
	static size_t const nv = sb::value<<s | v;
	//static size_t const nv = sb::value>0 ? (v<<sb::size::value | sb::value) : v;
	static size_t const ns = s+sb::size::value;

	typedef cbin<nn,nv,ns> cb;

public:
	static size_t const value = cb::value;
	struct size { static size_t const value = cb::size::value; };
};
template <size_t v,size_t s>
struct cbin <0,v,s>
{
	static size_t const value = v;
	struct size { static size_t const value = s; };
};
template <size_t s>
struct cbin <0,0,s>
{
	static size_t const value = 0;
	struct size { static size_t const value = s; };
};

template <size_t n>
struct cbin2
{
private:
	typedef cbin<n> bin;
public:
	static size_t const value = bin::value;
	typedef typename bin::size size;
};
template <>
struct cbin2 <-1>
{
	static size_t const value = 0;
	struct size { static size_t const value = 0; };
};
template <>
struct cbin2 <0>
{
	static size_t const value = 0;
	struct size { static size_t const value = 1; };
};



/// combine bit to and unsigned int [32 bits]
template
<
	unsigned int b31=-1, unsigned int b30=-1, unsigned int b29=-1, unsigned int b28=-1, unsigned int b27=-1, unsigned int b26=-1, unsigned int b25=-1, unsigned int b24=-1,
	unsigned int b23=-1, unsigned int b22=-1, unsigned int b21=-1, unsigned int b20=-1, unsigned int b19=-1, unsigned int b18=-1, unsigned int b17=-1, unsigned int b16=-1,
	unsigned int b15=-1, unsigned int b14=-1, unsigned int b13=-1, unsigned int b12=-1, unsigned int b11=-1, unsigned int b10=-1, unsigned int b9=-1, unsigned int b8=-1,
	unsigned int b7=-1, unsigned int b6=-1, unsigned int b5=-1, unsigned int b4=-1, unsigned int b3=-1, unsigned int b2=-1, unsigned int b1=-1, unsigned int b0=-1
>
struct binary
{
private:
	typedef binary
		<
			b31, b30, b29, b28, b27, b26, b25, b24,
			b23, b22, b21, b20, b19, b18, b17, b16,
			b15, b14, b13, b12, b11, b10, b9, b8
		>
	bin31_8;
	typedef binary<b7, b6, b5, b4, b3, b2, b1, b0> bin7_0;

public:
	static size_t const value
		=
		bin31_8::value << bin7_0::size::value |
		bin7_0::value;
		;
	struct size
	{
		static size_t const value = bin31_8::size::value+bin7_0::size::value;
	};
};

/// when the right 8 part is -1
template
<
	unsigned int b23, unsigned int b22, unsigned int b21, unsigned int b20, unsigned int b19, unsigned int b18, unsigned int b17, unsigned int b16,
	unsigned int b15, unsigned int b14, unsigned int b13, unsigned int b12, unsigned int b11, unsigned int b10, unsigned int b9, unsigned int b8,
	unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0
>
struct binary
<
	b23, b22, b21, b20, b19, b18, b17, b16,
	b15, b14, b13, b12, b11, b10, b9, b8,
	b7, b6, b5, b4, b3, b2, b1, b0,
	-1,-1,-1,-1, -1,-1,-1,-1
>
{
private:
	typedef binary
		<
			b23, b22, b21, b20, b19, b18, b17, b16,
			b15, b14, b13, b12, b11, b10, b9, b8
		>
	bin23_8;
	typedef binary<b7, b6, b5, b4, b3, b2, b1, b0> bin7_0;

public:
	static size_t const value
		=
		bin23_8::value << bin7_0::size::value |
		bin7_0::value;
		;
	struct size
	{
		static size_t const value = bin23_8::size::value+bin7_0::size::value;
	};
};

/// when the right 16 part is -1
template
<
	unsigned int b15, unsigned int b14, unsigned int b13, unsigned int b12, unsigned int b11, unsigned int b10, unsigned int b9, unsigned int b8,
	unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0
>
struct binary
<
	b15, b14, b13, b12, b11, b10, b9, b8,
	b7, b6, b5, b4, b3, b2, b1, b0,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1
>
{
private:
	typedef binary<b15, b14, b13, b12, b11, b10, b9, b8> bin15_8;
	typedef binary<b7, b6, b5, b4, b3, b2, b1, b0> bin7_0;

public:
	static size_t const value
		=
		bin15_8::value << bin7_0::size::value |
		bin7_0::value;
		;
};

/// when the left 8 part is -1
template
<
	unsigned int b7, unsigned int b6, unsigned int b5, unsigned int b4, unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0
>
struct binary
<
	b7, b6, b5, b4, b3, b2, b1, b0,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1
>
{
private:
	typedef binary<b7,b6,b5,b4> bin7_4;
	typedef binary<b3,b2,b1,b0> bin3_0;
public:
	static size_t const value
		=
		bin7_4::value << bin3_0::size::value |
		bin3_0::value;
	struct size
	{
		static size_t const value
			= bin7_4::size::value+bin3_0::size::value;
	};
};
/// when the left 4 part is not -1
template
<
	unsigned int b3, unsigned int b2, unsigned int b1, unsigned int b0
>
struct binary
<
	b3, b2, b1, b0, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1
>
{
private:
	typedef binary<b3,b2> bin32;
	typedef binary<b1,b0> bin10;

public:
	static size_t const value
		=
		bin32::value << bin10::size::value |
		bin10::value
		;
	struct size
	{
		static size_t const value
			= bin32::size::value+bin10::size::value;
	};
};

/// when the left 2 part is not -1
template
<
	unsigned int b1, unsigned int b0
>
struct binary
<
	b1, b0, -1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1
>
{
private:
	typedef cbin2<b1> bin1;
	typedef cbin2<b0> bin0;

public:
	static size_t const value
		=
		bin1::value << bin0::size::value |
		bin0::value
		;
	struct size
	{
		static size_t const value
			= bin1::size::value+bin0::size::value;
	};
};

/// when the left 1 part is not -1
template <unsigned int b0>
struct binary
<
	b0, -1, -1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1
>
{
private:
	typedef cbin2<b0> bin0;

public:
	static size_t const value = bin0::value;
	struct size
	{
		static size_t const value = bin0::size::value;
	};
};
/// when all the N is -1
template <>
struct binary
<
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1,
	-1,-1,-1,-1, -1,-1,-1,-1
>
{
public:
	static size_t const value = -1;
	struct size
	{
		static size_t const value = sizeof(size_t)*8;
	};
};

template
<
	unsigned int b31=-1, unsigned int b30=-1, unsigned int b29=-1, unsigned int b28=-1, unsigned int b27=-1, unsigned int b26=-1, unsigned int b25=-1, unsigned int b24=-1,
	unsigned int b23=-1, unsigned int b22=-1, unsigned int b21=-1, unsigned int b20=-1, unsigned int b19=-1, unsigned int b18=-1, unsigned int b17=-1, unsigned int b16=-1,
	unsigned int b15=-1, unsigned int b14=-1, unsigned int b13=-1, unsigned int b12=-1, unsigned int b11=-1, unsigned int b10=-1, unsigned int b9=-1, unsigned int b8=-1,
	unsigned int b7=-1, unsigned int b6=-1, unsigned int b5=-1, unsigned int b4=-1, unsigned int b3=-1, unsigned int b2=-1, unsigned int b1=-1, unsigned int b0=-1
>
struct x2 :
	met::binary
	<
		b31, b30, b29, b28, b27, b26, b25, b24,
		b23, b22, b21, b20, b19, b18, b17, b16,
		b15, b14, b13, b12, b11, b10, b9, b8,
		b7, b6, b5, b4, b3, b2, b1, b0
	>
{};


___namespace2_end()


/*
	each bit must be a decimal. number which is regarded as a binary number in background
	ahead of each bit number, the 0 will be ignored, and also wonot increase the size of the nubmer
	and also ammetion: in C++ language, oct. canot be started with 0, orelse will be interpreted as octal
	so, never input a number with 0 ahead, because we will interpret the number by decimal format
	if you want to write the zero in front of the number, please use 9 instead of it.
	the number 9 will be regarded as a zero always
	one exception. that, one number, just has zero, we will regard it as 0 with size 1
	for example:
	<1,000,10> ---> <1,0,10> and with size<1,1,2>
	<01,09,0> ---> error, invalid, 09 and 01 started with 0, okay you can write like the fellowed
	<91,99,0> ---> okay, 2x01,00,0 and with size<2,2,1>
	<91,99,0> ---> okay, 2x01,00,0 and with size<2,2,1>
*/
