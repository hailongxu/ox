
#include <string.h>
#include "tetris_define.h"
#include "win_cui.h"


#pragma once




template <typename tetris_ui_t>
struct draw_view_tt
{
	draw_view_tt(tetris_ui_t& tetris_ui) : _m_tetris_ui(tetris_ui) {}
	rc_point_t _m_origin;
	tetris_ui_t& _m_tetris_ui;
	template <typename cond_tn>
	void draw(data_view_t const& data_view, rc_point_t const& p,cond_tn const& cond)
	{
		rc_size_t size = data_view.size();
		for (int r = 0; r < size.rc(); r++)
			for (int c = 0; c < size.cc(); c++)
			{
				char v = data_view.value(rc_point_t(r, c));
				rc_point_t board_point = p+rc_point_t(r, c);
				if (!cond(v,board_point)) continue;
				_m_tetris_ui().draw_point(_m_origin+board_point, v);
			}
	}
};

template <typename tetris_ui_t>
struct ui_board
{
	struct always_true
	{
		bool operator()(char,rc_point_t const&) const {return true;}
	};
	struct rect_view_t
	{
		typedef rect_view_t self;
		self(rc_size_t const& size,char c) : _m_size(size),_m_value(c) {}
		char _m_value;
		rc_size_t _m_size;
		char get_value(rc_point_t const& p)
		{
			return _m_value;
		}
		rc_size_t get_size()
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
	typedef draw_view_tt<tetris_ui_t> draw_view_t;
	ui_board(tetris_ui_t& tetris_ui) : _m_tetris_ui(tetris_ui),_m_draw_view(_m_tetris_ui) {}
	tetris_ui_t& _m_tetris_ui;
	rc_point_t _m_origin;
	draw_view_t _m_draw_view;
	void init()
	{
		_m_origin.set(2,2);
		_m_draw_view._m_origin = _m_origin;
	}
	void draw_box(data_view_t const& box,rc_point_t const& p)
	{
		_m_draw_view.draw(box,p,[](char v,rc_point_t const& p){return !tetris_define::is_value_null(v)&&p.r()>=0;});
	}
	void draw_board(data_view_t const& box_board)
	{
		draw_view(box_board,rc_point_t());
	}
	void draw_board(rc_rect_t const& rect,char v)
	{
		rc_rect_t r = rect;
		r.p = _m_origin+r.p;
		_m_tetris_ui().fill_rect(r,v);
	}
	void clear_box(data_view_t const& box,rc_point_t const& p)
	{
		clear_view(box,p);
	}
	void clear_board(data_view_t const& box_board)
	{
		clear_view(box_board,rc_point_t());
	}

	void draw_view(data_view_t const& data_view, rc_point_t const& p)
	{
		_m_draw_view.draw(data_view,p,always_true());
	}
	void clear_view(data_view_t const& data_view, rc_point_t const& p)
	{
		_m_tetris_ui().fill_rect(rc_rect_t(_m_origin+p, data_view.size()), '.');
	}
};

template <typename tetris_ui_t>
struct ui_preview
{
	typedef draw_view_tt<tetris_ui_t> draw_view_t;
	ui_preview(tetris_ui_t& tetris_ui): _m_tetris_ui(tetris_ui), _m_draw_view(tetris_ui)
	{}
	tetris_ui_t& _m_tetris_ui;
	rc_rect_t _m_rect;
	draw_view_t _m_draw_view;
	void init()
	{
		_m_rect.p.set(2,14);
		_m_rect.s.set(4,4);
		_m_draw_view._m_origin = _m_rect.p;
	}
	void draw(data_view_t const& box,rc_point_t const& p)
	{
		_m_draw_view.draw(box,p,[](char v,rc_point_t const& p){return true;});
	}
	void clear()
	{
		_m_tetris_ui().fill_rect(_m_rect,tetris_define::back_char());
	}
};

template <typename tetris_ui_t>
struct ui_information
{
	ui_information(tetris_ui_t& tetris_ui): _m_tetris_ui(tetris_ui)
	{}
	tetris_ui_t& _m_tetris_ui;
	rc_rect_t _m_rect;
	void init()
	{
		_m_rect.p.set(18,14);
		_m_rect.s.set(1,10);
	}
	void draw_text(int credit)
	{
		char str [16] ;
		sprintf(str,"%d",credit);
		_m_tetris_ui().draw_text(_m_rect.p,str,strlen(str));
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
