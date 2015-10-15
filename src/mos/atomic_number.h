

/*
	author: xuhailong
	date: 2011-5-20
	msn: hailongxu@msn.com
	Hi: hsuhailong@baidu.com

	synchronized integer number, speedest
 */


#include <windows.h>
#include "../ox/nsab.h"

// fix error C2733: second C linkage of overloaded function '_interlockedbittestandreset' not allowed
// definition conflict between winSDK(winnt.h) and VC8(intrin.h), 
// we disable VC8 version here, using macro replacement
#define _interlockedbittestandset   _interlockedbittestandset_VC8_version
#define _interlockedbittestandreset _interlockedbittestandreset_VC8_version
#include <intrin.h>
#undef _interlockedbittestandset
#undef _interlockedbittestandreset

#pragma once


___namespace2_begin(ox,mos)

/// _InterlockedCompareExchange : any cpu
/// interlockedadd : just x86


template <typename number_tn>
struct atomic_number;

//template <>
//struct atomic_number <short>
//{
//	typedef short integer_type;
//	atomic_number() : _m_value(0) {}
//	atomic_number(integer_type value) : _m_value(value) {}
//
//	operator integer_type()
//	{
//		return value();
//	}
//	integer_type value() const
//	{
//		return InterlockedExchangeAdd16(&_m_value,0);
//	}
//	integer_type operator++()
//	{
//		return inc();
//	}
//	integer_type operator--()
//	{
//		return dec();
//	}
//	integer_type operator++(int)
//	{
//		return add_and_return_old(1);
//	}
//	integer_type operator--(int)
//	{
//		return sub_and_return_old(1);
//	}
//	//integer_type operator+=(integer_type addend)
//	//{
//	//	return add(addend);
//	//}
//	//integer_type operator-=(integer_type subtrahend)
//	//{
//	//	return sub(subtrahend);
//	//}
//	/// return old value
//	integer_type operator=(integer_type value)
//	{
//		return assign_and_return_old(value);
//	}
//	integer_type operator&(integer_type andend)
//	{
//		return add_and_return_old(andend);
//	}
//	integer_type inc()
//	{
//		return InterlockedIncrement(&_m_value);
//	}
//	integer_type dec()
//	{
//		return InterlockedDecrement(&_m_value);
//	}
//	/// 
//	integer_type add_and_return_old(integer_type addend)
//	{
//		return InterlockedExchangeAdd(&_m_value,addend);
//		//return InterlockedExchange(&_m_value,_m_value+addend);
//		//return InterlockedCompareExchange(&_m_value,_m_value+addend,_m_value);
//	}
//	integer_type sub_and_return_old (integer_type subtrahend)
//	{
//		return InterlockedExchangeAdd(&_m_value,-subtrahend);
//		//return InterlockedExchange(&_m_value,_m_value-subtrahend);
//		//return InterlockedCompareExchange(&_m_value,_m_value-subtrahend,_m_value);
//	}
//	integer_type assign_and_return_old(integer_type value)
//	{
//		return InterlockedExchange(&_m_value,value);
//		//return InterlockedCompareExchange(&_m_value,value,_m_value);
//	}
//	integer_type and_and_return_old(integer_type andend)
//	{
//		return _InterlockedAnd(&_m_value,andend);
//	}
//	integer_type compare_assign_and_return_old(integer_type value,integer_type comperand)
//	{
//		return _InterlockedCompareExchange(&_m_value,value,comperand);
//	}
//
//private:
//	volatile mutable integer_type _m_value;
//};
//
//
//
template <>
struct atomic_number <long>
{
	typedef long integer_type;
	atomic_number() : _m_value(0) {}
	atomic_number(integer_type value) : _m_value(value) {}

	operator integer_type()
	{
		return value();
	}
	integer_type value() const
	{
		return _m_value;//InterlockedExchangeAdd(&_m_value,0);
	}
	integer_type operator++()
	{
		return inc();
	}
	integer_type operator--()
	{
		return dec();
	}
	integer_type operator++(int)
	{
		return add_and_return_old(1);
	}
	integer_type operator--(int)
	{
		return sub_and_return_old(1);
	}
	//integer_type operator+=(integer_type addend)
	//{
	//	return add(addend);
	//}
	//integer_type operator-=(integer_type subtrahend)
	//{
	//	return sub(subtrahend);
	//}
	/// return old value
	integer_type operator=(integer_type value)
	{
		return assign_and_return_old(value);
	}
	integer_type operator&(integer_type andend)
	{
		return add_and_return_old(andend);
	}
	integer_type inc()
	{
		return InterlockedIncrement(&_m_value);
	}
	integer_type dec()
	{
		return InterlockedDecrement(&_m_value);
	}
	/// 
	integer_type add_and_return_old(integer_type addend)
	{
		return InterlockedExchangeAdd(&_m_value,addend);
		//return InterlockedExchange(&_m_value,_m_value+addend);
		//return InterlockedCompareExchange(&_m_value,_m_value+addend,_m_value);
	}
	integer_type sub_and_return_old (integer_type subtrahend)
	{
		return InterlockedExchangeAdd(&_m_value,-subtrahend);
		//return InterlockedExchange(&_m_value,_m_value-subtrahend);
		//return InterlockedCompareExchange(&_m_value,_m_value-subtrahend,_m_value);
	}
	integer_type assign_and_return_old(integer_type value)
	{
		return InterlockedExchange(&_m_value,value);
		//return InterlockedCompareExchange(&_m_value,value,_m_value);
	}
	integer_type and_and_return_old(integer_type andend)
	{
		return _InterlockedAnd(&_m_value,andend);
	}
	integer_type compare_assign_and_return_old(integer_type value,integer_type comperand)
	{
		return _InterlockedCompareExchange(&_m_value,value,comperand);
	}

private:
	volatile /*mutable*/ integer_type _m_value;
};
typedef atomic_number<long> atomic_long;


//#if _WIN32_WINNT >= 0x0502
///// the windows ver >= vista
//template <>
//struct atomic_number <__int64>
//{
//	typedef __int64 integer_type;
//	atomic_number() : _m_value(0) {}
//	atomic_number(integer_type value) : _m_value(value) {}
//
//	operator integer_type()
//	{
//		return value();
//	}
//	integer_type value() const
//	{
//		return InterlockedExchangeAdd64(&_m_value,0);
//	}
//	integer_type operator++()
//	{
//		return inc();
//	}
//	integer_type operator--()
//	{
//		return dec();
//	}
//	integer_type operator++(int)
//	{
//		return add_and_return_old(1);
//	}
//	integer_type operator--(int)
//	{
//		return sub_and_return_old(1);
//	}
//	//integer_type operator+=(integer_type addend)
//	//{
//	//	return add(addend);
//	//}
//	//integer_type operator-=(integer_type subtrahend)
//	//{
//	//	return sub(subtrahend);
//	//}
//	/// return old value
//	integer_type operator=(integer_type value)
//	{
//		return assign_and_return_old(value);
//	}
//	integer_type operator&(integer_type andend)
//	{
//		return add_and_return_old(andend);
//	}
//	integer_type inc()
//	{
//		return InterlockedIncrement64(&_m_value);
//	}
//	integer_type dec()
//	{
//		return InterlockedDecrement64(&_m_value);
//	}
//	/// 
//	integer_type add_and_return_old(integer_type addend)
//	{
//		return InterlockedExchangeAdd64(&_m_value,addend);
//		//return InterlockedExchange(&_m_value,_m_value+addend);
//		//return InterlockedCompareExchange(&_m_value,_m_value+addend,_m_value);
//	}
//	integer_type sub_and_return_old (integer_type subtrahend)
//	{
//		return InterlockedExchangeAdd64(&_m_value,-subtrahend);
//		//return InterlockedExchange(&_m_value,_m_value-subtrahend);
//		//return InterlockedCompareExchange(&_m_value,_m_value-subtrahend,_m_value);
//	}
//	integer_type assign_and_return_old(integer_type value)
//	{
//		return InterlockedExchange64(&_m_value,value);
//		//return InterlockedCompareExchange(&_m_value,value,_m_value);
//	}
//	integer_type and_and_return_old(integer_type andend)
//	{
//		return _InterlockedAnd64(&_m_value,andend);
//	}
//	integer_type compare_assign_and_return_old(integer_type value,integer_type comperand)
//	{
//		return _InterlockedCompareExchange64(&_m_value,value,comperand);
//	}
//
//private:
//	volatile mutable integer_type _m_value;
//};
//typedef atomic_number<__int64> atomic_int64;
//#endif


___namespace2_end()