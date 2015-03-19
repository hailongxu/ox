
#include "../ox/nsab.h"



#pragma once


___namespace_begin(ox,utl)

template <typename t>
struct intv_tt
{
	typedef intv_tt self;
	typedef t iterator;
	self(){}
	self(iterator const& begin,iterator end)
		: begin(begin),end(end)
	{}
	self& set(iterator const& begin,iterator const& end)
	{
		this->begin = begin;
		this->end = end;
	}
	self& set_begin(iterator const& begin)
	{
		this->begin = begin;
	}
	self& set_end(iterator const& end)
	{
		this->end = end;
	}
	iterator begin;
	iterator end;
};


___namespace_end()