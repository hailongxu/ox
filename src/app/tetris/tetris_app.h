// tetris.cpp : Defines the entry point for the console application.
//

#include <memory.h>
#include <assert.h>
#include "../../ox/nsab.h"
#include "math.h"
#include "tetris_define.h"
#include "tetris_credit_calculate.h"
#include "tetris_view_interface.h"
#include "tetris_vui.h"
#include "tetris_event.h"
#include "tetris.h"




#pragma once



//___namespace3_begin(ox,app,tetris_zone)


template <typename tetris_ui,typename tetris_event>
struct app
{
	typedef app self;
	typedef tetris_ui tetris_ui;
	typedef ui_board<tetris_ui> ui_board;
	typedef ui_preview<tetris_ui> ui_preview;
	typedef ui_information<tetris_ui> ui_information;

	struct box_view_t
	{
		typedef box_view_t self;
		self(box_t const* box) : _m_box(box) {}
		self(box_t const& box) : _m_box(&box) {}
		box_t const* _m_box;
		char get_value(point_t const& p)
		{
			return _m_box->get(p);
		}
		ssize2_t get_size()
		{
			return ssize2_t(_m_box->rc(),_m_box->cc());
		}
		data_view_t as_view()
		{
			data_view_t view;
			view.size.assign(this,&self::get_size);
			view.value.assign(this,&self::get_value);
			return view;
		}
	};
	struct box_view_unit
	{
		box_t const* box;
		box_view_t box_view;
	};
	struct box_view_list
	{
		~box_view_list()
		{
			for(auto i:_m_box_view_list)
			{
				assert(false);
				//delete *i;
			}
		}
		box_view_list(tetris_drive& dr) : _m_drive(dr) {}
		tetris_drive& _m_drive;
		std::vector<box_view_t*> _m_box_view_list;
		void init()
		{
			int n = _m_drive._m_tetris_core_data.boxes().size();
			for(ibox_t index;index.i<n;++index.i)
			{
				for (index.j=0;index.j<4;++index.j)
				{
					box_t const& box2 = _m_drive._m_tetris_core_data.box(index);
					box_view_t* box_view = new box_view_t {box2};
					_m_box_view_list.push_back(box_view);
				}
			}
		}
		data_view_t get(ibox_t i)
		{
			return _m_box_view_list[i.index()]->as_view();
		}
	};
	struct board_part_view
	{
		typedef board_part_view self;
		self(box_board::access_t& access): _m_access(access)
		{}
		rect_t _m_rect;
		box_board::access_t& _m_access;
		rect_t const& rect() const {return _m_rect;}
		void set(rect_t const& rect)
		{
			_m_rect=rect;
			if (_m_rect.p.r<0)
			{
				_m_rect.s.rc += _m_rect.p.r;
				_m_rect.p.r = 0;
			}
		}
		char get_value(point_t const& p)
		{
			return _m_access.get(_m_rect.p+p);
		}
		ssize2_t get_size()
		{
			return _m_rect.s;
		}
		data_view_t as_view()
		{
			data_view_t view;
			view.size.assign(this,&self::get_size);
			view.value.assign(this,&self::get_value);
			return view;
		}
	};
	struct credit_info
	{
		size_t _m_sum = 0;
		void clear() {_m_sum=0;}
	};

	/// the outer element
	tetris_ui* _m_console;
	tetris_event* _m_event_source;

	ui_board _m_ui_board;
	ui_preview _m_ui_preview;
	tetris_drive _m_drive;
	data_view_t _m_board_view;
	box_view_list _m_box_view_list;
	board_part_view _m_board_part_view;
	ibox_t _m_ibox_next;
	ibox_generator _m_ibox_generator;
	ui_information _m_ui_information;
	tetris_credit_calculate _m_credit_calculate;
	credit_info _m_credit_info;

	self(): _m_box_view_list(_m_drive), _m_board_part_view(_m_drive._m_tetris_core_data._m_board.access())
		, _m_ui_board(*_m_console), _m_ui_preview(*_m_console), _m_ui_information(*_m_console)
	{}

	void init()
	{
		_m_console.enable_keyboard_input(true);
		_m_drive.on_init();
		_m_drive.on_trace_changed.assign(this,&self::on_trace_changed);
		_m_drive.on_data_changed.assign(this,&self::on_data_changed);
		_m_drive.on_finished.assign(this,&self::on_finished);
		_m_ui_board.init();
		_m_ui_preview.init();
		_m_ui_information.init();
		_m_board_view = get_board_view();
		_m_box_view_list.init();
		//_m_event_source._m_console = &_m_ui_board._m_console;
		_m_event_source.init();
		_m_event_source.on_start_game.assign(this,&self::on_start_game);
		_m_event_source.on_moved_rotate.assign(this, &self::on_moved_rotate);
		_m_event_source.on_moved_down.assign(this, &self::on_move_down);
		_m_event_source.on_moved_right.assign(this, &self::on_move_right);
		_m_event_source.on_moved_left.assign(this, &self::on_move_left);
		_m_ibox_next.i=_m_ibox_next.j = -1;
		_m_ibox_generator.init(_m_drive._m_tetris_core_data.boxes().size());
		_m_credit_calculate.init();
	}
	/// 0: active, 1...: each rows
	void on_data_changed(rect_t const* invalid,size_t size)
	{
		assert(size>=1);
		on_credit_changed(invalid+1,size-1);
		draw_board();
		start_round();
	}
	void on_trace_changed(box_trace_t const& from,box_trace_t const& to)
	{
		draw_board_part(from.rect);
		draw_box(to.ibox,to.rect.p);
	}
	void on_credit_changed(rect_t const* invalid,size_t size)
	{
		int this_credits = _m_credit_calculate.get(invalid,size);
		_m_credit_info._m_sum += this_credits;
		_m_ui_information.draw_text(_m_credit_info._m_sum);
	}
	void on_finished()
	{
		rect_t user_rect = _m_drive.user_rect();
		for (int r=0;r<user_rect.s.rc;++r)
		{
			Sleep(50);
			_m_ui_board.draw_board(rect_t(r,0,1,user_rect.s.cc),tetris_define::front_char());
		}
		Sleep(100);
		for (int r=0;r<user_rect.s.rc;++r)
		{
			Sleep(50);
			_m_ui_board.draw_board(rect_t(r,0,1,user_rect.s.cc),tetris_define::back_char());
		}
		clear_preview();
		clear_credit();
	}
	void on_start_game()
	{
		_m_drive.start_game();
		retrieve_ibox_next();
		clear_preview();
		draw_active_in_preview();
		start_round();
		clear_credit();
	}
	void on_move_down()
	{
		_m_drive.move_down();
	}
	void on_move_left()
	{
		_m_drive.move_left();
	}
	void on_move_right()
	{
		_m_drive.move_right();
	}
	void on_moved_rotate()
	{
		_m_drive.move_rotate();
	}
	void on_quit()
	{
		_m_event_source.start();
	}
	void start()
	{
		_m_event_source.start();
		draw_board();
		clear_preview();
	}
	void start_round()
	{
		_m_drive.set_ibox_current(_m_ibox_next);
		retrieve_ibox_next();
		clear_preview();
		draw_active_in_preview();
	}
	void retrieve_ibox_next()
	{
		_m_ibox_next = _m_ibox_generator();
	}
	void draw_board()
	{
		_m_ui_board.draw_board(_m_board_view);
	}
	void draw_board_part(rect_t const& rect)
	{
		/// the rect maybe chipped by the user rect border
		_m_board_part_view.set(rect);
		_m_ui_board.draw_view(_m_board_part_view.as_view(),_m_board_part_view.rect().p);
	}
	void draw_box(ibox_t const& ibox,point_t const& p)
	{
		data_view_t view = _m_box_view_list.get(ibox);
		_m_ui_board.draw_box(view,p);
	}
	void clear_preview()
	{
		_m_ui_preview.clear();
	}
	void draw_active_in_preview()
	{
		data_view_t view = _m_box_view_list.get(_m_ibox_next);
		_m_ui_preview.draw(view,point_t(0,0));
	}
	void draw_active_box()
	{
		draw_box(_m_drive.active_ibox(),_m_drive.active_point());
	}
	void clear_credit()
	{
		_m_credit_info.clear();
		on_credit_changed(0,0);
	}
	data_view_t get_board_view()
	{
		data_view_t view;
		view.size.assign(this,&self::get_board_size);
		view.value.assign(this,&self::get_board_value);
		return view;
	}
	char get_board_value(point_t const& p)
	{
		return _m_drive._m_tetris_core_data._m_board.access().get(p);
	}
	ssize2_t get_board_size()
	{
		return _m_drive._m_tetris_core_data._m_board.user_rect().rc().rect.s;
	}
};




//___namespace3_end()