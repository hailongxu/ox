
#include "math.h"
#include "win_gui.h"



#pragma once


struct matrix_shape
{
	win_gui& _m_gui;
	point_t left_top(point_t const& p)
	{
		point_t r;
		return r;
	}
	void draw(int row_count,int col_count)
	{
		for (int r=0;r<row_count;++r)
			for (int c=0;c<col_count;++c)
			{
				_m_gui.draw_rectangle();
			}
	}
	point_t _m_orgin;
	int _m_unit_width;
	int _m_unit_height;
	int _m_left_margin;
	int _m_right_margin;
	int _m_top_margin;
	int _m_bottom_margin;
	int _m_unit_height_margin;
	int _m_unit_width_margin;
};


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

