
#include "../sys/grammer.h"

#pragma once

template<typename T>
struct ox_basetype_para
{
	typedef T half_ty;
	typedef T full_uty;
};

template<>
struct ox_basetype_para<ox_dword>
{
	typedef ox_word half_ty;
	typedef ox_dword full_uty;
};

template<>
struct ox_basetype_para<ox_sdword>
{
	typedef ox_word half_ty;
	typedef ox_dword full_uty;
};

template<>
struct ox_basetype_para<ox_word>
{
	typedef ox_byte half_ty;
	typedef ox_word full_uty;
};

template<>
struct ox_basetype_para<ox_sword>
{
	typedef ox_byte half_ty;
	typedef ox_word full_uty;
};


template <typename T_r>
class ox_basetype_kit
{
public:
	typename typedef ox_basetype_para<T_r>::half_ty T_p;
	typename typedef ox_basetype_para<T_r>::full_uty r_uty;

	static
	T_r v(T_p hi, T_p lo)
	{
		return (((T_r)(hi))<<(sizeof(T_p)*ox_byte_bit_size))|((T_r)(lo));
	}

	static
	T_p h(T_r value)
	{
		return static_cast<T_p>(((value)>>sizeof(T_p)*ox_byte_bit_size)/* & ((T_r)(-1)<<sizeof(T_p)*ox_byte_bitlen)*/);
	}

	static
	T_p l(T_r value)
	{
		return static_cast<T_p>(((value))/* & ((T_r)(-1)<<sizeof(T_p)*ox_byte_bitlen)*/);
	}

	enum { ox_byte_bit_size = 8 };
};

	//template<typename T>
	//static
	//T
	//make(typename ox_basetype_result<T_r>::result_ty half_high, T_half half_low)
	//{
	//	return (((T_one)(half_high))<<(sizeof(T_half)*ox_byte_len))|((T_one)(half_low));
	//}

	//template<typename T_half, typename T_one>
	//static
	//T_half high(T_one value)
	//{
	//	return (((value)>>sizeof(T_half)*ox_byte_len) & (1<<sizeof(T_half)*ox_byte_len));
	//}

	//template<typename T_half, typename T_one>
	//static
	//T_half low(T_one value)
	//{
	//	return ((value) & (1<<(sizeof(T_half)*ox_byte_len));)
	//}

//template <>
//struct ox_basetype_para <double>
//{
//	typedef int parameter_ty;
//};
//
//template <>
//class ox_basetype_kit<double>
//{
//public:
//	typedef ox_basetype_para<double>::parameter_ty T_p;
//
//	static
//	double make(T_p p)
//	{
//		return p;
//	}
//};
