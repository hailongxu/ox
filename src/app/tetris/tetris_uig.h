
#include "math.h"
#include "win_gui.h"
#include "tetris_define.h"



#pragma once


struct matrix_shape
{
	typedef matrix_shape self;

	self(win_gui& wgui) : _m_gui(wgui)
	{
		_m_unit_size.set(20,20);
		_m_unit_between_size.set(2,2);
	}

	win_gui& _m_gui;
	xy_point_t left_top(xy_point_t const& p)
	{
		xy_point_t r;
		return r;
	}
	void draw(rc_rect_t const& rect)
	{
		int rh = single_row_height();
		int cw = single_col_width();
		int y = _m_orgin.y()+_m_margin_size.top();
		int top = rect.top();
		int bottom = rect.bottom();
		int left = rect.left();
		int right = rect.right();
		for (int r=top;r<bottom;++r,y+=rh)
		{
			int x = _m_orgin.x()+_m_margin_size.left();
			for (int c=rect.left();c<rect.right();++c,x+=cw)
			{
				xy_rect_t r(x,y,_m_unit_size);
				_m_gui.draw_rectangle(r);
			}
		}
	}
	void draw(rc_point_t const& p)
	{
		xy_point_t xy = get_point(p);
		xy_rect_t rect(xy,_m_unit_size);
		_m_gui.draw_rectangle(rect);
	}
	void fill(rc_point_t const& p,color_t const& c)
	{
		_m_gui.fill_rect(get_rect(p),c);
	}
	xy_rect_t get_rect(rc_point_t const& p) const
	{
		return xy_rect_t(get_point(p),_m_unit_size);
	}
	xy_point_t get_point(rc_point_t const& p) const
	{
		xy_piont_t xy;
		xy.x() = _m_orgin.x()+_m_margin_size.left()+p.c()*single_col_width();
		xy.y() = _m_orgin.y()+_m_margin_size.top()+p.r()*single_row_height();
		return xy;
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
	typedef tetris_win_gui self;
	win_gui& _m_gui;
	matrix_shape _m_matrix_shape;
	self(win_gui& gui): _m_gui(gui), _m_matrix_shape(_m_gui)
	{
		_m_matrix_shape._m_orgin.set(30,30);
	}
	void fill_rect(rc_rect_t const& rect,char v)
	{
		_m_matrix_shape.draw(rect);
	}
	void draw_point(rc_point_t const& p, char v)
	{
		_m_matrix_shape.draw(p);
		if (tetris_define::is_value_null(v)) return;
		_m_matrix_shape.fill(p,get_color(v));
	}
	void draw_text(rc_point_t const& p,char const* str,size_t size)
	{
		xy_point_t pos = _m_matrix_shape.get_point(p);
		_m_gui.draw_text(str,size,pos);
	}
	static color_t get_color(char v)
	{
		if (tetris_define::is_value_null(v))
			return color_t::white();
		return color_t::black();
	}
};

