
#include <string.h>
#include "tetris_define.h"
#include "win_console.h"


#pragma once



struct draw_view_t
{
	draw_view_t(win_console& console) : _m_console(console) {}
	point_t _m_origin;
	win_console& _m_console;
	template <typename cond_tn>
	void draw(data_view_t const& data_view, point_t const& p,cond_tn const& cond)
	{
		ssize2_t size = data_view.size();
		for (int r = 0; r < size.rc; r++)
			for (int c = 0; c < size.cc; c++)
			{
				char v = data_view.value(point_t(r, c));
				point_t board_point = p+point_t(r, c);
				if (!cond(v,board_point)) continue;
				_m_console.draw_point(_m_origin+board_point, v);
			}
	}
};

struct ui_board
{
	struct always_true
	{
		bool operator()(char,point_t const&) const {return true;}
	};
	struct rect_view_t
	{
		typedef rect_view_t self;
		self(ssize2_t const& size,char c) : _m_size(size),_m_value(c) {}
		char _m_value;
		ssize2_t _m_size;
		char get_value(point_t const& p)
		{
			return _m_value;
		}
		ssize2_t get_size()
		{
			return _m_size;
		}
		data_view_t as_view()
		{
			data_view_t view;
			view.size.assign(this,&self::get_size);
			view.value.assign(this,&self::get_value);
			return view;
		}
	};
	ui_board(win_console& console) : _m_console(console),_m_draw_view(_m_console) {}
	win_console& _m_console;
	point_t _m_origin;
	draw_view_t _m_draw_view;
	void init()
	{
		_m_origin.set(2,2);
		_m_draw_view._m_origin = _m_origin;
	}
	void draw_box(data_view_t const& box,point_t const& p)
	{
		_m_draw_view.draw(box,p,[](char v,point_t const& p){return !tetris_define::is_value_null(v)&&p.r>=0;});
	}
	void draw_board(data_view_t const& box_board)
	{
		draw_view(box_board,point_t());
	}
	void draw_board(rect_t const& rect,char v)
	{
		rect_t r = rect;
		r.p = _m_origin+r.p;
		_m_console.fill_rect(r,v);
	}
	void clear_box(data_view_t const& box,point_t const& p)
	{
		clear_view(box,p);
	}
	void clear_board(data_view_t const& box_board)
	{
		clear_view(box_board,point_t());
	}

	void draw_view(data_view_t const& data_view, point_t const& p)
	{
		_m_draw_view.draw(data_view,p,always_true());
	}
	void clear_view(data_view_t const& data_view, point_t const& p)
	{
		_m_console.fill_rect(rect_t(_m_origin+p, data_view.size()), '.');
	}
};

struct ui_preview
{
	ui_preview(win_console& console): _m_console(console), _m_draw_view(console)
	{}
	win_console& _m_console;
	rect_t _m_rect;
	draw_view_t _m_draw_view;
	void init()
	{
		_m_rect.p.set(2,14);
		_m_rect.s.set(4,4);
		_m_draw_view._m_origin = _m_rect.p;
	}
	void draw(data_view_t const& box,point_t const& p)
	{
		_m_draw_view.draw(box,p,[](char v,point_t const& p){return true;});
	}
	void clear()
	{
		_m_console.fill_rect(_m_rect,tetris_define::back_char());
	}
};

struct ui_information
{
	ui_information(win_console& console): _m_console(console)
	{}
	win_console& _m_console;
	rect_t _m_rect;
	void init()
	{
		_m_rect.p.set(18,14);
		_m_rect.s.set(1,10);
	}
	void draw_text(int credit)
	{
		char str [16] ;
		sprintf(str,"%d",credit);
		_m_console.draw_text(_m_rect.p,str,strlen(str));
	}
};


//struct ui_drive
//{
//	ui_board _m_ui_board;
//	input_event_source _m_event_source;
//	void init()
//	{
//		_m_ui_board.init();
//	}
//	void start()
//	{
//		//_m_ui
//	}
//	void on_move_down()
//	{
//
//	}
//};
