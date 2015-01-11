
/**
	author : xuhailong
	msn: hailongxu@msn.com
	date: 2009-3-24
 */

#include "../ox/nsab.h"
#include "../met/_if_switch.h"
#include "../met/tslist.h"
#include "../met/lambda0.h"
#include "../met/lambda.h"
#include "./mixture_decl.h"
#include "./mixture_method_decl.h"
#include "./mixture_conv.h"
#include "./mixture_inf.h"


#pragma once



/////////////////////////////////////
///
/// mixture method
///
/////////////////////////////////////

___namespace2_begin(ox,mix)



/*
	mixture_method of each unit of mixture whose type of unit is void
	sl : slist
	i : index/i
	iow : index of first level of mixture, which keeps immutable in this unfoldering
	ow : the derivedest mixture type, an also immutable type

	inner type 
	_t : _type_tn
 */


/*
	/// t: the current tag name of method
	/// i: the current index of method tslist
	/// iow: the current index of mixture tslist
	/// ow: the derivedest mixture type
	
	slmd = lamdate<method_unit>::type

	template <typename t,typename i,typename iow,typename go>
	struct method_unit
	{
		return_type operator()()
		{
			/// get the derivedest object of mixture
			ow* owner = static_cast<ow*>(this);
			/// do your business
			/// .....

			return return_type();
		}
	};
 */


/*
	slmd: the method unit-instantiated
 */
template <template<size_t>class ssl,size_t i,typename tsl,typename uz,typename ms,typename edi,typename idi>
struct mixture_method<ssl<i>,tsl,uz,ms,edi,idi>
	: methodlmd_apply<ssl<i>,tsl,uz,ms,edi>::type
{
	/// mixture_method's metas
	typedef ssl<i> ifeature;
	typedef typename ifeature::idiff idiff;
	/// the element's meta, for the compatible with the tslist
	typedef typename ifeature::_t_syms _t_type;
	typedef typename ifeature::_t_type _t_syms; //_t_syms _m_syms;
	typedef typename ifeature::_t_meta _t_meta;
	typedef mixture_method <ssl<i-1>,tsl,uz,ms,edi,typename ssl<i-1>::idiff> _t_next;

	static size_t const index = ssl<0>::n-i;
	static size_t const iowner = tsl::i;
	typedef mixture_method self;
	typedef tinf<ssl<i>,tsl,uz,ms,edi> thinf;
	typedef typename thinf::owner owner;
	typedef typename methodlmd_apply<ssl<i>,tsl,uz,ms,edi>::type unit_type;

	_t_next& next() { *return static_cast<_t_next*>(this); }
	_t_next const& next() const { *return static_cast<_t_next const*>(this); }



	/// constructor and destructor of data-unit
	typedef typename methodlmd_apply<ssl<i>,tsl,uz,ms,edi>::type base;
	typedef mixture_method self;
	typedef mixture_method<ssl<i>,tsl,uz,ms,edi,idi> data_unit_type;
	typedef typename thinf::value_type object_type;
	typedef typename thinf::reference reference;
	typedef typename thinf::const_reference const_reference;

	self() {}
	self(data_unit_type const& du): base(du) {}
	self(ox::empty_type const&) {}
	self(typename thinf::param_type v0): base(v0) {}
	template <template<size_t>class _ssl,typename _tsl,typename _uz,typename _ms,typename _edi,typename _idi>
	self(mixture_method<_ssl<0>,_tsl,_uz,_ms,_edi,_idi> const& o): base(o) {}
};


template <template<size_t>class ssl,size_t i,typename tsl,typename uz,typename ms,typename edi>
struct mixture_method<ssl<i>,tsl,uz,ms,edi,ox::itot<0>>
	: methodlmd_apply<ssl<i>,tsl,uz,ms,edi>::type
{
	/// mixture_method's metas
	typedef ssl<i> ifeature;
	/// the element's meta, for the compatible with the tslist
	typedef typename ifeature::_t_syms _t_type;
	typedef typename ifeature::_t_type _t_syms; //_t_syms _m_syms;
	typedef typename ifeature::_t_meta _t_meta;
	typedef mixture_method <ssl<i-1>,tsl,uz,ms,edi,ox::itot<0>> _t_next;

	static size_t const index = ssl<0>::n-i;
	static size_t const iowner = tsl::i;
	typedef mixture_method self;
	typedef tinf<ssl<i>,tsl,uz,ms,edi> thinf;
	typedef typename thinf::owner owner;
	typedef typename methodlmd_apply<ssl<i>,tsl,uz,ms,edi>::type unit_type;

	_t_next& next() { *return static_cast<_t_next*>(this); }
	_t_next const& next() const { *return static_cast<_t_next const*>(this); }

	mixture_method() {}
	template<typename t> mixture_method(t const&) {}
};


template <template<size_t>class ssl,typename tsl,typename uz,typename ms,typename edi,typename idi>
struct mixture_method<ssl<1>,tsl,uz,ms,edi,idi>
	: methodlmd_apply<ssl<1>,tsl,uz,ms,edi>::type
{
	static size_t const index = 1;
	/// the element's meta, for the compatible with the tslist
	typedef typename ssl<1>::_t_syms _t_type;
	typedef typename ssl<1>::_t_type _t_syms;
	typedef typename ssl<1>::_t_meta _t_meta;
	typedef void _t_next;

	/// mixture_method's metas
	typedef ssl<1> ifeature;
	typedef mixture_method self;
	typedef typename methodlmd_apply<ssl<1>,tsl,uz,ms,edi>::type unit_type;
	typedef tinf<ssl<1>,tsl,uz,ms,edi> thinf;
	typedef typename thinf::owner owner;

	ox::empty_type next() { return ox::empty_type(); }
	ox::empty_type next() const { return ox::empty_type(); }



	typedef mixture_method self;
	typedef mixture_method<ssl<1>,tsl,uz,ms,edi,idi> data_unit_type;
	typedef typename methodlmd_apply<ssl<1>,tsl,uz,ms,edi>::type base;
	typedef typename thinf::value_type object_type;
	typedef typename thinf::reference reference;
	typedef typename thinf::const_reference const_reference;


	self() {}
	self(data_unit_type const& du): base(du) {}
	self(ox::empty_type const&) {}
	self(typename thinf::param_type v0): base(v0) {}
	template <template<size_t>class _ssl,typename _tsl,typename _uz,typename _ms,typename _edi,typename _idi>
	self(mixture_method<_ssl<0>,_tsl,_uz,_ms,_edi,_idi> const& o): base(o) {}
};


template <template<size_t>class ssl,typename tsl,typename uz,typename ms,typename edi>
struct mixture_method<ssl<1>,tsl,uz,ms,edi,ox::itot<0>>
	: methodlmd_apply<ssl<1>,tsl,uz,ms,edi>::type
{
	static size_t const index = 1;
	/// the element's meta, for the compatible with the tslist
	typedef typename ssl<1>::_t_syms _t_type;
	typedef typename ssl<1>::_t_type _t_syms;
	typedef typename ssl<1>::_t_meta _t_meta;
	typedef void _t_next;

	/// mixture_method's metas
	typedef ssl<1> ifeature;
	typedef mixture_method self;
	typedef tinf<ssl<1>,tsl,uz,ms,edi> thinf;
	typedef typename thinf::owner owner;
	typedef typename methodlmd_apply<ssl<1>,tsl,uz,ms,edi>::type unit_type;

	ox::empty_type next() { return ox::empty_type(); }
	ox::empty_type next() const { return ox::empty_type(); }


	mixture_method() {}
	template<typename t> mixture_method(t const&) {}
};

template <template<size_t>class ssl,typename tsl,typename uz,typename ms,typename edi,typename idi>
struct mixture_method<ssl<0>,tsl,uz,ms,edi,idi>
{
	static size_t const index = 0;
	/// the element's meta, for the compatible with the tslist
	typedef typename ssl<0>::_t_syms _t_type;
	typedef typename ssl<0>::_t_type _t_syms;
	typedef typename ssl<0>::_t_meta _t_meta;
	typedef void _t_next;

	/// mixture_method's metas
	typedef void unit_type;
	typedef ssl<0> ifeature;
	typedef mixture_method self;
	typedef tinf<ssl<0>,tsl,uz,ms,edi> thinf;
	typedef typename thinf::owner owner;
	typedef typename thinf::value_type value_type;
	typedef typename thinf::reference reference;
	typedef typename thinf::const_reference const_reference;


	value_type _m_value;

	self() :_m_value() {}
	self(self const& other): _m_value(other._m_value) {}
	template <template<size_t>class _ssl,typename _sl,typename _uz,typename _ms,typename _edi,typename _idi>
	self(mixture_method<_ssl<0>,_sl,_uz,_ms,_edi,_idi> const& other)
		: _m_value(other._m_value)
	{}
	template <template<size_t>class _ssl,typename _sl,typename _uz,typename _ms,typename _edi>
	self(mixture_method<_ssl<0>,_sl,_uz,_ms,_edi,ox::itot<0>> const& other)
	{}
	self(ox::empty_type const&) {}
	self(typename thinf::param_type value): _m_value(value) {}
	

	reference value() { return _m_value; }
	const_reference value() const { return _m_value; }


	//self& assign(self const& o)
	//{
	//	_m_value = o._m_value;
	//	return *this;
	//}

	//self& assign(const_reference p)
	//{
	//	_m_value = p;
	//	return *this;
	//}

	//template <template<size_t>class _ssl,typename _sl,typename _uz,typename _ms,typename _edi,typename _idi>
	//self& assign(mixture_method<_ssl<0>,_sl,_uz,_ms,_edi,_idi> const& o)
	//{
	//	_m_value = o._m_value;
	//	return *this;
	//}

	self& operator=(self const& o)
	{
		_m_value = o._m_value;
		return *this;
	}

	self& operator=(const_reference o)
	{
		_m_value = o;
		return *this;
	}

	template <template<size_t>class _ssl,typename _sl,typename _uz,typename _ms,typename _edi,typename _idi>
	self& operator=(mixture_method<_ssl<0>,_sl,_uz,_ms,_edi,_idi> const& o)
	{
		_m_value = o._m_value;
		return *this;
	}
	template <template<size_t>class _ssl,typename _sl,typename _uz,typename _ms,typename _edi>
	self& operator=(mixture_method<_ssl<0>,_sl,_uz,_ms,_edi,ox::itot<0>> const& o)
	{
		return *this;
	}
};

template <template<size_t>class ssl,typename tsl,typename uz,typename ms,typename edi>
struct mixture_method<ssl<0>,tsl,uz,ms,edi,ox::itot<0>>
{
	static size_t const index = 0;
	/// the element's meta, for the compatible with the tslist
	typedef typename ssl<0>::_t_syms _t_type;
	typedef typename ssl<0>::_t_type _t_syms;
	typedef typename ssl<0>::_t_meta _t_meta;
	typedef void _t_next;

	mixture_method() {}
	template<typename t> mixture_method(t const&) {}
};



/// overload the ==, !=

template <template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi,typename idi>
bool operator ==
(
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& l,
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& r
)
{
	return l.value()==r.value();
}

template <template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi>
bool operator ==
(
	mixture_method<ssl<0>,sl,uz,ms,edi,ox::itot<0>>const& l,
	mixture_method<ssl<0>,sl,uz,ms,edi,ox::itot<0>>const& r
)
{
	return true;
}

template <template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi,typename idi,typename non_dataunit>
bool operator ==
(
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& l,
	non_dataunit const& r
)
{
	return false;
}

template <typename non_dataunit,template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi,typename idi>
bool operator ==
(
	non_dataunit const& l,
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& r
)
{
	return false;
}




/// whether the two mixture is equal at value

template <template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi,typename idi>
bool operator !=
(
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& l,
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& r
)
{
	return l.value()!=r.value();
}

template <template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi>
bool operator !=
(
	mixture_method<ssl<0>,sl,uz,ms,edi,ox::itot<0>>const& l,
	mixture_method<ssl<0>,sl,uz,ms,edi,ox::itot<0>>const& r
)
{
	return false;
}

template <template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi,typename idi,typename non_dataunit>
bool operator !=
(
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& l,
	non_dataunit const& r
)
{
	return true;
}

template <typename non_dataunit,template<size_t>class ssl,typename sl,typename uz,typename ms,typename edi,typename idi>
bool operator !=
(
	non_dataunit const& l,
	mixture_method<ssl<0>,sl,uz,ms,edi,idi>const& r
)
{
	return true;
}




___namespace2_end()
