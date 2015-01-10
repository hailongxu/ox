


/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 20010-10-24
	版权：版权归作者所有，本文件可以自由传播
 */



#pragma once


//___namespace2_begin(ox,lg)

namespace lg {


template <bool not_tc>
bool not(bool b)
{
	return not_tc?!b:b;
};


}//___namespace2_end()