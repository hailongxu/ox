

#include <windows.h>
#include "math.h"


#pragma once


struct win_gui
{
	void init(HDC hdc) {_m_hdc=hdc;}
	HDC _m_hdc;
	void draw_rectangle(xy_rect_t const& rect)
	{
		Rectangle(_m_hdc,rect.left(),rect.top(),rect.right(),rect.bottom());
	}
	void draw_line(xy_point_t const& from,xy_point_t const& to)
	{
		MoveToEx(_m_hdc,from.x(),from.y(),0);
		LineTo(_m_hdc,to.x(),to.y());
	}
};
