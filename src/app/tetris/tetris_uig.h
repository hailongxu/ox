#include "../../ox/nsab.h"

#include "math.h"
#include "win_gui.h"
#include "tetris_define.h"



#pragma once


___namespace3_begin(ox,app,tetris)




struct tetris_win_gui
{
	typedef tetris_win_gui self;
	typedef ox::gui::win_gui win_gui;
	typedef ox::gui::matrix_shape matrix_shape;

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


___namespace3_end()