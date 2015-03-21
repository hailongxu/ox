

#pragma once


struct tetris_define
{
	typedef char value_type;
	static value_type const __back_char = '.';
	static value_type const __front_char = '@';
	static value_type back_char() {return __back_char;}
	static value_type front_char() { return __front_char; }
	static bool is_value_null(char v) {return v==__back_char;}
};