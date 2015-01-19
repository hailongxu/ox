
/**
	作者：xuhailong
	日期：2007-11-12
*/


#pragma once


namespace ox
{
namespace algorithm
{


template
<
	typename element_tn,
	typename interval_tn,
	typename element_ret_tn,
	typename fc_compare_tn=compare_for_algorithm_default_tn<element_tn,interval_tn>
>
struct binary_search_tt
{
	element_ret_tn operator()(element_tn const & element, interval_tn begin, interval_tn end);
};



template
<
	typename element_tn,
	typename fc_compare_tn
>
struct binary_search_tt <element_tn, element_tn *, element_tn *, fc_compare_tn>
{
	typedef element_tn element_t;
	typedef element_t * interval_t;
	typedef element_t * element_ret_t;
	typedef fc_compare_tn fc_compare_t;

	element_ret_t operator()(element_t const & element, interval_t begin, interval_t end)
	{
		end --;
		if (begin>end || element<*begin || element>*end) return 0;

		interval_t i = begin+(end-begin)/2;

		for(; begin<end; i=begin+(end-begin)/2)
		{
			int r = fc_compare_t::_do(element,i);
			if (r==0) break;
			r>0 ? begin=i+1 : end=i-1;
		}
		return begin<end?0:i;
	};
};



} // end of algorithm
} // end of ox