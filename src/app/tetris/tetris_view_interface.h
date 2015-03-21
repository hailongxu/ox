


#include "../../src/cxx/delegate.h"

#pragma once

struct data_view_t
{
	typedef delegate<char(point_t const&)> get_value_d;
	typedef delegate<ssize2_t()> get_size_d;
	get_value_d value;
	get_size_d size;
};
