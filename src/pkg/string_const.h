/*	
	����: shell_kit
	����: �캣��
	����: 2007-5-20
	�ļ���: string_const.h
	��Ȩ����Ȩ���������У����ļ��������ɴ���
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