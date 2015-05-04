

#include <conio.h>
#include <Windows.h>
#include "math.h"



#pragma once



struct win_console
{
	typedef win_console self;

	HANDLE _m_hout;
	HANDLE _m_hin;
	~win_console()
	{
		CloseHandle(_m_hout);
		CloseHandle(_m_hin);
	}
	self()
	{
		_m_hout = GetStdHandle(STD_OUTPUT_HANDLE);
		_m_hin = GetStdHandle(STD_INPUT_HANDLE);
		assert(_m_hin != INVALID_HANDLE_VALUE);
		assert(_m_hout != INVALID_HANDLE_VALUE);
	}
	void draw_box(rc_rect_t const& r, char c)
	{
		if (r.height() != 0)
		{
			draw_hor(r.left_top(), r.width(), c);
			draw_hor(rc_point_t(r.p.r()+r.height()-1,r.p.c()), r.width(), c);
		}
		if (r.width() != 0)
		{
			draw_ver(r.left_top(), r.height(), c);
			draw_ver(rc_point_t(r.p.r(),r.p.c()+r.width()-1), r.height(), c);
		}
	}
	void draw_hor(rc_point_t const& p, size_t len, char c)
	{
		for (int i = 0; i < len;++i)
			WriteConsoleOutputCharacterA(_m_hout, &c, 1, COORD{ p.c()+i, p.r() }, 0);
	}
	void draw_ver(rc_point_t const& p, size_t len, char c)
	{
		size_t bottom = p.r() + len;
		for (size_t r=p.r(); r < bottom;++r)
			WriteConsoleOutputCharacterA(_m_hout, &c, 1, COORD{ p.c(),r }, 0);
	}
	void fill_rect(rc_rect_t const& rect, char c)
	{
		size_t bottom = rect.p.r() + rect.height();
		for (size_t r = rect.p.r(); r < bottom; ++r)
			draw_hor(rc_point_t(r, rect.p.c()), rect.width(), c);
	}
	void draw_point(rc_point_t const& p, char c)
	{
		DWORD l;
		WriteConsoleOutputCharacterA(_m_hout, &c, 1, COORD{p.c(),p.r()}, &l);
	}
	void draw_text(rc_point_t const& p,char const* str,int size)
	{
		WriteConsoleOutputCharacterA(_m_hout, str, size, COORD{p.c(),p.r()}, 0);
	}
	void diable_except_keyboard(bool b)
	{

	}
	void enable_keyboard_input(bool b)
	{
		DWORD mode;
		BOOL br = GetConsoleMode(_m_hin, &mode);
		assert(br);
		if (b) mode |= ENABLE_WINDOW_INPUT; else mode &= (~ENABLE_WINDOW_INPUT);
		br = SetConsoleMode(_m_hin, mode);
		assert(br);
	}
	enum event_type_enum
	{
		__mouse_event=1,
		__keyboard_event
	};
	void read_keyboard_input(size_t& c,bool& isdown)
	{
		INPUT_RECORD keyrec;
		DWORD state = 0, res;
		while (1)
		{
			ReadConsoleInput(_m_hin,&keyrec,1,&res);
			if (keyrec.EventType != KEY_EVENT) continue;
			c = keyrec.Event.KeyEvent.wVirtualKeyCode;
			isdown = keyrec.Event.KeyEvent.bKeyDown?true:false;
			break;
		}
	}
	void peek_keyboard_input(size_t& c)
	{
		INPUT_RECORD keyrec;
		DWORD state = 0, res;
		PeekConsoleInput(_m_hin, &keyrec, 1, &res);
		assert(keyrec.EventType == KEY_EVENT);
		c = keyrec.Event.KeyEvent.wVirtualKeyCode;
	}
};
