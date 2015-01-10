/*	
	类名: shell_kit
	作者: 徐海龙
	日期: 2007-5-20
	文件名: directory.h
	版权：版权归作者所有，本文件可以自由传播
 */

#include "../def/theory_type.h"
#include "./character_static_value.h"

#pragma once

namespace ox
{
namespace string
{
namespace met
{

	template <typename element_tn, ox::theory::polarity_c string_direction_tc>
	struct operator_add
	{
		typedef element_tn element_t;

		void operator++()
		{
			add<string_direction_tc>(element);
		}
		void operator++(int)
		{
			add<string_direction_tc>(element);
		}
		template <ox::theory::polarity_c string_direction_tc> void add(element_t & element);
		template<> void add<ox::theory::__positive>(element_t & element) { element++; }
		template<> void add<ox::theory::__negative>(element_t & element) { element--; }
		element_t element;
	};

	template <typename distance_tn, typename element_tn, ox::theory::polarity_c string_direction_tc>
	struct operator_distance
	{
		typedef distance_tn distance_t;
		typedef element_tn element_t;

		static distance_t distance(element_t begin, element_t end)
		{ return distance<string_direction_tc>(begin,end); }
		template <ox::theory::polarity_c string_direction_tc>
		static distance_t distance(element_t begin, element_t end);
		template <>
		static distance_t distance<ox::theory::__positive>(element_t begin, element_t end)
		{ return end-begin; }
		template <>
		static distance_t distance<ox::theory::__negative>(element_t begin, element_t end)
		{ return begin-end; }
	};


	template <ox::theory::polarity_c,typename element_tn,ox::string::constant_t::string_mode_c>
	struct endcondition_of_string;

	template <ox::theory::polarity_c string_direction_tc,typename element_tn>
	struct endcondition_of_string<string_direction_tc,element_tn,ox::string::constant_t::__string_null>
	{
		typedef element_tn element_t;
		static bool eof(element_t const * p) { return *p==character_static_value<element_t>::null_character; }
		static bool ueof(element_t const * p) { return *p!=character_static_value<element_t>::null_character; }
	};
	template <typename element_tn> // interval is former closed latter open interval
	struct endcondition_of_string<ox::theory::__positive,element_tn,ox::string::constant_t::__string_interval>
	{
		typedef element_tn element_t;
		static bool eof(element_t const * p, element_t const * pend) { return p>=pend; }
		static bool ueof(element_t const * p, element_t const * pend) { return p<pend; }
	};
	template <typename element_tn> // interval is former closed latter open interval
	struct endcondition_of_string<ox::theory::__negative,element_tn,ox::string::constant_t::__string_interval>
	{
		typedef element_tn element_t;
		static bool eof(element_t const * p, element_t const * pend) { return p<=pend; }
		static bool ueof(element_t const * p, element_t const * pend) { return p>pend; }
	};

}
} // end of match
} // end of ox