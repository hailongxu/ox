

#include <windows.h>
#include <string.h>
#include "../ox/nsab.h"
#include "../alg/bit_stat.h"
#include "gui_define.h"


#pragma once


___namespace2_begin(ox,gui)


struct color_t
{
	color_t() {data[0]=data[1]=data[2]=data[3]=0;}
	color_t(unsigned char r,unsigned char g,unsigned char b) {set(r,g,b);}
	color_t(unsigned char a, unsigned char r,unsigned char g,unsigned char b) {set(a,r,g,b);}
	unsigned char data[4];
	unsigned char a() const {return data[0];}
	unsigned char r() const {return data[1];}
	unsigned char g() const {return data[2];}
	unsigned char b() const {return data[3];}
	unsigned char& a() {return data[0];}
	unsigned char& r() {return data[1];}
	unsigned char& g() {return data[2];}
	unsigned char& b() {return data[3];}
	static color_t white() {return color_t(255,255,255);}
	static color_t black() {return color_t(0,0,0);}
	void set(unsigned char r,unsigned char g,unsigned char b)
	{
		this->r()=r,this->g()=g,this->b()=b;
	}
	void set(unsigned char a,unsigned char r,unsigned char g,unsigned char b)
	{
		this->a()=a,this->r()=r,this->g()=g,this->b()=b;
	}
};

enum struct text_align_enum
{
	__top = 1,
	__bottom = 2,
	__left = 4,
	__right = 8,
	__center = 16,
};

struct win_gui_helper
{
	static RECT to_rect(xy_rect_t const& rect)
	{
		return RECT{rect.left(),rect.top(),rect.right(),rect.bottom()};
	}
	static size_t to_align(text_align_enum align)
	{
		static size_t win_aligns[5] =
		{
			DT_TOP,
			DT_BOTTOM,
			DT_LEFT,
			DT_RIGHT,
			DT_CENTER,
		};
		int i = ox::alg::get_1index_of_2n((unsigned int)align);
		return win_aligns[i];
	}
};

struct win_gui_draw
{
	typedef win_gui_draw self;

	self():_m_hdc(0) {}
	self(HDC hdc) {init(hdc);}
	void init(HDC hdc) {_m_hdc=hdc;}
	HDC _m_hdc;
	void draw_rectangle(xy_rect_t const& rect) const
	{
		Rectangle(_m_hdc,rect.left(),rect.top(),rect.right(),rect.bottom());
	}
	void draw_line(xy_point_t const& from,xy_point_t const& to)
	{
		MoveToEx(_m_hdc,from.x(),from.y(),0);
		LineTo(_m_hdc,to.x(),to.y());
	}
	void fill_rect(xy_rect_t const& r,color_t const& c)
	{
		HBRUSH h = CreateSolidBrush(COLORREF(RGB(c.r(),c.g(),c.b())));
		HBRUSH hold = (HBRUSH)SelectObject(_m_hdc,(HGDIOBJ)h);
		RECT rect = {r.left(),r.top(),r.right(),r.bottom()};
		FillRect(_m_hdc,&rect,h);
		SelectObject(_m_hdc,(HGDIOBJ)hold);
		DeleteObject((HGDIOBJ)h);
	}
	void draw_text(char const* str,size_t str_len,xy_rect_t const& rect,text_align_enum align)
	{
		DrawTextA(_m_hdc,str,str_len,&win_gui_helper::to_rect(rect),win_gui_helper::to_align(align));
	}
	void draw_text(char const* str,size_t str_len,xy_point_t const& p) const
	{
		TextOutA(_m_hdc,p.x(),p.y(),str,str_len);
	}
	void draw_text(char const* str,xy_point_t const& p) const
	{
		draw_text(str,strlen(str),p);
	}
};

typedef win_gui_draw drawer_t;


___namespace2_end()