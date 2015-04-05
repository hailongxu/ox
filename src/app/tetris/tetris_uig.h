
#include "math.h"
#include "win_gui.h"



#pragma once


struct matrix_shape
{
	typedef matrix_shape self;

	win_gui& _m_gui;
	xy_point_t left_top(xy_point_t const& p)
	{
		xy_point_t r;
		return r;
	}
	void draw(int row_count,int col_count)
	{
		int rh = single_row_height();
		int cw = single_col_width();
		int y = _m_orgin.y()+_m_margin_size.top();
		for (int r=0;r<row_count;++r,y+=rh)
		{
			int x = _m_orgin.x()+_m_margin_size.left();
			for (int c=0;c<col_count;++c,x+=cw)
			{
				xy_rect_t rect(x,y,_m_unit_size);
				_m_gui.draw_rectangle(rect);
			}
		}
	}
	int single_row_height() const {return _m_unit_size.ys()+_m_unit_between_size.ys();}
	int single_col_width() const {return _m_unit_size.xs()+_m_unit_between_size.xs();}
	xy_point_t _m_orgin;
	xy_size_t _m_unit_size;
	xy_size_t _m_unit_between_size;
	xy_rect_t _m_margin_size;
};


struct tetris_win_gui
{
	win_gui& _m_gui;
	tetris_win_gui(win_gui& gui): _m_gui(gui)
	{}
	void fill_rect(rc_rect_t const& rect,char v)
	{
		
	}
	void draw_point(rc_point_t const& p, char v)
	{
	}
	void draw_text(rc_point_t const& p,char const* str,size_t size)
	{
	}
};

