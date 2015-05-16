
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

#include <new.h>
#include "../cxx/delegate.h"


#pragma once


___namespace2_begin(ox,comm)


struct communication_helper
{
	struct simple_allocator
	{
		template <typename t>
		t* _new()
		{
			t* p =  new (std::nothrow) t;
			return p;
		}
		template <typename t,typename t1>
		t* _new(t1 p1)
		{
			t* p =  new (std::nothrow) t (p1);
			return p;
		}
		char* _new(size_t size)
		{
			return new (std::nothrow) char [size];
		}
		template <typename t>
		void _delete(t* p)
		{
			delete p;
		}
	};
} ; ///end of communication helper


___namespace2_end()