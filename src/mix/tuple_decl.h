
/**
	2010-11-10
	���ߣ��캣��
	����һ�����Ӽ򵥵ĸ��������loki�µ�typelist
	����������ԭ����mixture������ԭ�����Ǹ�mixture�ǻ��������͵ģ��ܸ��ӣ��������
	����������д��һ������򵥵ĵ��������Ƚ�����
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"


#pragma once


___namespace2_begin(ox,mix)



/**
	tuple, a different type list
*/



template
<
	typename t1=void,
	typename t2=void,
	typename t3=void,
	typename t4=void,
	typename t5=void,
	typename t6=void,
	typename t7=void,
	typename t8=void
>
struct tuple;


template <size_t count>
struct tup;





___namespace2_end()