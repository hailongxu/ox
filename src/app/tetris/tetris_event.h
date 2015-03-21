

#include "../../src/cxx/delegate.h"
#include "../../src/thread/win_thread.h"


#pragma once


struct input_event_source
{
	typedef input_event_source self;
	typedef delegate<void()> action_d;
	typedef ox::win_thread<unsigned()> thread_t;
	
	action_d on_start_game;
	action_d on_moved_left;
	action_d on_moved_rotate;
	action_d on_moved_right;
	action_d on_moved_down;
	action_d on_quit;
	thread_t _m_thread;
	win_console* _m_console;
	void init() {}
	void start()
	{
		_m_thread.on_run().assign(this,&self::read_keyboard);
		_m_thread.start();
	}
	void stop()
	{
		ungetch('q');
	}
	static size_t const __arrow_left = 37;
	static size_t const __arrow_up = 38;
	static size_t const __arrow_right = 39;
	static size_t const __arrow_down = 40;
	static size_t const __start_game_s = 's';
	static size_t const __start_game_S = 'S';
	static size_t const __char_q = 'q';
	static size_t const __char_Q = 'Q';

	unsigned read_keyboard()
	{
		bool bquit = false;
		while (!bquit)
		{
			size_t c; bool isdown;
			_m_console->read_keyboard_input(c,isdown);
			if (!isdown) continue;
			switch (c)
			{
			case __arrow_left: if (!on_moved_left.is_empty()) on_moved_left(); break;
			case __arrow_up: if (!on_moved_rotate.is_empty()) on_moved_rotate(); break;
			case __arrow_right: if (!on_moved_right.is_empty()) on_moved_right(); break;
			case __arrow_down: if (!on_moved_down.is_empty()) on_moved_down(); break;
			case __start_game_s: case __start_game_S: if (!on_start_game.is_empty()) on_start_game(); break;
			case __char_q: case __char_Q: bquit = true; if (!on_quit.is_empty()) on_quit(); break;
			}
		}
		return 0;
	}
};
