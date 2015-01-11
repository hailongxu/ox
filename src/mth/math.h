

//
//////////////////////////////////////////////////////////////////////////
/**
	date: 2008-6-23
	author: xuhailong
	histroy: math's operation, tmp's grammer, the second c++ grammer
*/

#pragma once


namespace ox
{
	namespace mth
	{
		template <unsigned int base_tc, unsigned index_tc>
		struct power
		{
			enum { __base = base_tc, __index = index_tc };
			enum { value = __base*power<__base, __index-1>::value };
		};
		template <unsigned int base_tc>
		struct power<base_tc, 1>
		{
			enum { __base = base_tc, __index = 1 };
			enum { value = __base };
		};
		template <unsigned int base_tc>
		struct power<base_tc, 0>
		{
			enum { __base = base_tc, __index = 0 };
			enum { value = 1 };
		};
		template <unsigned int index_tc>
		struct power<0,index_tc>
		{
			enum { __base = 0, __index = index_tc };
			enum { value = 0 };
		};

		//
		// suffix_tc based from <0> ZERO
		template <unsigned long v_tc, unsigned char suffix_tc>
		struct test_bit
		{
			enum
			{
				__out_and=(v_tc)&(1<<suffix_tc),
				__out_bool = !!__out_and,
				__out_bit=__out_bool?1:0
			};
		};

		/// get the max & min value between the const number
		template <unsigned long number1_tc, unsigned long number2_tc>
		struct maxmin
		{
			enum
			{
				max_value = number1_tc>number2_tc ? number1_tc:number2_tc,
				min_value = number1_tc<number2_tc ? number1_tc:number2_tc,
			};
		};
		/// we do not care who is max between the begin and the end number
		/// "[" suffix_begin_tc, suffix_end_tc "]" is given as closed interval form
		template <unsigned int suffix_begin_tc, unsigned int suffix_end_tc>
		struct make_interval
		{
			enum
			{
				__1_value = (-1<<maxmin<suffix_begin_tc,suffix_end_tc>::max_value+1)^(-1<<maxmin<suffix_begin_tc,suffix_end_tc>::min_value),
				__0_value = ~__1_value,
			};
		};
		template <unsigned int suffix_tc>
		struct make_bit
		{
			enum
			{
				__1_value = make_interval<suffix_tc,suffix_tc>::__1_value,
				__0_value = ~__1_value,
			};
		};
		
		/**
			set the highest bit
		*/
		template <typename num_tn, num_tn num_tc, bool bool_bit_tc>
		struct bit_set_highest_tt
		{
			enum
			{
				value = bool_bit_tc
				? (num_tc | make_bit<sizeof(num_tn)*8-1>::__1_value)
				: (num_tc & make_bit<sizeof(num_tn)*8-1>::__0_value)
			};
		};
		/**
			cmc[reverse] the highest bit
		*/
		template <typename num_tn, num_tn num_tc>
		struct bit_cmc_highest_tt
		{
		private:
			enum
			{
				__high_bit_value = num_tc & make_bit<sizeof(num_tn)*8-1>::__1_value,
				__num_except_high_bit = num_tc & make_bit<sizeof(num_tn)*8-1>::__0_value,
				__cmc_high_bit_value = __high_bit_value
										? 0
										: make_bit<sizeof(num_tn)*8-1>::__1_value
			};
		public:
			enum
			{
				value = __num_except_high_bit | __cmc_high_bit_value
			};
		};
		/**
			test the highest bit
		*/
		template <typename num_tn, num_tn num_tc>
		struct bit_tst_highest_tt
		{
			enum
			{
				__out_and=(num_tc)&(make_bit<sizeof(num_tn)*8-1>::__1_value),
				__out_bool = !!__out_and,
				__out_bit=__out_bool?1:0
			};
		};

		/**
			get : Greatest Common Divisor
		*/
		template < unsigned long Np, unsigned long Nq >
		struct gcd
		{
			static const unsigned long value = gcd< Nq, Np % Nq >::value;
		};

		// specialization（模板具体化），用作终止递归
		template < unsigned long Np >
		struct gcd< Np, 0 >
		{ 
			static const unsigned long value = Np;
		};

	} // end of met
} // end of ox
