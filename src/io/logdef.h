


/*
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	时间     :  2008-1-14
*/


#pragma once



namespace ox
{
	namespace io
	{
		enum logpretext_enum
		{
			__logpretext_null = 0, /// nothing 
			__logpretext_date = 1,
			__logpretext_time = 2,
			__logpretext_compile_time = 4,
			__logpretext_threadid = 8, /// no using
			__logpretext_ignore = 16,
			__logpretext_datetime = 3,
			__logpretext_all = 15,
		};
	}
}