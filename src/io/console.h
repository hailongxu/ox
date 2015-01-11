/*
	--------------------------------------------------------------
	类名: 控制台的输入和输出函数
	作者: 徐海龙
	日期: 2008-1-1
	文件名: console.h
	功能: 
 */

#include "../pkg/character_trait.h"
#include <stdio.h>
#include <stdarg.h>
#include <conio.h>

#pragma once

namespace ox
{
namespace io
{
namespace console
{
	template <typename character_tn> struct in_trait;
	template <> struct in_trait<char>
	{
		static int getch() { return _getch(); }
	};
	template <> struct in_trait<wchar_t>
	{
		static int getch() { return _getwch(); }
	};

	template <typename character_tn> struct in :
		public in_trait<character_tn>
	{};

	template <typename character_tn> struct out_trait;
	template <>	struct out_trait<char>
	{
		typedef ox::character::character_trait<char>::character character;
		static int printf(const character * format, ...)
		{
			va_list arglist;
			va_start(arglist, format);
			return vprintf (format,arglist);
		};
	};
	template <>	struct out_trait<wchar_t>
	{
		typedef ox::character::character_trait<wchar_t>::character character;
		static int printf(const character * format, ...)
		{
			va_list arglist;
			va_start(arglist, format);
			return vwprintf (format,arglist);
		};
	};

	template <typename character_tn>
	struct out : public out_trait<character_tn>
	{};

} // end of console
} // end of io
} // end of ox