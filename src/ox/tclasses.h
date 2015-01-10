
/**
	date: 20010-10-24
	author : xuhailong
	msn: hailongxu@msn.com

	the most basic & general types defined in oxlib
 */

#include "../ox/nsab.h"





#pragma once




___namespace1_begin(ox)



struct null_type;


template <typename t>
struct nilptr
{
	typedef t value_type;
	typedef t* pointer;
	operator pointer () { return pointer(0); }
	pointer operator ()(size_t i) { return pointer(reinterpret_cast<pointer>(i)); }
};

template <typename t>
struct nilptr <t&>
{
	typedef t& value_type;
	typedef t* pointer;
	operator pointer () { return pointer(0); }
	pointer operator ()(size_t i) { return pointer(reinterpret_cast<pointer>(i)); }
};


/*
	any constructor or assignment do nothing to it
*/
struct empty_type
{
	///*empty constructor*/ empty_type() {}
	///*any type constructor*/ template <typename t> empty_type(t const& o)
	//{
	//}
	///*any type assignment*/ template <typename t> empty_type& operator=(t const& o)
	//{
	//	return *this;
	//}
	empty_type unit() { return empty_type(); }
	empty_type const unit() const { return empty_type(); }
	empty_type next() {return empty_type();}
	empty_type const next() const {return empty_type();}
	template <bool b> empty_type dnext() {return empty_type();}
	template <bool b> empty_type const dnext() const {return empty_type();}
};


inline bool operator==(const empty_type&, const empty_type&)
{return true;}

inline bool operator!=(const empty_type&, const empty_type&)
{return false;}

inline bool operator<(const empty_type&, const empty_type&)
{return false;}

inline bool operator>(const empty_type&, const empty_type&)
{return false;}

inline bool operator<=(const empty_type&, const empty_type&)
{return false;}

inline bool operator>=(const empty_type&, const empty_type&)
{return false;}



/**
@brief: 只是简单的进行类型转存
*/

template <typename type_tn>
struct identity
{
	typedef type_tn type;
};


/**
@brief: 常数和类型的互化
*/

//// 从int到type
template <long value_tc>
struct itot
{
	typedef long type;
	static long const value = value_tc;

	template <long delta=1>
	struct inc
	{
		typedef itot<value+delta> type;
	};
	template <long delta=1>
	struct dec
	{
		typedef itot<value-delta> type;
	};

	operator long () const { return value; }

	static int const sign = (long)(value)<0 ? -1 : (value==0 ? 0 : 1);
};

//// 从int到type
template <long value_tc,long value2_tc>
struct i2tot
{
	typedef itot<value_tc> t;
	typedef itot<value2_tc> t2;
	template <size_t> struct at;
	template <> struct at<0> { static long const value = value_tc; };
	template <> struct at<1> { static long const value = value2_tc; };
	
};


/*
 *  @author：xuhailong
 *  @date：2011-3-4
 *      provide a type that stand the value type of bool
 *  @history: 2009-10-5
 *      yes, we can give all kinds of versions of empty types or null types
 *      and the most interested is we unify all the different type
 */
template <bool bool_value_tc>
struct boolean
{
	typedef bool type;
	static bool const value = bool_value_tc;
	struct bit { static size_t const value = bool_value_tc?1:0; };
	operator bool () const { return value; }
};


struct treectrlid
{
	enum ctrl
	{
		__break = 0,
		__continue_children = 1,
		__continue_sibling = 2,
		__continue = 3, // === __continue_children|__continue_sibling,
	};

	struct met
	{
		template <size_t id> struct is_continue { static bool const value = id?true:false; };
		template <size_t id> struct is_continue_children { static bool const value = id&__continue_children?true:false; };
		template <size_t id> struct is_continue_sibling { static bool const value = id&__continue_sibling?true:false; };
		template <size_t id> struct is_ignore_children { static bool const value = !is_continue_children<id>::value; };
		template <size_t id> struct is_ignore_sibling { static bool const value = !is_continue_sibling<id>::value; };
		template <size_t id> struct is_break { static bool const value = !is_continue<id>::value; };
	};

	treectrlid() : _m_value(__continue) {}
	treectrlid(ctrl c) : _m_value(c) {}
	ctrl value() const {return ctrl(_m_value);}
	bool is_continue() const {return _m_value?true:false;}
	bool is_continue_children() const {return _m_value&__continue_children?true:false;}
	bool is_continue_sibling() const {return _m_value&__continue_sibling?true:false;}
	bool is_ignore_children() const {return !is_continue_children();}
	bool is_ignore_sibling() const {return !is_continue_sibling();}
	bool is_break() const {return !is_continue();}
	void add_continue() {_m_value=__continue;}
	void add_continue_children() {_m_value |= __continue_children;}
	void add_continue_sibling() {_m_value |= __continue_sibling;}
	void add_ignore_children() {_m_value &= ~__continue_children;}
	void add_ignore_sibling() {_m_value &= ~__continue_sibling;}
	void add_break() {_m_value=__break;}
	static treectrlid make_continue() {return treectrlid(__continue);}
	static treectrlid make_continue_children() {return treectrlid(__continue_children);}
	static treectrlid make_continue_sibling() {return treectrlid(__continue_sibling);}
	static treectrlid make_ignore_children() {return treectrlid(__continue_sibling);}
	static treectrlid make_ignore_sibling() {return treectrlid(__continue_children);}
	static treectrlid make_break() {return treectrlid(__break);}
	static treectrlid make(bool b) {return treectrlid(b?__continue:__break);}
	template <bool> static treectrlid make();
	template <> static treectrlid make<true>() {return make_continue();}
	template <> static treectrlid make<false>() {return make_break();}
private:
	size_t _m_value;
};



template <typename t>
struct tref
{
	typedef t value_type;
	typedef t& reference;
	explicit tref(t& p): o(p) {}
	operator t& () { return o; }
	t& operator()() { return o; }
	t& value() { return o; }
	t& value() const { return o; }
	t& o;
};
template <typename t>
struct tref <t&>
{
	typedef t& value_type;
	typedef t& reference;
	explicit tref(t& p): o(p) {}
	operator t& () { return o; }
	t& operator()() { return o; }
	t& value() { return o; }
	t& value() const { return o; }
	t& o;
};

template <typename t> static tref<t> ref(t& o) { return tref<t>(o); }
template <typename t> static tref<t const> ref(t const& o) { return tref<t const>(o); }
//template <typename t> t* const& ref(t* const& o) { return o; }
//template <typename t> t*& ref(t*& o) { return o; }

template <typename t> static t& fwd(t& o) { return o; }
template <typename t> static t const& fwd(t const& o) { return o; }

template <typename t>
struct trefof
{
	typedef t& reference;
	reference operator()(t& o) { return o; }
};
template <typename t>
struct trefof <t&>
{
	typedef t& reference;
	reference operator()(t& o) { return o; }
};
template <typename t>
struct trefof <tref<t>>
{
	typedef typename tref<t>::reference reference;
	reference operator()(t& o) { return o; }
};

template <typename t>
static 
typename trefof<t>::reference
refof(t& o)
{
	return trefof<t>()(o);
}


template <typename t,typename tag>
struct box
{
	box(t const& p)
		: o(p)
	{}
	operator t const& () { return o; }
	t const& o;
};

template <typename t>
struct box <t*,int*>
{
	box(t*const& p)
	{
		o = p;
	}
	t*const& o;
	operator t*const&() { return o; }
};
template <typename t>
struct box <t,int>
{
	box(t const& p)
		: o(p)
	{}
	t const& o;
	operator t const& () { return o; }
};
template <typename t>
struct box <t&,int&>
{
	box(t& p)
		: o(p)
	{}
	t& o;
	operator t& () { return o; }
};
template <typename t>
struct box <t,ox::empty_type>
{
	box(t const& p)
		: o(p)
	{}
	//template <typename px>
	//box(ox::mix::tparam<px>& p)
	//{}
	t const& o;
	operator t const& () { return o; }
};

//template <typename t>
//struct box <t,ox::empty_type>
//{
//	box(t const& p)
//		: o(p)
//	{}
//	t const& o;
//	operator t const& () { return o; }
//};


//template <size_t bool_value_tc>
//struct tribool
//{
//	typedef bool type;
//	static bool const value = bool_value_tc;
//	struct bit { static size_t const value = bool_value_tc?1:0; };
//	operator bool () { return value; }
//};

//template <typename t0>
//struct nake_tlist1
//{
//	static size_t const value = 1;
//	typedef t0 type0;
//	template<size_t i> struct at;
//	template<0> struct at { typedef type0 type; }
//};
//template <typename t0,typename t1>
//struct nake_tlist2
//{
//	static size_t const value = 2;
//	typedef t0 type1;
//	typedef t1 type2;
//	template<size_t i> struct at;
//	template<0> struct at { typedef type0 type; }
//	template<1> struct at { typedef type1 type; }
//};


//
///// tribool type, but never use
//template <int bool_tc>
//struct tribool_type;
//


///*
//	yin and yang both sides
// */
//
//template <char>
//struct side;
//
//template <>
//struct side<'+'>
//{
//	typedef char type;
//	static char const value = '+';
//	operator char () { return value; }
//};
//
//template <>
//struct side<'-'>
//{
//	typedef char type;
//	static char const value = '-';
//	operator char () { return value; }
//};


___namespace1_end()