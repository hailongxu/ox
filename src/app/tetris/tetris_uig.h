
#include "math.h"
#include "win_gui.h"



#pragma once




struct tetris_win_gui
{
	win_gui& _m_gui;
	tetris_win_gui(win_gui& gui): _m_gui(gui)
	{}
	void fill_rect(rect_t const& rect,char v)
	{
	}
	void draw_point(point_t const& p, char v)
	{
	}
	void draw_text(point_t const& p,char const* str,size_t size)
	{
	}
};

