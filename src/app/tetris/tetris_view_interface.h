


#include "../../ox/nsab.h"
#include "../../src/cxx/delegate.h"
#include "math.h"

#pragma once



___namespace3_begin(ox,app,tetris)


struct data_view_t
{
	typedef delegate<char(rc_point_t const&)> get_value_d;
	typedef delegate<rc_size_t()> get_size_d;
	get_value_d value;
	get_size_d size;
};


___namespace3_end()