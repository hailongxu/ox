//
//////////////////////////////////////////////////////////////////////////
// file : simple_allocator.h
// author : xuhailong
// date: 2007-7-30
// history: 

#include "../ox/nsab.h"

#pragma once


___namespace_begin(ox,cxx)


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


___namespace_end()
