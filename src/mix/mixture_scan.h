
/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 20010-10-24
	±éÀúÒ»¸ömixture : instantiate of slist
 */

#include "../ox/nsab.h"
#include "../ox/tclasses.h"
#include "../met/tnums.h"
#include "../met/tslist.h"
#include "../met/lambda.h"
#include "./mixture_decl.h"
#include "./mixture_is.h"
#include "./mixture_sizeof.h"


#pragma once


___namespace2_begin(ox,mix)



/*
depth:0                   root
                        /  |  \   \
depth:1              c1   c2   c3   c4
                    / |    |
depth:2           l1 l2   c5
*/

/// example of handle 
/*
template <typename path,tyename i,typename tag>
struct handle
{
	/// if you has param
	template <typename param_tn>
	handle(param_tn& param)
		: _m_param(param)
	{}
	template <typename mixture_tn,typename parent_tn,typename origin_tn>
	ox::treectrlid operator()(mixture_tn& m,parent_tn& parent,origin_tn& origin)
	{
		return ox::treectrlid::make<true>();
	}
};
*/

/// we can also add the conf of whether the suffix of handle
/// is static of dynamic
/// whatmore, when the range is embedded mixture, whether we give 
/// the scan mode or scan depth.
template <typename handle_lmd,char direct_tc='+',typename path=ox::tnum1<0>,size_t suffix_tc=0>
struct mixture_scan;

template <typename handle_lmd,char direct_tc,typename path,size_t suffix_tc,typename param_tn,size_t isibling=0>
struct mixture_scan_sibling;


struct mixture_scan_sibling_ignored
{
	template <typename mixture_or_unit,typename owner,typename origin>
	ox::treectrlid operator()(mixture_or_unit& m_or_u,owner& o,origin& or)
	{
		return ox::treectrlid::make<true>();
	}
	mixture_scan_sibling_ignored() {}
	template <typename t> mixture_scan_sibling_ignored(t const&) {}
	static size_t const controlid = ox::treectrlid::__continue;
};

template <bool exitcode>
struct mixture_scan_sibling_exit
{
	template <typename param_tn>
	mixture_scan_sibling_exit(param_tn const& param)
	{}
	template <typename mixture_tn,typename parent_tn,typename origin_tn>
	ox::treectrlid operator()(mixture_tn& m,parent_tn& parent,origin_tn& origin)
	{
		return ox::treectrlid::make<exitcode>();
	}
};
template <typename mixture_scan_tn,typename param_tn>
struct mixture_scan_delegate
{
	static size_t const controlid = ox::treectrlid::__continue;

	mixture_scan_delegate(param_tn& param)
		: _m_param(param)
	{}
	template <typename mixture_data_tn,typename parent_tn,typename origin_tn>
	ox::treectrlid operator()(mixture_data_tn& md,parent_tn& parent,origin_tn& origin)
	{
		return mixture_scan_tn().run(md.value(),_m_param,ox::boolean<true>(),parent,origin);
	}
	param_tn& _m_param;
};
template <typename mixture_scan_tn>
struct mixture_scan_delegate <mixture_scan_tn,void>
{
	static size_t const controlid = ox::treectrlid::__continue;

	template <typename mixture_data_tn,typename parent_tn,typename origin_tn>
	ox::treectrlid operator()(mixture_data_tn& md,parent_tn& parent,origin_tn& origin)
	{
		return mixture_scan_tn().run(md.value(),ox::empty_type(),ox::boolean<false>(),parent,origin);
	}
};

struct mixture_scan_handle_ignored
{
	static size_t const controlid = ox::treectrlid::__continue;
	template <typename mixture_or_unit,typename owner,typename origin>
	ox::treectrlid operator()(mixture_or_unit& m_or_u,owner& o,origin& or)
	{
		return ox::treectrlid::make<true>();
	}
	mixture_scan_handle_ignored() {}
	template <typename t> mixture_scan_handle_ignored(t const&) {}
};

template <typename handle_tn,typename param_tn>
struct mixture_scan_handle_delegate
	//: handle_tn
{
	mixture_scan_handle_delegate(param_tn& param)
		: _m_handle(param)
	{}
	template <typename mixture_tn,typename parent_tn,typename origin_tn>
	ox::treectrlid operator()(mixture_tn& m,parent_tn& parent,origin_tn& origin)
	{
		return _m_handle(m,parent,origin);
	}
	handle_tn _m_handle;
};
template <typename handle_tn>
struct mixture_scan_handle_delegate <handle_tn,void>
	//: handle_tn
{
	template <typename param_tn>
	mixture_scan_handle_delegate(param_tn const& param)
		//: _m_handle(param)
	{}
	template <typename mixture_tn,typename parent_tn,typename origin_tn>
	ox::treectrlid operator()(mixture_tn& m,parent_tn& parent,origin_tn& origin)
	{
		return _m_handle(m,parent,origin);
	}
	handle_tn _m_handle;
};

template <typename param_tn>
struct mixture_scan_param_delegate
{
	mixture_scan_param_delegate(param_tn& param)
		: _m_param(param)
	{}
	param_tn& param_ref() { return _m_param; }
	param_tn& _m_param;
};
template <>
struct mixture_scan_param_delegate <void>
{
	template <typename param_tn>
	mixture_scan_param_delegate(param_tn const& param)
	{}
	mixture_scan_param_delegate& param_ref() { return *this; }
};

template <typename handle,typename handle_next,char direct>
struct mixture_scan_sibling_expression
{
	template <typename mixture_tn,typename parent_tn,typename origin_tn,typename param_tn>
	ox::treectrlid operator()(mixture_tn& m,parent_tn& p,origin_tn& o,param_tn& param)
	{
		ox::treectrlid ctrid;
		(ctrid=handle(param)(m.unit(),p,o)).is_continue() &&
		(ctrid=handle_next(param)(m.next(),m,o)).is_continue();
		return ctrid;
	}
};
template <typename handle,typename handle_next>
struct mixture_scan_sibling_expression <handle,handle_next,'-'>
{
	template <typename mixture_tn,typename parent_tn,typename origin_tn,typename param_tn>
	ox::treectrlid operator()(mixture_tn& m,parent_tn& p,origin_tn& o,param_tn& param)
	{
		ox::treectrlid ctrlid;
		(ctrlid=handle_next(param)(m.next(),m,o)).is_continue() &&
		(ctrlid=handle(param)(m.unit(),p,o)).is_continue();
		return ctrlid;
	}
};

/// scan forwardly, we use the first method
template <typename handle_lmd,char direct,typename path,size_t suffix_tc>
struct mixture_scan
{
	static size_t const controlid = ox::treectrlid::__continue;

	template <typename mixture_tn>
	ox::treectrlid operator()(mixture_tn& m)
	{
		typedef typename ox::met::add_con<mixture_tn>::to<ox::empty_type>::type empty_type;
		return run(m,ox::empty_type(),ox::boolean<false>(),empty_type(),m);
	}
	template <typename mixture_tn,typename param_tn>
	ox::treectrlid operator()(mixture_tn& m,param_tn& param)
	{
		typedef typename ox::met::add_con<mixture_tn>::to<ox::empty_type>::type empty_type;
		return run(m,param,ox::boolean<true>(),empty_type(),m);
	}
	template <typename mixture_tn,typename param_tn,bool has,typename parent_tn,typename origin_tn>
	ox::treectrlid run(mixture_tn& m,param_tn& param,ox::boolean<has>const&,parent_tn& p,origin_tn& o)
	{
		typedef typename handle_lmd::apply<path,ox::itot<suffix_tc>,ox::tags::begin>::type begin_handle;
		typedef typename handle_lmd::apply<path,ox::itot<suffix_tc>,ox::tags::end>::type end_handle;
		typedef typename ox::met::iff<has,param_tn,void>::type param_type;
		typedef mixture_scan_handle_delegate<begin_handle,param_type> begin_handle_delegate;
		typedef mixture_scan_handle_delegate<end_handle,param_type> end_handle_delegate;

		static bool const __is_continue_children
			= ox::treectrlid::met::is_continue_children<begin_handle::controlid>::value;
		typedef typename
			met::iff
			<
				__is_continue_children,
				mixture_scan_sibling<handle_lmd,direct,path,suffix_tc,param_type>,
				mixture_scan_sibling_ignored
			>
			::type
		children_handle;

		ox::treectrlid ctrlid;
		(ctrlid=begin_handle_delegate(param)(m,p,o)).is_continue_children() &&
		(ctrlid=children_handle(param)(m,p,o)).is_continue() &&
		(ctrlid=end_handle_delegate(param)(m,p,o)).is_continue();
		return ctrlid;
	}
};

template <typename handle_lmd,char direct,typename path,size_t suffix_tc,typename param_tn,size_t sibling_suffix>
struct mixture_scan_sibling
	: mixture_scan_param_delegate<param_tn>
{
	static size_t const controlid = ox::treectrlid::__continue;

	template <typename _param_tn>
	mixture_scan_sibling(_param_tn& param)
		: mixture_scan_param_delegate<param_tn>(param)
	{}

	/// start the scan
	template <typename mixture_tn,typename parent_tn,typename origin_tn>
	ox::treectrlid operator()(mixture_tn& m,parent_tn& parent,origin_tn& origin)
	{
		/// the next objectid
		typedef typename mixture_tn::ifeature sl;
		static bool const __is_obj_void = met::is_void<typename sl::_t_type>::value;
		static bool const __is_obj_mixture = is_mixture<typename sl::_t_type>::value;
		static size_t const __objid = __is_obj_void?0:(__is_obj_mixture?2:1);

		typedef typename
			ox::met::_switch
			<
				ox::met::_case<__objid==0,mixture_scan_handle_ignored>,
				ox::met::_case<__objid==1,handle_lmd::apply<typename path::push<sibling_suffix>::type,ox::itot<suffix_tc+1>,void>::type>,
				ox::met::_case<__objid==2,mixture_scan_delegate<mixture_scan<handle_lmd,direct,typename path::push<sibling_suffix>::type,suffix_tc+1>,param_tn> >
			>
			::type
		handle_first;
		typedef mixture_scan_handle_delegate<handle_first,param_tn> handle_first_delegate;

		static bool const __is_next_valiable = !ox::met::is_void<typename mixture_tn::_t_next>::value;

		typedef typename
			ox::met::iff
			<
				!!handle_first::controlid && __is_next_valiable,
				mixture_scan_sibling<handle_lmd,direct,path,suffix_tc+1,param_tn,sibling_suffix+1>,
				mixture_scan_sibling_exit<!__is_next_valiable>
			>
			::type
		handle_next_sibling;

		typedef handle_first_delegate handle;
		typedef handle_next_sibling handle_next;
		return mixture_scan_sibling_expression<handle,handle_next,direct>
			()(m,parent,origin,param_ref());
		return ox::treectrlid();
	}
};




#if 0

	struct handle
	{
		template <size_t i,typename unit>
		bool begin(size_t depth)
		{}
		template <size_t i,typename unit>
		bool end(size_t depth)
		{}
		template <size_t i,typename unit>
		bool value(size_t depth)
		{}

		template <size_t i,size_t j,typename unit>
		bool begin(size_t depth)
		{}
		template <size_t i,size_t j,typename unit>
		bool end(size_t depth)
		{}
		template <size_t i,size_t j,typename unit>
		bool value(size_t depth)
		{}
	};

#endif



template <typename mixture,size_t depth=0,size_t i=0>
struct mixture_type_scan;


template <typename sl,typename uz,typename ms,typename edi,size_t depth,size_t i>
struct mixture_type_scan <mixture<sl,uz,ms,edi>,depth,i>
{
	typedef mixture<sl,uz,ms,edi> mixture;
	template <typename _mixture>
	struct _scan
	{
		typedef typename _mixture::_t_next next;
		template <typename handle_tn>
		ox::treectrlid operator ()(handle_tn& handle)
		{
			static bool const __is_type_mixture = is_mixture<typename _mixture::_t_type>::value;
			ox::treectrlid ctrlid;
			(ctrlid=do_handle(handle,ox::boolean<__is_type_mixture>())).is_continue() &&
			(ctrlid=do_handle_tags(handle)).is_continue() &&
			(ctrlid=_scan<next>()(handle)).is_continue();
			return ctrlid;
		}
		template <typename handle_tn>
		static ox::treectrlid do_handle(handle_tn& handle,ox::boolean<true>)
		{
			typedef typename _mixture::unit_type::_t_type _t_type;
			return mixture_type_scan<_t_type,i+1>()(handle);
		}
		template <typename handle_tn>
		static ox::treectrlid do_handle(handle_tn& handle,ox::boolean<false>)
		{
			typedef typename _mixture::unit_type unit_type;
			return handle.operator()<unit_type,depth,i>();
		}
		template <typename handle_tn>
		static ox::treectrlid do_handle_tags(handle_tn& handle)
		{
			typedef typename _mixture::method_type method_type;
			ox::treectrlid ctrlid;
			(ctrlid=handle.begin<method_type,depth,i,0>()).is_continue() &&
			(ctrlid=_scan_tags<method_type>()(handle)).is_continue() &&
			(ctrlid=handle.end<method_type,depth,i,0>()).is_continue();
			return ctrlid;
		}
	};
	template <>
	struct _scan <void>
	{
		template <typename handle_tn>
		ox::treectrlid operator ()(handle_tn& handle)
		{
			return ox::treectrlid::make_continue();
		}
	};
	template <typename _method,size_t j=0>
	struct _scan_tags
	{
		typedef typename _method::_t_next next;
		template <typename handle_tn>
		ox::treectrlid operator ()(handle_tn& handle)
		{
			ox::treectrlid ctrlid;
			(ctrlid=handle.operator()<_method,depth,i,j>()).is_continue() &&
			(ctrlid=_scan_tags<next,j+1>()(handle)).is_continue();
			return ctrlid;
		}
	};
	template <size_t j>
	struct _scan_tags <void,j>
	{
		template <typename handle_tn>
		ox::treectrlid operator ()(handle_tn& handle)
		{
			return ox::treectrlid::make_continue();
		}
	};
	typedef typename mixture::unit_type unit_type;
	template <typename handle_tn>
	ox::treectrlid operator ()(handle_tn& handle)
	{
		ox::treectrlid ctrlid;
		(ctrlid=handle.begin<mixture,depth,i>()).is_continue_children() &&
		(ctrlid=_scan<mixture>()(handle)).is_continue() &&
		(ctrlid=handle.end<mixture,depth,i>()).is_continue();
		return ctrlid;
	}
};
template <size_t depth,size_t i>
struct mixture_type_scan <void,depth,i>
{
	template <typename handle_tn>
	ox::treectrlid operator ()(handle_tn& handle)
	{
		ox::treectrlid ctrlid;
		(ctrlid=handle.begin<mixture,depth,i>()).is_continue_children() &&
		(ctrlid=handle.end<mixture,depth,i>()).is_continue();
		return ctrlid;
	}
};


___namespace2_end()