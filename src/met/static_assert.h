/**
	��̬����
*/

#include "../ox/nsab.h"
#include "./mod.h"

#pragma once


___namespace2_begin(ox,met)

/**
	�����ڶ���
 */


/**
	��������չ��ģ��ʱ����������Ƿ���ڵ�������ԭ���ǷǱ�Ҫ��������飬
	����ֻ�Ƕ��壬����������Ͷ��壬Ҳû����ô����ʹ�ã��������ǲ��ᱨ��ģ�
	����������������С����ʱ���룬��֪�������Ƿ���ڣ�������ô���С��
	���ԣ������ڶ��Ծ��ǽ�����������Խ��еģ�boost��loki���������õ�
 */
namespace inside
{
	template <int> struct compile_error;
	template <> struct compile_error<true> {};
	template <int> struct static_check_tt {};
	template <bool> struct type_size_ok_tt;
	template <> struct type_size_ok_tt<true>{};
}

#define ___static_assert_message(expr, msg) \
	{ ox::met::inside::compile_error<((expr) != 0)> error_##msg; (void)error_##msg;} 
/// ��̬���Ժ궨��
#define ___static_assert(b) \
	typedef ox::met::inside::static_check_tt< \
		sizeof(ox::met::inside::type_size_ok_tt<b>) \
	> \
	___macro_internal_ox_type_typedef_compile_static_assert_global_macro__2010_11_06

/**
	�궨���׳��ֵĴ���
	#define A(x) x;
	#define B(x,y) x+y
	A(B(1,2)) ; /// error, why? ��ΪA��B(1�����˵�һ������
*/
#define ___static_assert_same_type(type_from_tn, type_to_tn) \
	___static_assert((ox::met::is_same<type_from_tn,type_to_tn>::value))

#define ___static_desert_same_type(type_from_tn, type_to_tn) \
	___static_assert(!(ox::met::is_same<type_from_tn,type_to_tn>::value))


___namespace2_end()