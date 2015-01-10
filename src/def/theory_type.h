
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
	Desp: 为其他模块提供通用类型定义
	history: 
/************************************************************************/

#pragma once

namespace ox
{
	/**
		定义元类型分类，这两种类型是模板算法时，能操作的类型
		__met_type : 表示类型
		__met_number : 表示是数字
	*/
	enum met_type_c
	{
		__met_type = 1,
		__met_number = 2,
	};

namespace theory
{

	///
	/// 极性类型常量类型定义
	enum polarity_c
	{
		/// 正向
		__positive = 1,
		/// 负向
		__negative = -1,
	};

	///
	///// 三态极性常量类型定义
	//enum tri_polarity_c
	//{
	//	/// 正向
	//	__positive = 1,
	//	/// 零态
	//	__zero = 0,
	//	/// 负向
	//	__negative = -1
	//};

	///
	/// 存在类型常量类型定义
	enum onism_c
	{
		/// 有，存在
		__on_existance,
		/// 无，虚空
		__on_emptiness,
	};

	///
	/// 相对方位类型常量类型定义
	enum relative_azimuth_c
	{
		/// 相对方位：前方
		__ra_former,
		/// 相对方位：后方
		__ra_latter,
	};

	///
	/// 三态布尔类型常量类型定义
	enum triboolean_c
	{
		/// 三态其一 负态、假、阴态
		__tri_false=0,
		/// 三态其二 正态、真、阳态
		__tri_true=1,
		/// 三态其三 空态、未知态
		__tri_dummy=-1,
	};
}
}
