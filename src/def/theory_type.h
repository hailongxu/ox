
/************************************************************************/
/*	filename : theory.h
	---------------------------------------------------------------------
	copyright: you can freely copy and reuse and distribute the code, 
	          but should add the head comment in front of your file.
	---------------------------------------------------------------------
	Author: xuhailong
	date: 2007.12.4
	support lib: N/A
	---------------------------------------------------------------------
	Desp: Ϊ����ģ���ṩͨ�����Ͷ���
	history: 
/************************************************************************/

#pragma once

namespace ox
{
	/**
		����Ԫ���ͷ��࣬������������ģ���㷨ʱ���ܲ���������
		__met_type : ��ʾ����
		__met_number : ��ʾ������
	*/
	enum met_type_c
	{
		__met_type = 1,
		__met_number = 2,
	};

namespace theory
{

	///
	/// �������ͳ������Ͷ���
	enum polarity_c
	{
		/// ����
		__positive = 1,
		/// ����
		__negative = -1,
	};

	///
	///// ��̬���Գ������Ͷ���
	//enum tri_polarity_c
	//{
	//	/// ����
	//	__positive = 1,
	//	/// ��̬
	//	__zero = 0,
	//	/// ����
	//	__negative = -1
	//};

	///
	/// �������ͳ������Ͷ���
	enum onism_c
	{
		/// �У�����
		__on_existance,
		/// �ޣ����
		__on_emptiness,
	};

	///
	/// ��Է�λ���ͳ������Ͷ���
	enum relative_azimuth_c
	{
		/// ��Է�λ��ǰ��
		__ra_former,
		/// ��Է�λ����
		__ra_latter,
	};

	///
	/// ��̬�������ͳ������Ͷ���
	enum triboolean_c
	{
		/// ��̬��һ ��̬���١���̬
		__tri_false=0,
		/// ��̬��� ��̬���桢��̬
		__tri_true=1,
		/// ��̬���� ��̬��δ֪̬
		__tri_dummy=-1,
	};
}
}
