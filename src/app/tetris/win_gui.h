

#include <windows.h>
#include "math.h"


#pragma once


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
	void set(unsigned char r,unsigned char g,unsigned char b)
	{
		this->r()=r,this->g()=g,this->b()=b;
	}
	void set(unsigned char a,unsigned char r,unsigned char g,unsigned char b)
	{
		this->a()=a,this->r()=r,this->g()=g,this->b()=b;
	}
};

struct win_gui
{
	typedef win_gui self;
	self():_m_hdc(0) {}
	self(HDC hdc) {init(hdc);}
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
	void fill_rect(xy_rect_t const& r,color_t const& c)
	{
		HBRUSH h = CreateSolidBrush(COLORREF(RGB(c.r(),c.g(),c.b())));
		HBRUSH hold = (HBRUSH)SelectObject(_m_hdc,(HGDIOBJ)h);
		RECT rect = {r.left(),r.top(),r.right(),r.bottom()};
		FillRect(_m_hdc,&rect,h);
		SelectObject(_m_hdc,(HGDIOBJ)hold);
		DeleteObject((HGDIOBJ)h);
	}
};
