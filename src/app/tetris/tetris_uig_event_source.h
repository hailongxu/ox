

#include <stdio.h>
#include "../../ox/nsab.h"
#include "tetris_event_define.h"


#pragma once


___namespace3_begin(ox,app,tetris)


struct tetris_uig_input_event_source
{
	typedef tetris_uig_input_event_source self;

	static size_t const __arrow_left = 37;
	static size_t const __arrow_up = 38;
	static size_t const __arrow_right = 39;
	static size_t const __arrow_down = 40;
	static size_t const __start_game_s = 's';
	static size_t const __start_game_S = 'S';
	static size_t const __char_q = 'q';
	static size_t const __char_Q = 'Q';

	
	action_d on_start_game;
	action_d on_moved_left;
	action_d on_moved_rotate;
	action_d on_moved_right;
	action_d on_moved_down;
	action_d on_quit;

	//self(win_console& console): _m_console(&console)
	//{}
	void start() {}
	void on_keydown_from_system(int vkey)
	{
		switch (vkey)
		{
		case __arrow_left: if (!on_moved_left.is_empty()) on_moved_left(); break;
		case __arrow_up: if (!on_moved_rotate.is_empty()) on_moved_rotate(); break;
		case __arrow_right: if (!on_moved_right.is_empty()) on_moved_right(); break;
		case __arrow_down: if (!on_moved_down.is_empty()) on_moved_down(); break;
		case __start_game_s: case __start_game_S: if (!on_start_game.is_empty()) on_start_game(); break;
		case __char_q: case __char_Q: if (!on_quit.is_empty()) on_quit(); break;
		}
	}
	void stop()
	{
		//ungetch('q');
	}
};


___namespace3_end()