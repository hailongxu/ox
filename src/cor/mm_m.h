
/*
	copyright:  you can freely copy, reuse and distribute the code, 
	            but should add the head comment in front of your file.
	---------------------------------------------------------------------
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	时间     :  2008-1-14
	Hobby    :  Playing Table Tenis, Playing China Chess, Speaking English, etc.
	            Programing, Linux, C++, Network, etc.
	P.S.     :  You can contract me at any time.
 */

#include <memory.h>
#include <wchar.h>

#pragma once

namespace ox
{
	namespace cor
	{
		struct mm_m
		{
			template <typename T>
			static void memove(
				T * _outval_dst,
				T const * const src,
				size_t count)
			{}
			
			template <> /// cell is signed char
			static void memove<signed char>(
				signed char * _outval_dst,
				signed char const * const src,
				size_t count)
			{
				::memmove(
					_outval_dst,
					src,
					count);
			}
			template <> /// cell is char
			static void memove<char>(
				char * _outval_dst,
				char const * const src,
				size_t count)
			{
				::memmove(
					_outval_dst,
					src,
					count);
			}
			template <> /// cell is char
			static void memove<unsigned char>(
				unsigned char * _outval_dst,
				unsigned char const * const src,
				size_t count)
			{
				::memmove(
					_outval_dst,
					src,
					count);
			}
			template <> /// cell is wchar_t
			static void memove<wchar_t>(
				wchar_t * _outval_dst,
				wchar_t const * const src,
				size_t count)
			{
				::wmemmove(
					_outval_dst,
					src,
					count);
			}

			/// mem set
			template <typename cell_tn>
			static void memset(
				cell_tn * const cell_begin,
				cell_tn const & cell_value,
				size_t count)
			{
				cell_tn * pend = cell_begin + count;
				cell_tn * n = cell_begin;
				::memmove(n++,&cell_value,sizeof(cell_tn));
				while (true)
				{
					if (pend-n>n-cell_begin)
						::memmove(
							n,
							cell_begin,
							(n-cell_begin)*sizeof(cell_tn));
					else
						break;
				}
				::memmove(
					n,
					cell_begin,
					(pend-n)*sizeof(cell_tn));
			}
			template <>
			static void memset <char> (
				char * const char_begin,
				char const & char_value,
				size_t count)
			{
				::memset(char_begin,char_value,count);
			}
			//template <>
			//void memset <unsigned char> (unsigned char * const _inptrcon_uchar, unsigned char _in_uchar, size_t count)
			//{
			//	::memset(_inptrcon_uchar,_in_uchar,count);
			//}
			template <>
			static void memset <wchar_t> (
				wchar_t * const wchar_begin,
				wchar_t const & wchar_value,
				size_t count)
			{
				::wmemset(wchar_begin,wchar_value,count);
			}

			static void memset(
				void * const void_dst_begin,
				size_t void_dst_count,
				void const * const void_src_cell,
				size_t void_src_cell_size)
			{
				::memmove(void_dst_begin,void_src_cell,void_src_cell_size);
				char * begin = static_cast<char*>(void_dst_begin);
				char * end = static_cast<char*>(void_dst_begin)+void_dst_count;
				char * n = static_cast<char*>(void_dst_begin) + void_src_cell_size;
				while (true)
				{
					if (n-begin>end-n)
						::memmove(n,begin,n-begin);
					else
						break;
				}
				::memmove(n,begin,end-n);
			}

			/// mem cmp
			template <typename cell_tn>
			static int memcmp(
				cell_tn const * const cell_1,
				cell_tn const * const cell_2,
				size_t count)
			{
				return ::memcmp(cell_1,cell_2,count*sizeof(cell_tn));
			}
			template <>
			static int memcmp <void> (
				void const * const void_1,
				void const * const void_2,
				size_t size)
			{
				return ::memcmp(void_1,void_2,size);
			}
			template <>
			static int memcmp <wchar_t> (
				wchar_t const * const wchar_1,
				wchar_t const * const wchar_2,
				size_t count)
			{
				return ::wmemcmp(wchar_1,wchar_2,count);
			}

			template <typename cell_tn>
			static int memcmp(
				cell_tn const * const cell_1,
				size_t cell_1_count,
				cell_tn const * const cell_2,
				size_t cell_2_count)
			{
				int _default_ret;
				int _min_size;
				int _cmpret = cell_1_count-cell_2_count;

				if (_cmpret<0)
					_default_ret=-1, _min_size=cell_1_count;
				else if (_cmpret>0)
					_default_ret=+1, _min_size=cell_2_count;
				else
					_default_ret= 0, _min_size=cell_1_count;

				int _locret = ::memcmp(
					cell_1,
					cell_2,
					_min_size*sizeof(cell_tn));
				return _locret==0?_default_ret:_locret;
			}

			/// mem case ignore cmp
			//template <typename cell_tn>
			//int memicmp(cell_tn const * const cell_1, cell_tn const * const cell_2, size_t count)
			//{
			//	return ::memicmp(cell_1,cell_2,count*sizeof(cell_tn));
			//}
			//template <>
			//int memicmp <void> (void const * const cell_1, void const * const cell_2, size_t count)
			//{
			//	return ::memicmp(cell_1,cell_2,count);
			//}
			//template <>
			//int memicmp <wchar_t> (wchar_t const * const cell_1, wchar_t const * const cell_2, size_t count)
			//{
			//	return ::wmemicmp(cell_1,cell_2,count);
			//}
		}; /// end of class mm_m
	} /// end of namespace cor
} /// end of namespace ox