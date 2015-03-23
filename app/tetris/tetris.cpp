// tetris.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/thread/win_queue_thread.h"
#include "../../src/app/tetris/tetris_app.h"

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


#include "../../src/app/tetris/tetris_api_cui.h"
#include "../../src/app/tetris/tetris_event.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/*ox::app::tetris_zone::*/app<tetris_win_cui,input_event_source>  _l_app;
	_l_app.init();
	_l_app.start();
	ox::win_queue_thread th;
	th.start_here();
	getchar();
	return 0;
}

