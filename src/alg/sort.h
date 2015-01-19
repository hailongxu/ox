

#include <algorithm>

#include "../def/theory_type.h"

#pragma once


namespace ox
{
namespace alg
{

typedef ox::theory::polarity_c sort_direction_t;

template <typename element_tn, typename element_ref_tn>

struct swap_tt
{
	static void _do(element_tn & e1,element_tn & e2)
	{
		element_tn e = e1;
		e1 = e2;
		e2 = e;
	}
	static void _do(element_tn * e1,element_tn * e2)
	{
		element_tn e = *e1;
		*e1 = *e2;
		*e2 = e;
	}
	static void _do(element_ref_tn e1,element_ref_tn e2)
	{
		_do(*e1,*e2);
	}
};

template <typename element_tn>

struct swap_tt <element_tn,element_tn *>
{
	static void _do(element_tn & e1,element_tn & e2)
	{
		element_tn e = e1;
		e1 = e2;
		e2 = e;
	}
	static void _do(element_tn * e1,element_tn * e2)
	{
		element_tn e = *e1;
		*e1 = *e2;
		*e2 = e;
	}
};

template <typename element_tn>
struct assign_tt
{
	static void _do(element_tn const & s, element_tn & d)
	{
		d = s;
	}
};

template <typename element_tn, typename element_ref_tn>
struct distance_tt
{
	typedef element_tn element_t;
	typedef element_ref_tn element_ref_t;
	static long _do(element_ref_t from, element_ref_t to)
	{
		return (to-from);
	}
};


// ¿ìËÙÅÅÐò

template
<
	sort_direction_t sort_mode_tc,
	typename element_tn,
	typename element_ref_tn,
	typename swap_tn,
	typename distance_tn,
	typename fc_compare_tn
>

struct qsort_tt
{
	typedef element_tn element_t;
	typedef element_ref_tn element_ref_t;
	typedef swap_tn swap_t;
	typedef distance_tn distance_t;
	typedef fc_compare_tn fc_compare_t;

	/// trace the special item
	struct swap_trace_t
	{
		static element_ref_t & _do(element_ref_t _1, element_ref_t _2, element_ref_t & trace)
		{
			swap_t::_do(_1,_2);
			if (trace==_1) return trace=_2;
			if (trace==_2) return trace=_1;
			return trace;
		};
	};

	/// !===
	template <sort_direction_t sort_mode_tc>
	static bool do_l(int const & formmer, int const & latter);
	template <>
	static bool do_l<ox::theory::__positive>(int const & formmer, int const & latter)
	{
		return formmer<latter;
	}
	template <>
	static bool do_l<ox::theory::__negative>(int const & formmer, int const & latter)
	{
		return formmer>latter;
	}
	static bool do_l(int const & formmer, int const & latter)
	{
		return do_l<sort_mode_tc>(formmer,latter);
	}

	/// ===
	template <sort_direction_t sort_mode_tc>
	static bool do_le(int const & formmer, int const & latter);
	template <>
	static bool do_le<ox::theory::__positive>(int const & formmer, int const & latter)
	{
		return formmer<=latter;
	}
	template <>
	static bool do_le<ox::theory::__negative>(int const & formmer, int const & latter)
	{
		return formmer>=latter;
	}
	static bool do_le(int const & formmer, int const & latter)
	{
		return do_le<sort_mode_tc>(formmer,latter);
	}

	/// ===
	template <sort_direction_t sort_mode_tc>
	static bool do_ge(int const & formmer, int const & latter);
	template <>
	static bool do_ge<ox::theory::__positive>(int const & formmer, int const & latter)
	{
		return formmer>=latter;
	}
	template <>
	static bool do_ge<ox::theory::__negative>(int const & formmer, int const & latter)
	{
		return formmer<=latter;
	}
	static bool do_ge(int const & formmer, int const & latter)
	{
		return do_ge<sort_mode_tc>(formmer,latter);
	}

	element_ref_t operator()(element_ref_t begin, element_ref_t end, element_ref_t trace)
	{
		typedef swap_trace_t sort_swap_t;

		struct small_sort_t
		{
			static element_ref_t & _do_4(element_ref_t begin, element_ref_t end, element_ref_t & trace)
			{
				end --;
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin,trace);

				if (do_l(fc_compare_t::_do(*(end-1),*(begin+1)),0)) sort_swap_t::_do(end-1,begin+1,trace);
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin,trace);

				if (do_l(fc_compare_t::_do(*end,*(end-1)),0)) sort_swap_t::_do(end,end-1,trace);
				if (do_l(fc_compare_t::_do(*(end-1),*(begin+1)),0)) sort_swap_t::_do(end-1,begin+1,trace);
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin,trace);
				
				return trace;
			}
			static element_ref_t & _do_3(element_ref_t begin, element_ref_t end, element_ref_t & trace)
			{
				end --;
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin,trace);

				if (do_l(fc_compare_t::_do(*end,*(begin+1)),0)) sort_swap_t::_do(end,begin+1,trace);
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin,trace);

				return trace;
			}
			static element_ref_t & _do_2(element_ref_t begin, element_ref_t & trace)
			{
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin,trace);

				return trace;
			}
		};

		element_ref_t ref_trace = trace;

		int count = 0;
		if ((count=distance_t::_do(begin,end))==4)
		                          return small_sort_t::_do_4(begin,end,ref_trace);
		else if (count==3)        return small_sort_t::_do_3(begin,end,ref_trace);
		else if (count==2)        return small_sort_t::_do_2(begin,ref_trace);
		else if (count<2)         return ref_trace;

		element_ref_t pivot = partition(begin+1,begin,end,ref_trace);

		if ((count=distance_t::_do(begin,pivot))>4)
		                                      (*this)(begin,pivot,ref_trace);
		else if (count==4)        small_sort_t::_do_4(begin,pivot,ref_trace);
		else if (count==3)        small_sort_t::_do_3(begin,pivot,ref_trace);
		else if (count==2)        small_sort_t::_do_2(begin,ref_trace);

		if ((count=distance_t::_do((pivot+1),end))>4)
		                                      (*this)(pivot+1,end,ref_trace);
		else if (count==4)        small_sort_t::_do_4(pivot+1,end,ref_trace);
		else if (count==3)        small_sort_t::_do_3(pivot+1,end,ref_trace);
		else if (count==2)        small_sort_t::_do_2(pivot+1,ref_trace);

		return ref_trace;
	}
	
	/// [begin,end)
	void operator()(element_ref_t begin, element_ref_t end)
	{
		typedef swap_t sort_swap_t;
		struct small_sort_t
		{
			static void _do_4(element_ref_t begin, element_ref_t end)
			{
				end --;
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin);

				if (do_l(fc_compare_t::_do(*(end-1),*(begin+1)),0)) sort_swap_t::_do(end-1,begin+1);
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin);

				if (do_l(fc_compare_t::_do(*end,*(end-1)),0)) sort_swap_t::_do(end,end-1);
				if (do_l(fc_compare_t::_do(*(end-1),*(begin+1)),0)) sort_swap_t::_do(end-1,begin+1);
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin);
			}
			static void _do_3(element_ref_t begin, element_ref_t end)
			{
				end --;
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin);

				if (do_l(fc_compare_t::_do(*end,*(begin+1)),0)) sort_swap_t::_do(end,begin+1);
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin);
			}
			static void _do_2(element_ref_t begin)
			{
				if (do_l(fc_compare_t::_do(*(begin+1),*begin),0)) sort_swap_t::_do(begin+1,begin);
			}
		};

		int count = 0;
		if ((count=distance_t::_do(begin,end))==4)
		                          return small_sort_t::_do_4(begin,end);
		else if (count==3)        return small_sort_t::_do_3(begin,end);
		else if (count==2)        return small_sort_t::_do_2(begin);
		else if (count<2)         return;

		element_ref_t pivot = partition(begin+1,begin,end);

		if ((count=distance_t::_do(begin,pivot))>4)
		                                      (*this)(begin,pivot);
		else if (count==4)        small_sort_t::_do_4(begin,pivot);
		else if (count==3)        small_sort_t::_do_3(begin,pivot);
		else if (count==2)        small_sort_t::_do_2(begin);

		if ((count=distance_t::_do((pivot+1),end))>4)
		                                      (*this)(pivot+1,end);
		else if (count==4)        small_sort_t::_do_4(pivot+1,end);
		else if (count==3)        small_sort_t::_do_3(pivot+1,end);
		else if (count==2)        small_sort_t::_do_2(pivot+1);
	}
	/// be sure that the pivot belongs the interval of [begin, end).
	/// generally, we can select the first element as the pivot.
private:
	element_ref_t partition(element_ref_t pivot, element_ref_t begin, element_ref_t end)
	{
		typedef swap_t sort_swap_t;

		end --;
		sort_swap_t::_do(pivot,begin);
		element_ref_t bound = begin;
		begin ++;

		/// ÓÒ¼«¶Ë
		for (; begin<=end && do_le(fc_compare_t::_do(*begin,*bound),0); begin ++);
		if (begin>end)
		{
			sort_swap_t::_do(bound,end);
			return end;
		}
		/// ×ó¼«¶Ë
		for (; end>=begin && do_ge(fc_compare_t::_do(*end,*bound),0)/*>=0*/; end --);
		if (end<begin && begin<=bound+1)
		{
			; // no swap
			return bound;
		}
		
		if (begin<end)
		{
			sort_swap_t::_do(begin,end);
			begin++,end--;
		}

		for (;distance_t::_do(begin,end)+1>=2;)
		{
			for (; begin<=end && do_le(fc_compare_t::_do(*begin,*bound),0); begin ++);
			if (begin>end)
			{
				sort_swap_t::_do(bound,end);
				return end;
			}
			for (; end>=begin && do_ge(fc_compare_t::_do(*end,*bound),0); end --);
			if (end<begin)
			{
				sort_swap_t::_do(bound,end); // need to swap, it is different
				return end;
			}
			else if (begin<end)
			{
				sort_swap_t::_do(begin,end);
				begin++, end--;
			}
		}
		if (distance_t::_do(begin,end)+1==1)
		{
			if (do_l(fc_compare_t::_do(*begin,*bound),0))
			{
				sort_swap_t::_do(begin,bound);
				return begin;
			}
			else
			{
				sort_swap_t::_do(begin-1,bound);
				return begin-1;
			}
		}
		else
		{
			sort_swap_t::_do(bound,end);
			return end;
		}
	}
	element_ref_t partition(element_ref_t pivot, element_ref_t begin, element_ref_t end, element_ref_t & trace)
	{
		typedef swap_trace_t sort_swap_t;

		end --;
		sort_swap_t::_do(pivot,begin,trace);
		element_ref_t bound = begin;
		begin ++;

		/// ÓÒ¼«¶Ë
		for (; begin<=end && do_le(fc_compare_t::_do(*begin,*bound),0); begin ++);
		if (begin>end)
		{
			sort_swap_t::_do(bound,end,trace);
			return end;
		}
		/// ×ó¼«¶Ë
		for (; end>=begin && do_ge(fc_compare_t::_do(*end,*bound),0); end --);
		if (end<begin && begin<=bound+1)
		{
			; // no swap
			return bound;
		}
		
		if (begin<end)
		{
			sort_swap_t::_do(begin,end,trace);
			begin++,end--;
		}

		for (;distance_t::_do(begin,end)+1>=2;)
		{
			for (; begin<=end && do_le(fc_compare_t::_do(*begin,*bound),0); begin ++);
			if (begin>end)
			{
				sort_swap_t::_do(bound,end,trace);
				return end;
			}
			for (; end>=begin && do_ge(fc_compare_t::_do(*end,*bound),0); end --);
			if (end<begin)
			{
				sort_swap_t::_do(bound,end,trace); // need to swap, it is different
				return end;
			}
			else if (begin<end)
			{
				sort_swap_t::_do(begin,end,trace);
				begin++, end--;
			}
		}
		if (distance_t::_do(begin,end)+1==1)
		{
			if (do_l(fc_compare_t::_do(*begin,*bound),0))
			{
				sort_swap_t::_do(begin,bound,trace);
				return begin;
			}
			else
			{
				sort_swap_t::_do(begin-1,bound,trace);
				return begin-1;
			}
		}
		else
		{
			sort_swap_t::_do(bound,end,trace);
			return end;
		}
	}
};


/// ²åÈëÅÅÐò
template
<
	typename element_tn,
	typename element_ref_tn,
	typename assign_tn,
	typename fc_compare_tn
>

struct insertsort_tt
{
	typedef element_tn element_t;
	typedef element_ref_tn element_ref_t;
	typedef assign_tn assign_t;
	typedef fc_compare_tn fc_compare_t;

	void operator()(element_ref_t begin, element_ref_t end)
	{
		for (element_ref_t i=begin+1; i<end; i++)
		{
			element_t e = *i;
			element_ref_t j=i;
			for (; j>begin && fc_compare_t::_do(*(j-1),e)>0; j--)
				assign_t::_do(*(j-1),*(j));
			assign_t::_do(e,*j);
		}
	}
};


} // end of algorithm
} // end of ox
