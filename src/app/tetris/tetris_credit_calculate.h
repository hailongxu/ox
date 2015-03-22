

#include "math.h"



#pragma once


struct tetris_credit_calculate
{
	int _m_credits_by_rows[5];

	void init()
	{
		_m_credits_by_rows[0] = 0;
		_m_credits_by_rows[1] = 1;
		_m_credits_by_rows[2] = 3;
		_m_credits_by_rows[3] = 6;
		_m_credits_by_rows[4] = 10;
	}
	int get(rect_t const* row_intv,size_t size)
	{
		int count = get_row_count(row_intv,size);
		assert(count>=0 && count<=4);
		return _m_credits_by_rows[count];
	}
	static int get_row_count(rect_t const* row_intv,size_t size)
	{
		int count = 0;
		for (int i=0;i<size;++i)
			count += (row_intv[i].s.rc);
		return count;
	}
};