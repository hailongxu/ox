
#include <assert.h>
#include "../met/mod.h"


#pragma once

namespace ox
{


	//template <typename t,bool b>
	//struct ifcon
	//{
	//	typedef t type;
	//};
	//template <typename t>
	//struct ifcon <t,true>
	//{
	//	typedef typename add_con<t>::type type;
	//};


	//template <typename t> inline typename rmv_ref<t>::type&& move(t&& v)
	//{
	//	typedef typename rmv_ref<t>::type ty;
	//	return static_cast<ty&&>(v);
	//}
	///// T& && => T&
	///// T& & => T&
	///// T&& & => T&
	///// T&& && => T&&
	///*
	//	void f(int& a); == f(int&)
	//	void f(int&& a); == f(
	//	void f(int const& a);
	//	void f(int const&& a);
	//	template <typename t> void F(t&& v) { f(forward<t>(v)); }
	//*/
	template<class t> t&& forward(typename met::rmv_ref<t>::type& v)
	{
		return static_cast<t&&>(v);
	}

	//template <typename t>
	//inline void swap(t& a,t& b)
	//{
	//	t c = move(a);
	//	a = move(b);
	//	b = move(c);
	//}

	template <typename t1,typename t2>
	struct pair
	{
		typedef t1 first_type;
		typedef t2 second_type;
		pair() {}
		pair(t1 const& a,t2 const& b) : first(a), second(b) {}
		t1 first;
		t2 second;
	};
	template <typename t1,typename t2,typename t3,typename t4>
	struct pair4
	{
		typedef t1 first_type;
		typedef t2 second_type;
		typedef t3 third_type;
		typedef t4 fourth_type;
		t1 first;
		t2 second;
		t3 third;
		t4 fourth;
		pair4(t1 const& a,t2 const& b,t3 const& c,t4 const& d) : first(a),second(b),third(c),fourth(d) {}
	};


	template<typename ch_tn>
	struct interval_tt
	{
		typedef ch_tn data_type;
		interval_tt() {}
		interval_tt(data_type b,data_type e) : begin(b),end(e) {}
		data_type begin;
		data_type end;
		size_t size() const {return end-begin;}
		interval_tt& zero() {begin=end=0;return *this;}
	};

	//template <typename ty>
	//static interval_tt<ty> make_interval(ty const& a,ty const& b)
	//{
	//	interval_tt<ty> r = {a,b};
	//	return r;
	//}

	template <typename value_tn>
	struct sizable_space_tt
	{
		typedef value_tn value_type;
		value_type* begin;
		size_t size;
		sizable_space_tt():begin(0),size(0) {}
		sizable_space_tt(value_type* _begin,size_t _size):begin(_begin),size(_size) {}
		sizable_space_tt(size_t _size):size(_size) {}
		size_t size_of_bytes() const {return size*sizeof(value_type);}
	};

	template <size_t size_tc>
	char* get_readonly_memory()
	{
		static char const _static_readonly[size_tc] = {0};
		return (char*)(&_static_readonly[0]);
	}

	template <size_t size_tc>
	struct readonly_memory
	{
		char* operator()() const {return get_readonly_memory<size_tc>();}
		template <size_t size_tc>
		static char* get() {return get_readonly_memory<size_tc>();}
	};
	template <typename character_tn>
	struct nullcharacter_tt {character_tn operator()() const {return 0;}};

	//template <typename type_tn>
	//static size_of_bytes(sizable_space_tt<type_tn> const& value)
	//{
	//	return size_of_bytes(value)
	//}
//#undef min
//#undef max
//	template <typename t1,typename t2> static t1 min(t1 p1,t2 p2) {return p1<p2?p1:p2;}
//	template <typename t1,typename t2> static t1 max(t1 p1,t2 p2) {return p1>p2?p1:p2;}

	//template <typename obj_tn>
	//struct alias
	//{
	//	typedef obj_tn obj_type;
	//	aliaser(obj_type& obj) : _m_obj(&obj) {}
	//	aliaser(obj_type* obj) : _m_obj(obj) {}
	//	aliaser& operator=(obj_type const& obj) {*_m_obj=obj;return*this;}
	//	obj_type* _m_obj;
	//};
	//template <typename obj_tn>
	//struct aliaser <obj_tn*>
	//{
	//	typedef obj_tn* obj_type;
	//	aliaser(obj_type& obj) : _m_obj(&obj) {}
	//	aliaser(obj_type* obj) : _m_obj(obj) {}
	//	aliaser& operator=(obj_type const& obj) {*_m_obj=obj;return*this;}
	//	aliaser& operator++() {++(*_m_obj);return *this;}
	//	aliaser& operator--() {--(*_m_obj);return *this;}
	//	aliaser& operator+=(int const& n) {(*_m_obj)+=n;return *this;}
	//	obj_type const& value() const {return *_m_obj;}
	//	obj_type* _m_obj;
	//};
	//template <typename obj_tn>
	//struct aliaser_null
	//{
	//	typedef obj_tn obj_type;
	//	aliaser_null(obj_type& obj) {}
	//	aliaser_null(obj_type* obj=0) {}
	//	aliaser_null& operator=(obj_type const& obj) {return*this;}
	//};
	//template <typename value_tn>
	//struct ptrliker
	//{
	//	typedef ptrliker self;
	//	typedef value_tn value_type;
	//	typedef value_type* pointer_type;
	//	self(pointer_type ptr=0) : _m_pointer(ptr) {}
	//	self& operator=(pointer_type ptr) {_m_pointer=ptr;return*this;}
	//	self& operator=(value_type const& value) {*_m_pointer=value;return*this;}
	//	self& operator++() {++_m_pointer;return *this;}
	//	self operator++(int) {return self(_m_pointer++);}
	//	value_type& operator*() {return *_m_pointer;}
	//	value_type const& operator*() const {return *_m_pointer;}
	//	operator pointer_type() {return _m_pointer;}
	//	pointer_type _m_pointer;
	//};

	//template <typename value_tn>
	//struct ptrliker_null
	//{
	//	typedef ptrliker_null self;
	//	typedef value_tn value_type;
	//	typedef value_type* pointer_type;
	//	self(pointer_type ptr=0) {}
	//	self& operator=(pointer_type ptr) {return*this;}
	//	self& operator=(value_type const& value) {;return*this;}
	//	self& operator++() {return *this;}
	//	self operator++(int) {return *this;}
	//	value_type& operator*() {return *(pointer_type)(0);}
	//	value_type const& operator*() const {return *(pointer_type)(0);}
	//	operator pointer_type() {return 0;}
	//};

	//template <typename obj_tn,typename size_tn>
	//struct baseoffptr;
	//template <typename obj_tn,typename size_tn>
	//struct baseoffptr <obj_tn*,size_tn>
	//{
	//	typedef baseoffptr self;
	//	typedef obj_tn* obj_type;
	//	self(obj_type base,size_tn& off) : _m_base(base), _m_offptr(&off) {}
	//	self(obj_type base,size_tn* off) : _m_base(base), _m_offptr(off) {}
	//	self& operator=(obj_type const& obj) {*_m_offptr=obj-_m_base;return*this;}
	//	self& operator=(size_t const& off) {*_m_offptr=off;return*this;}
	//	self& operator++() {++(*_m_offptr);return *this;}
	//	self& operator--() {--(*_m_offptr);return *this;}
	//	operator obj_type() const {return _m_base+*_m_offptr;}
	//private:
	//	obj_type _m_base;
	//	size_tn* _m_offptr;
	//};


	////// type cast
	//template <typename t>
	//struct remove_pointer
	//{
	//	typedef t type;
	//};
	//template <typename t>
	//struct remove_pointer <t*>
	//{
	//	typedef t type;
	//};


	//struct bdbase {};


	//template <typename to,typename from>
	//static to arg2_cast(from* arg)
	//{
	//	if (!arg) return 0;

	//	if (arg->argtype()==0)
	//	{
	//		assert("the type is the abstract type, we can not cast to the destination type, and so we return 0");
	//		return 0;
	//	}
	//	if (remove_pointer<to>::type::thistype!=arg->argtype())
	//	{
	//		assert("the type cast failed, so we return 0");
	//		return 0;
	//	}
	//	return static_cast<to>(arg);
	//}

	//template <template <typename>class argbaseid_tn,template<typename>class derived_tn>
	//struct argument_simple_base2 : bdbase
	//{
	//	argument_simple_base2(size_t _argtype=0)
	//		: _m_argtype(_argtype)
	//	{}
	//	template <typename t>
	//	argument_simple_base2(size_t _argtype,t*)
	//		: _m_argtype(_argtype)
	//	{}
	//	bool is_empty() const {return _m_argtype==0;}
	//	void* get() { return 0; }
	//	size_t argtype() const { return _m_argtype; }
	//	void set(void const* _pointer)
	//	{
	//		assert("nothing not support");
	//	}
	//	template <typename pt>
	//	pt cast()
	//	{
	//		return arg2_cast<pt>(this);
	//	}
	//	template <typename value_tn>
	//	value_tn& as()
	//	{
	//		typedef derived_tn<value_tn> derived;
	//		derived* d = arg2_cast<derived*>(this);
	//		return d->value();
	//	}
	//	virtual void free()
	//	{
	//		assert("it is a just a abstract class! do nothing!");
	//	}
	//private:
	//	size_t _m_argtype;
	//};


	//template <typename base_tn,template<typename>class id_tn,typename value_tn>
	//struct argument2_struct : base_tn
	//{
	//	typedef argument2_struct self;
	//	typedef base_tn base;
	//	typedef value_tn value_type;
	//	static size_t const thistype = id_tn<value_tn>::value;

	//	self(value_type const* p)
	//		: base(thistype,p)
	//		, _m_value(*p)
	//	{}
	//	self(value_type const& v)
	//		: base(thistype,&v)
	//		, _m_value(v)
	//	{}
	//	value_type* get() { return &_m_value; }
	//	value_type& value() {return _m_value;}
	//	value_type const& value() const {return _m_value;}
	//	void set(value_type const* p)
	//	{
	//		_m_value = *p;
	//		base::set(p);
	//	}
	//	void free()
	//	{
	//		if (this) delete this;
	//	}
	//private:
	//	value_type _m_value;
	//};


	///// under is an example
	///// you should define three types

	//template <typename> struct XuiEventId;
	//template <> struct XuiEventId<int> {static size_t const value = 1;};
	//template <> struct XuiEventId<int*> {static size_t const value = 2;};

	//template <typename value_tn>
	//struct xui_argument2_struct;

	//struct xui_argument_base
	//	: argument_simple_base2<XuiEventId,xui_argument2_struct>
	//{
	//	typedef argument_simple_base2<XuiEventId,xui_argument2_struct> base;
	//	xui_argument_base(size_t _argtype=0)
	//		: base(_argtype)
	//	{}
	//	template <typename t>
	//	xui_argument_base(size_t _argtype,t*)
	//		: base(_argtype)
	//	{}
	//};
	//template <typename value_tn>
	//struct xui_argument2_struct
	//	: argument2_struct<xui_argument_base,XuiEventId,value_tn>
	//{
	//	typedef xui_argument2_struct self;
	//	typedef argument2_struct<xui_argument_base,XuiEventId,value_tn> base;

	//	self(value_type const* p)
	//		: base(p)
	//	{}
	//	self(value_type const& v)
	//		: base(&v)
	//	{}
	//};

	//typedef xui_argument_base XuiRaiseEvent;
}
