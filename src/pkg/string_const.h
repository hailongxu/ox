/*	
	类名: shell_kit
	作者: 徐海龙
	日期: 2007-5-20
	文件名: string_const.h
	版权：版权归作者所有，本文件可以自由传播
 */

#pragma once

namespace ox
{
namespace str
{


struct constant_t
{
	enum string_mode_c
	{
		__string_null = 0,
		__string_size = 1,
		__string_interval = 2,
	};
	enum string_compare_n_policy_c
	{
		__string_policy_normal,
		__string_policy_former,
		__string_policy_latter,
		__string_policy_interval,
		__string_policy_null,
	};
};


} // end of match
} // end of ox