
#include "../../ox/nsab.h"

#include "math.h"
#include "win_cui.h"



#pragma once



___namespace3_begin(ox,app,tetris)


struct tetris_win_cui
{
	tetris_win_cui(win_console& console) : _m_console(console)
	{}
	win_console& _m_console;
	void fill_rect(rc_rect_t const& rect,char v)
	{
		_m_console.fill_rect(rect,v);
	}
	void draw_point(rc_point_t const& p, char v)
	{
		_m_console.draw_point(p,v);
	}
	void draw_text(rc_point_t const& p,char const* str,size_t size)
	{
		_m_console.draw_text(p,str,size);
	}
};


___namespace3_end()