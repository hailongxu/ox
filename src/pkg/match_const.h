/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 20010-10-24
	版权：版权归作者所有，本文件可以自由传播
 */


#pragma once

namespace ox
{
namespace match
{


struct constant_t
{
	enum match_samemode_c
	{
		__same_raw_mode,
		__same_max_mode,
		__same_min_mode,
	};
	enum match_selective_c
	{
		__sel_raw_mode,
		__sel_leftest_mode,
		__sel_rightest_mode,
	};
};


} // end of match
} // end of ox