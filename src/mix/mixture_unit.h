
/**
	2010-11-10
	作者：徐海龙
	创建一个更加简单的概念，类似于loki下的typelist
	本来我想用原来的mixture，但是原来的那个mixture是基于树类型的，很复杂，不大敢用
	后来，我又写了一个这个简单的调试起来比较容易
	2012-9-22
	作者：徐海龙
	加入了ett的支持
*/

#include "../ox/nsab.h"
#include "mixture_method_decl.h"
#include "mixture_unit_decl.h"
#include "mixture_inf.h"



#pragma once




#define ___def_method_forward(name,inf_lmd,sym,inf) \
	typedef typename inf_lmd::apply<sym>::type inf; \
	typedef typename inf::jfeature ifeature; \
	typedef typename ifeature::idiff idiff; \
	/* the element's meta, for the compatible with the tslist */ \
	typedef typename ifeature::_t_type _t_type; \
	typedef typename ifeature::_t_syms _t_syms; \
	typedef typename ifeature::_t_meta _t_meta; \
	typedef typename inf::_t_next _t_next; \
	typedef name unit_type; /* no need any more, the method itself is unit tooo */ \
	name() {} \
	name(typename inf::unit_type const& du): inf::base(du) {} \
	name(ox::empty_type const&) {} \
	template<typename t0> name(t0& p0): inf::base(p0) {} \
	template <typename _sym,size_t _i,size_t _iow,size_t _idi,typename _inf> \
	name(ox::mix::mixture_data<_sym,_i,_iow,_idi,_inf> const& p): inf::base(p) {}

#define ___def_method_for_data(name,inf) \
	name() {} \
	name(typename inf::unit_type const& du): inf::base(du) {} \
	name(ox::empty_type const&) {} \
	name(typename inf::param_type v0): inf::base(v0) {} \
	template <typename _sym,size_t _i,size_t _iow,size_t _b,typename _inf> \
	name(ox::mix::mixture_data<_sym,_i,_iow,_b,_inf> const& v): inf::base(v) {}

#define ___def_method_for_void(name,inf) 
	/* nothing */



#define ___def_method_begin(name) \
template <typename sym,size_t i,size_t iow,size_t b,typename inf> \
struct name : inf::base \
{ \
	___def_method_forward(name,inf)


#define ___def_method_end() \
}




___namespace2_begin(ox,mix)



template <typename inf>
struct avoid_void
{
	typedef typename
		met::iff
		<
			met::is_same<typename inf::param_type,ox::empty_type const&>::value,
			ox::empty_type,
			typename inf::param_type
		>
		::type
	type;
};

template <typename sym,size_t i,size_t iow,size_t idi,typename inf_lmd>
struct method_default : inf_lmd::call<sym>::base
{
	___def_method_forward(method_default,inf_lmd,sym,inf)

	void operator()() {}
	void operator()() const {}
};


template <typename sym,size_t i,size_t iow,size_t idi,typename inf_lmd>
struct data_method_default : inf_lmd::call<sym>::base
{
	___def_method_forward(data_method_default,inf_lmd,sym,inf)

	typename inf::reference
	operator()()
	{
		return inf::unit(this).value();
	}
	typename inf::const_reference
	operator()() const
	{
		return inf::unit(this).value();
	}
};

/// no value
template <typename sym,size_t i,size_t iow,typename inf_lmd>
struct extern_method_default<sym,i,iow,0,inf_lmd> : inf_lmd::call<sym>::base
{
	___def_method_forward(extern_method_default,inf_lmd,sym,inf)

	void operator()() {}
	void operator()() const {}
};

/// member value
template <typename sym,size_t i,size_t iow,typename inf_lmd>
struct extern_method_default<sym,i,iow,2,inf_lmd> : inf_lmd::call<sym>::base
{
	___def_method_forward(extern_method_default,inf_lmd,sym,inf)

	typename ox::met::add_ref<typename inf::jfeature::_t_type>::type
	operator()()
	{
		return _m_value;
	}
	typename ox::met::add<int const&>::to<typename inf::jfeature::_t_type>::type
	operator()() const
	{
		return _m_value;
	}

	typename inf::jfeature::_t_type _m_value;
};

/// stack value, return temporary value of external type
template <typename sym,size_t i,size_t iow,typename inf_lmd>
struct extern_method_default<sym,i,iow,3,inf_lmd> : inf_lmd::call<sym>::base
{
	___def_method_forward(extern_method_default,inf_lmd,sym,inf)

	typename inf::jfeature::_t_type
	operator()() const
	{
		return inf::jfeature::_t_type();
	}
	template <typename t1>
	typename inf::jfeature::_t_type
	operator()(t1 p1) const
	{
		return inf::jfeature::_t_type(p1);
	}
	template <typename t1,typename t2>
	typename inf::jfeature::_t_type
	operator()(t1 p1,t2 p2) const
	{
		return inf::jfeature::_t_type(p1,p2);
	}
};


//
//___def_method_begin(data_method_default)
//
//	typename inf::reference
//	operator()()
//	{
//		return inf::unit(this).value();
//	}
//	typename inf::const_reference
//	operator()() const
//	{
//		return inf::unit(this).value();
//	}
//
//___def_method_end();



template <typename sym,size_t i,size_t iow,size_t idi,typename inf_lmd>
struct mixture_data
{
	/// the element's meta, for the compatible with the tslist
	typedef void _t_type;
	typedef void _t_syms;
	typedef void _t_meta;
	typedef void _t_next;

	/// mixture_method's metas
	typedef sym ssl0;
	typedef void unit_type;
	typedef typename inf_lmd::apply<sym>::type thinf;
	typedef typename thinf::ifeature ifeature;
	typedef mixture_data self;
	//typedef tinf<ssl0,tsl,uz,ms,edi> thinf;
	typedef typename thinf::owner_type owner_type;
	typedef typename thinf::value_type value_type;
	typedef typename thinf::reference reference;
	typedef typename thinf::const_reference const_reference;


	value_type _m_value;

	self() :_m_value()
	{}
	self(self const& other): _m_value(other._m_value)
	{}
	template <typename _sym,size_t _i,size_t _iow,size_t _idi,typename _inf>
	self(mixture_data<_sym,_i,_iow,_idi,_inf> const& other)
		: _m_value(other._m_value)
	{}
	template <typename _sym,size_t _i,size_t _iow,typename _inf>
	self(mixture_data<_sym,_i,_iow,0,_inf> const& other)
	{}
	self(ox::empty_type const&)
	{}
	template <typename t>
	self(t& value): _m_value(value)
	{}
	

	reference value() { return _m_value; }
	const_reference value() const { return _m_value; }
	operator const_reference () const { return _m_value; }
	operator reference () { return _m_value; }


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

	template <typename _sym,size_t _i,size_t _iow,size_t _idi,typename _inf>
	self& operator=(mixture_data<_sym,_i,_iow,_idi,_inf> const& o)
	{
		_m_value = o._m_value;
		return *this;
	}
	template <typename _sym,size_t _i,size_t _iow,typename _inf>
	self& operator=(mixture_data<_sym,_i,_iow,0,_inf> const& o)
	{
		return *this;
	}
};

template <typename sym,size_t i,size_t iow,typename inf_lmd>
struct mixture_data <typename sym,i,iow,0,inf_lmd>
{
	/// the element's meta, for the compatible with the tslist
	typedef void _t_type;
	typedef void _t_syms;
	typedef void _t_meta;
	typedef void _t_next;

	typedef typename inf_lmd::apply<sym>::type inf;
	typedef typename inf::ifeature ifeature;

	mixture_data() {}
	template<typename t> mixture_data(t const&) {}
};




/// overload the ==, !=

template <typename sym,size_t i,size_t iow,size_t idi,typename inf>
bool operator ==
(
	mixture_data<sym,i,iow,idi,inf>const& l,
	mixture_data<sym,i,iow,idi,inf>const& r
)
{
	return l.value()==r.value();
}

template <typename sym,size_t i,size_t iow,typename inf>
bool operator ==
(
	mixture_data<sym,i,iow,0,inf>const& l,
	mixture_data<sym,i,iow,0,inf>const& r
)
{
	return true;
}

template <typename sym,size_t i,size_t iow,size_t idi,typename inf,typename non_dataunit>
bool operator ==
(
	mixture_data<sym,i,iow,idi,inf>const& l,
	non_dataunit const& r
)
{
	return false;
}

template <typename non_dataunit,typename sym,size_t i,size_t iow,size_t idi,typename inf>
bool operator ==
(
	non_dataunit const& l,
	mixture_data<sym,i,iow,idi,inf>const& r
)
{
	return false;
}




/// whether the two mixture is equal at value

template <typename sym,size_t i,size_t iow,size_t b,typename inf>
bool operator !=
(
	mixture_data<sym,i,iow,b,inf>const& l,
	mixture_data<sym,i,iow,b,inf>const& r
)
{
	return l.value()!=r.value();
}

template <typename sym,size_t i,size_t iow,typename inf>
bool operator !=
(
	mixture_data<sym,i,iow,0,inf>const& l,
	mixture_data<sym,i,iow,0,inf>const& r
)
{
	return false;
}

template <typename sym,size_t i,size_t iow,size_t b,typename inf,typename non_dataunit>
bool operator !=
(
	mixture_data<sym,i,iow,b,inf>const& l,
	non_dataunit const& r
)
{
	return true;
}

template <typename non_dataunit,typename sym,size_t i,size_t iow,size_t b,typename inf>
bool operator !=
(
	non_dataunit const& l,
	mixture_data<sym,i,iow,b,inf>const& r
)
{
	return true;
}





___namespace2_end()
