
#include <time.h>
#include "../ox/nsab.h"


#pragma once


___namespace2_begin(ox,mam)


struct random
{
	void init()
	{
		srand(time(0));
	}
	int get() const
	{
		return rand();
	}
};


___namespace2_end()
