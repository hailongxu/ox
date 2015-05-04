// tetris.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <string>
template <typename obj>
std::string to_string(obj const& b)
{
	std::string s;
	for (size_t i=0;i<b.height();++i)
	{
		for (size_t j=0;j<b.width();++j)
		{
			s.push_back(b.get(i,j));
		}
		s.push_back('\n');
	}
	return s;
}


#include "../../src/mos/win_queue_thread.h"
#include "../../src/app/tetris/win_cui.h"
#include "../../src/app/tetris/tetris_uic.h"
#include "../../src/app/tetris/tetris_uic_event_source.h"
#include "../../src/app/tetris/tetris_app.h"

using ox::app::tetris::tetris_uic_input_event_source;
using ox::app::tetris::tetris_win_cui;

struct on_application_start
{
	on_application_start(tetris_uic_input_event_source& tetris_event)
		: tetris_event(tetris_event)
	{}
	tetris_uic_input_event_source& tetris_event;
	void operator()() {tetris_event.start();}
};
int _tmain(int argc, _TCHAR* argv[])
{
	win_console console;
	tetris_win_cui tetris_cui(console);
	tetris_uic_input_event_source tetris_event(console);
	ox::app::tetris::app<tetris_win_cui,tetris_uic_input_event_source>  tetris_application;
	console.enable_keyboard_input(true);
	tetris_application.init(&tetris_cui,&tetris_event);
	tetris_application.on_started.assign(&on_application_start(tetris_event));
	tetris_application.start();
	ox::mos::win_queue_thread th;
	th.start_here();
	getchar();
	return 0;
}

