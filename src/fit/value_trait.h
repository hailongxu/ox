
#include <assert.h>
#include "../ox/nsab.h"



#pragma once



___namespace2_begin(ox,fit)



template <typename value_tn>
struct value_trait
{
	typedef value_tn value_type;
	typedef value_type* value_pointer;
	struct value_size
	{
		size_t operator()(value_type const& value)
		{
			return sizeof(value);
		}
	};
	struct value_construct
	{
		template <typename t1>
		void operator()(void* begin,t1& p1)
		{
			new (begin) value_type (p1);
		}
	};
};



___namespace2_end()