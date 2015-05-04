#include "../ox/nsab.h"
#include "../mam/math.h"


#pragma once


___namespace2_begin(ox,gui)


struct matrix_shape
{
	typedef matrix_shape self;
	typedef ox::mam::rc_point_t rc_point_t;
	typedef ox::mam::rc_rect_t rc_rect_t;
	typedef ox::mam::xy_size_t xy_size_t;

	self(win_gui_draw& wgui) : _m_gui(wgui)
	{
		_m_unit_size.set(20,20);
		_m_unit_between_size.set(2,2);
	}

	win_gui_draw& _m_gui;
	xy_point_t left_top(xy_point_t const& p)
	{
		xy_point_t r;
		return r;
	}
	void draw(rc_rect_t const& rect)
	{
		int rh = single_row_height();
		int cw = single_col_width();
		int top = rect.top();
		int bottom = rect.bottom();
		int left = rect.left();
		int right = rect.right();
		int y = _m_orgin.y()+_m_margin_size.top()+top*single_row_height();
		int x0 = _m_orgin.x()+_m_margin_size.left()+left*single_col_width();
		for (int r=top;r<bottom;++r,y+=rh)
		{
			int x = x0;
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
		xy_point_t xy;
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


___namespace2_end()