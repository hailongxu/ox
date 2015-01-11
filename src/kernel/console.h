
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hsuhailong@baidu.com

	redirect the stdout
 */


#include <stdio.h>
#include <windows.h>


#pragma once


struct console
{
	console()
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}
	~console()
	{
		FreeConsole();
	}
};


//// console _g_console
//// printf(); will output str to console window