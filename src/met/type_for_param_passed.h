/**
	������ת����ѵĲ������ݵ�����
*/

#include "../ox/nsab.h"
#include "../ox/tclasses.h"

#include "./_if_switch.h"
#include "./mod.h"



#pragma once

___namespace2_begin(ox,met)


/**
	@brief	�Բ�������ֻ������
	����ǻ������ͣ�Ҫ����const & ����
	����������ͣ�Ҫ����const & ����
	���������ָ�룺�Ͳ�Ҫ�ٽ���&���Σ�ֱ�Ӽ�const
*/
template <typename type_tn>
struct param_readonly_type
{
private:
	typedef typename rmv_ref<type_tn>::type removed_ref_type;
public:
	typedef typename
		ox::met::iff
		<
			is_sptr<removed_ref_type>::value,
			add_con<removed_ref_type>,
			add_ref<typename add_con<removed_ref_type>::type>
		>
		::type
	outer_type;
	typedef typename outer_type::type type;
};

/**
	@brief	�Բ������п�д����
	����ǻ������ͣ�Ҫ���� & ����
	����������ͣ�Ҫ���� & ����
*/
template <typename type_tn>
struct param_write_type
{
	typedef
		typename add_ref<
			typename rmv_con <
				typename rmv_ref<type_tn>
				::type
			>::type
		>::type
	type;
};

/**
	�����&���ͣ���������ԭ������
	����ж���ֵ���ݣ��ѱ�ɶ�������
	����ǻ�������ֵ���ݣ����ֵ����
*/
template <typename type_tn>
struct param_auto_type
{
	typedef typename
		iff
		<
			is_con<type_tn>::value,
			param_readonly_type<type_tn>,
			param_write_type<type_tn>
		>
		::type
	outer_type;
	typedef typename outer_type::type type;
};

/**
	��ֵ����ֵ��������һ��������
	�����ֵ�Ƕ���ֵ���ͣ��ѱ�ɶ��������ã��������β���
	һ�����ڣ�����ʽ����ȥ��ʼ��һ�����Ա�ڲ�������
	�Ǹ������������ʽ����Ӧ����ʲô�����أ�����������������������
	�洢����     ������ʼ����������
	T*           T*const
	T&           T&
	class        class const&
	num          nun const
*/
template <typename type_tn>
struct param_type_for_init
{
	typedef typename add_vcr<int const&>::to<type_tn>::type type;
	//typedef typename add_vcr<int const&>::to<type_tn>::type type;
	//typedef
	//	typename iff
	//	<
	//		is_class<type_tn>::value,
	//		add_vcr<int const&>::to<type_tn>,
	//	typename iff
	//	<
	//		is_value<type_tn>::value && !is_sptr<type_tn>::value,
	//		add_con<type_tn>,
	//		identity<typename add_con<type_tn>::type>
	//	>
	//	::type>::type
	//	::type
	//type;
};
template <typename type_tn>
struct param_type_for_init <type_tn&>
{
	typedef type_tn& type;
};


/**
	Ŀ�ģ���һ�������ԭ��ת��������Ҫ����ԭ�������������������������ǲ��ܱ��
		��һ�������������ȴ洢������Ȼ�����Ժ��ʱ�����ԭ���������������
		һ�����ڣ�����ʽ����ȥ��ʼ��һ�����Ա�ڲ�����

	��������Ҫ��ԭ������Ӧ�ó��ϱ�����ͬ�����ԣ�������Ӧ�ó��ϱ��ֵȼ���

	ԭ����     �洢����     ȡ��������  ��ʼ��������ʽ��������
	T&    =>   T&           T&          T&
	T*    =>   T*           T*          T*
	class =>   class&       class&      class&
	num   =>   num          num&        num const&

	num ΪC++�Ļ������ͣ�int,long,...��������enum
	����û�п��Ǻ������ͺ���������
*/
template <typename type_tn>
struct store_type_for_forward
{
	typedef typename
		iff
		<
			is_class<type_tn>::value,
			add_ref<type_tn>,
			identity<type_tn>
		>
		::type::type
	type;
};

template <typename type_tn>
struct return_type_for_forward
{
	//typedef typename
	//	iff
	//	<
	//		is_class<type_tn>::value || is_num<type_tn>::value,
	//		add_ref<type_tn>,
	//		identity<type_tn>
	//	>
	//	::type
	//outer_type;
	//typedef typename outer_type::type type;
	typedef typename add_ref<type_tn>::type type;
};


___namespace2_end()