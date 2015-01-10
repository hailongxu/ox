//
/////////////////////////////////////////////////////////////////////////////////
// 原创作者：徐海龙
// 创建时间：2007-5-12
// 历史信息：定义字符串相关的特性，以及通用类型
// 版权：版权归作者所有，本文件可以自由传播


#include "./character_trait.h"

#pragma once

namespace ox
{
namespace character
{

template <typename character_tn>
struct static_value
{
public:
	typedef typename ox::character::character_trait<character_tn>::character character;

	//static character to_character(char const & c) { return c; }
	//static character to_character(wchar_t const & c) { return c; }

	template<typename char_t, char_t c> struct to_static_character
	{
		static const character character_value = c;
	};
	template <unsigned char c> struct get
	{
		static const character c_value = c;
	};
	static const character null_character = 0;
	static const character space_character = to_static_character<char,' '>::character_value;
	static const character tab_character = to_static_character<char,'\t'>::character_value;
	/// move the cursor to the head of this line, at horizental line
	static const character carriagereturn_character = to_static_character<char,'\r'>::character_value;
	/// move the cursor to the next line, down the vertical line
	static const character linefeed_character = to_static_character<char,'\n'>::character_value;
	static const character slash_character = to_static_character<char,'/'>::character_value;
	static const character backslash_character = to_static_character<char,'\\'>::character_value;
	static const character dot_character = to_static_character<char,'.'>::character_value;

	static const character left_middle_brace_character = to_static_character<char,'['>::character_value;
	static const character right_middle_brace_character = to_static_character<char,']'>::character_value;

	static const character plus_character = to_static_character<char,'+'>::character_value;
	static const character minus_character = to_static_character<char,'-'>::character_value;
	static const character multiply_character = to_static_character<char,'*'>::character_value;
	static const character equal_character = to_static_character<char,'='>::character_value;
	static const character colon_character = to_static_character<char,':'>::character_value;
	static const character semicolon_character = to_static_character<char,';'>::character_value;

	static const character well_character = to_static_character<char,'#'>::character_value;
	static const character percent_character = to_static_character<char,'%'>::character_value;
};

//typedef character_static_value<char>

} // end of match
} // end of ox