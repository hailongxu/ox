

/*
	author: xuhailong
	date: 2011-5-20
	msn: hailongxu@msn.com
	Hi: hsuhailong@baidu.com

	synchronized integer number, speedest
 */


#include <windows.h>

// fix error C2733: second C linkage of overloaded function '_interlockedbittestandreset' not allowed
// definition conflict between winSDK(winnt.h) and VC8(intrin.h), 
// we disable VC8 version here, using macro replacement
#define _interlockedbittestandset   _interlockedbittestandset_VC8_version
#define _interlockedbittestandreset _interlockedbittestandreset_VC8_version
#include <intrin.h>
#undef _interlockedbittestandset
#undef _interlockedbittestandreset

#pragma once



/// _InterlockedCompareExchange : any cpu
/// interlockedadd : just x86

template <typename number_tn>
struct atomic_number;

template <>
struct atomic_number <long>
{
	atomic_number() : _m_value(0) {}
	atomic_number(long value) : _m_value(value) {}

	operator long()
	{
		return value();
	}
	long value() const
	{
		return InterlockedExchangeAdd(&_m_value,0);
	}
	long operator++()
	{
		return inc();
	}
	long operator--()
	{
		return dec();
	}
	long operator++(int)
	{
		return add_and_return_old(1);
	}
	long operator--(int)
	{
		return sub_and_return_old(1);
	}
	//long operator+=(long addend)
	//{
	//	return add(addend);
	//}
	//long operator-=(long subtrahend)
	//{
	//	return sub(subtrahend);
	//}
	long operator=(long value)
	{
		return assign(value);
	}
	long operator&(long andend)
	{
		return and(andend);
	}
	long inc()
	{
		return InterlockedIncrement(&_m_value);
	}
	long dec()
	{
		return InterlockedDecrement(&_m_value);
	}
	/// 
	long add_and_return_old(long addend)
	{
		return InterlockedExchangeAdd(&_m_value,addend);
		//return InterlockedExchange(&_m_value,_m_value+addend);
		//return InterlockedCompareExchange(&_m_value,_m_value+addend,_m_value);
	}
	long sub_and_return_old (long subtrahend)
	{
		return InterlockedExchangeAdd(&_m_value,-subtrahend);
		//return InterlockedExchange(&_m_value,_m_value-subtrahend);
		//return InterlockedCompareExchange(&_m_value,_m_value-subtrahend,_m_value);
	}
	long assign(long value)
	{
		return InterlockedExchange(&_m_value,value);
		//return InterlockedCompareExchange(&_m_value,value,_m_value);
	}
	long and(long andend)
	{
		return _InterlockedAnd(&_m_value,andend);
	}

private:
	volatile mutable long _m_value;
};

typedef atomic_number<long> atomic_long;

template <>
struct atomic_number <__int64>
{
	atomic_number() : _m_value(0) {}
	atomic_number(__int64 value) : _m_value(value) {}

	__int64 operator+=(__int64 addend)
	{
		return add(addend);
	}
	__int64 operator-=(__int64 subtrahend)
	{
		return sub(subtrahend);
	}
	__int64 operator=(__int64 value)
	{
		return assign(value);
	}
	__int64 operator&(__int64 andend)
	{
		return and(andend);
	}
	__int64 add(__int64 addend)
	{
		return 0;
		//return InterlockedCompareExchange64(&_m_value,_m_value+addend,_m_value);
	}
	__int64 sub (__int64 subtrahend)
	{
		return 0;
		//return InterlockedCompareExchange64(&_m_value,_m_value-subtrahend,_m_value);
	}
	__int64 assign(__int64 value)
	{
		return 0;
		//return InterlockedCompareExchange64(&_m_value,value,_m_value);
	}
	__int64 and(__int64 andend)
	{
		return 0;
		//return _InterlockedAnd64(&_m_value,andend);
	}
	__int64 value() const
	{
		return _m_value;
	}

private:
	__int64 _m_value;
};
