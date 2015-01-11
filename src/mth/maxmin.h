
/*
 *  @desc provide a best method for how to get the max and min_umature value
 */

#pragma once

namespace ox
{
	namespace mth
	{

		#undef max
		#undef min

		/// just for same type at present
		template <typename N>
		N& min (N& _1, N& _2)
		{
			return ((_1)>(_2))?(_2):(_1);
		}
		template <typename N>
		N* min (N* _1, N* _2)
		{
			return ((_1)>(_2))?(_2):(_1);
		}

		template <typename N>
		N& max (N& _1, N& _2)
		{
			return ((_1)>(_2))?(_1):(_2);
		}

		template <typename N>
		N* max (N* _1, N* _2)
		{
			return ((_1)>(_2))?(_1):(_2);
		}

		template <typename l_tn, typename r_tn>
		class min_result
		{
			typedef l_tn ltype;
			typedef r_tn rtype;
		public:
			min_result(ltype& lhs, rtype& rhs)
				: _m_lhs(lhs), _m_rhs(rhs)
			{}
			operator ltype&()
			{ return _m_lhs < _m_rhs ? _m_lhs : _m_rhs; }
			operator rtype&()
			{ return _m_lhs < _m_rhs ? _m_lhs : _m_rhs; }
		private:
			ltype& _m_lhs;
			rtype& _m_rhs;
		};

		template <typename lr_tn>
		class min_result<lr_tn, lr_tn>
		{
			typedef lr_tn lrtype;
		public:
			min_result(lrtype& lhs, lrtype& rhs)
				: _m_lhs(lhs), _m_rhs(rhs)
			{}
			operator lrtype()
			{ return _m_lhs < _m_rhs ? _m_lhs : _m_rhs; }
		private:
			lrtype& _m_lhs;
			lrtype& _m_rhs;
		};

		template <class l_tn, class r_tn>
		min_result<l_tn,r_tn> min_umature(l_tn lhs, r_tn rhs)
		{
			return min_result(lhs, rhs);
		}

		template <class l_tn, class r_tn>
		struct _mtti_get_min_return_type;

		// Specialization for the l_tn == r_tn case
		template <class lr_tn>
		struct _mtti_get_min_return_type<lr_tn, lr_tn>
		{
			typedef lr_tn& type;
		};

		// Specialization for bool and char
		template <>
		struct _mtti_get_min_return_type<bool, char>
		{
			typedef char type;
		};


		template <class l_tn, class r_tn>
		typename _mtti_get_min_return_type<l_tn, r_tn>::type 
		min_umature(l_tn& lhs, r_tn& rhs)
		{ return lhs < rhs ? lhs : rhs; }

		template <class l_tn, class r_tn>
		typename _mtti_get_min_return_type<const l_tn, r_tn>::type 
		min_umature(const l_tn& lhs, r_tn& rhs)
		{ return lhs < rhs ? lhs : rhs; }

		template <class l_tn, class r_tn>
		typename _mtti_get_min_return_type<l_tn, const r_tn>::type
		min_umature(l_tn& lhs, const r_tn& rhs)
		{ return lhs < rhs ? lhs : rhs; }

		template <class l_tn, class r_tn>
		typename _mtti_get_min_return_type<const l_tn, const r_tn>::type 
		min_umature(const l_tn& lhs, const r_tn& rhs)
		{ return lhs < rhs ? lhs : rhs; }

		//typedef TYPELIST_3(float, double, long double) FloatingPointTypes;
		//typedef Loki::TL::IndexOf<FloatingPointTypes, double>::value

		//typedef Loki::Select<sizeof(wchar_t) <
		//	sizeof(short int), wchar_t, short int>::Result
		//	SmallInt;

		//typedef TYPELIST_14(
		//	const bool,
		//	const char,
		//	const signed char,
		//	const unsigned char,
		//	const wchar_t,
		//	const short int,
		//	const unsigned short int,
		//	const int,
		//	const unsigned int,
		//	const long int,
		//	const unsigned long int,
		//	const float,
		//	const double,
		//	const long double)
		//	ArithTypes;

		//template <class l_tn, class r_tn>
		//typename MinMaxTraits<l_tn, r_tn>::Result
		//	Min(l_tn& lhs, r_tn& rhs)
		//{ if (lhs < rhs) return lhs; return rhs; }

		//template <class l_tn, class r_tn>
		//typename MinMaxTraits<const l_tn, r_tn>::Result
		//	Min(const l_tn& lhs, r_tn& rhs)
		//{ if (lhs < rhs) return lhs; return rhs; }

	} /// end of namespace mth
} /// end of namespace ox