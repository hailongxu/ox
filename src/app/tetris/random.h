

#include <time.h>

#pragma once

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
