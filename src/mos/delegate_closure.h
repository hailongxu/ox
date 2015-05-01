

/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */



#include "../cxx/delegate.h"
#include "../ox/tclasses.h"
#include "task_decl.h"


#pragma once


___namespace2_begin(ox,mos)

struct dummy{};

template <typename function_tn,template<typename> class holder=identity>
struct delegate_closure;

template <typename r,template<typename> class holder>
struct delegate_closure <r(),holder> : object_run_tt<r>
{
	typedef r return_type;
	typedef delegate<r()> delegate_t;

public:
	delegate_closure() {}
	delegate_closure(delegate_closure const& o)
	{
		assign(o);
	}
	template <typename c>
	delegate_closure(c* f)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate_closure(c* _c, f _f)
	{
		assign(_c,_f);
	}
	delegate_closure(delegate_t const& d)
	{
		assign(d);
	}

	delegate_t& as_delegate()
	{
		return *this;
	}

	template <typename c>
	void assign(c* f)
	{
		_m_delegate.assign(f);
	}
	template <typename c, typename f>
	void assign(c* _c, f _f)
	{
		_m_delegate.assign(_c,_f);
	}
	void assign(delegate_t const& d)
	{
		_m_delegate = d;
	}
	void assign(delegate_closure const& o)
	{
		_m_delegate = o._m_delegate;
	}

	template <typename c>
	void operator=(c* f)
	{
		assign(f);
	}
	void operator=(delegate_t const& d)
	{
		assign(d);
	}
	void operator=(delegate_closure const& o)
	{
		assign(o);
	}

	bool is_empty() const
	{
		return _m_delegate.is_empty();
	}
	void clear()
	{
		vf = 0;
	}

	r operator()()
	{
		return _m_delegate();
	}
	virtual r run()
	{
		return _m_delegate();
	}
	virtual void destruct()
	{
		this->~delegate_closure();
	}

private:
	delegate_t _m_delegate;
};

template <typename r,typename p1,template<typename> class holder>
struct delegate_closure <r(p1),holder> : object_run_tt<r>
{
	typedef r return_type;
	typedef delegate<r(p1)> delegate_t;
	typedef typename holder<p1>::type pp1;

public:
	delegate_closure() {}
	delegate_closure(delegate_closure const& o)
	{
		assign(o);
	}
	delegate_closure(delegate_t const& d)
	{
		assign(d);
	}
	template <typename c>
	delegate_closure(c* f)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate_closure(c* _c, f _f)
	{
		assign(_c,_f);
	}

	template <typename c>
	delegate_closure(c* f,dummy,p1 _p1)
		: _m_p1(_p1)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate_closure(c* _c, f _f,dummy,p1 _p1)
		: _m_p1(_p1)
	{
		assign(_c,_f);
	}
	delegate_closure(delegate_t const& d,dummy,p1 _p1)
		: _m_p1(_p1)
	{
		assign(d);
	}

	delegate_t& as_delegate()
	{
		return *this;
	}

	template <typename c>
	void assign(c* f)
	{
		_m_delegate.assign(f);
	}
	template <typename c, typename f>
	void assign(c* _c, f _f)
	{
		_m_delegate.assign(_c,_f);
	}
	void assign(delegate_closure const& o)
	{
		_m_delegate = o._m_delegate;
		this->_m_p1 = o._m_p1;
	}
	void assign(delegate<r(p1)> const& d)
	{
		_m_delegate = d;
	}

	template <size_t i,typename p>
	void assign_param(p _p)
	{
		assign_param(itot<i>(),_p);
	}
	void assign_param(itot<0>,p1 _p1)
	{
		_m_p1 = _p1;
	}

	template <typename c>
	void assign(c* f,dummy,p1 _p1)
	{
		_m_delegate.assign(f);
		_m_p1 = _p1;
	}
	template <typename c, typename f>
	void assign(c* _c, f _f,dummy,p1 _p1)
	{
		_m_delegate.assign(_c,_f);
		_m_p1 = _p1;
	}
	void assign(delegate<r(p1)> const& d,dummy,p1 _p1)
	{
		_m_delegate = o._m_delegate;
		_m_p1 = _p1;
	}

	template <typename c>
	void operator=(c* f)
	{
		assign(f);
	}
	void operator=(delegate<r(p1)> const& d)
	{
		assign(d);
	}
	void operator=(delegate_closure const& o)
	{
		assign(o);
	}

	bool is_empty() const
	{
		return _m_delegate.is_empty();
	}
	void clear()
	{
		_m_delegate.clear();
	}

	r operator()(p1 _p1) const
	{
		return _m_delegate(_p1);
	}
	r operator()()
	{
		return _m_delegate(_m_p1);
	}
	virtual r run()
	{
		return operator()();
	}
	virtual void destruct()
	{
		this->~delegate_closure();
	}

private:
	delegate_t _m_delegate;
	pp1 _m_p1;
};


#define MACRO_ASSIGN_FROM_PARAM(i) \
void assign_param(itot<i-1>,p##i _p##i) \
{ \
	_m_p##i = _p##i; \
}
#define MACRO_TYPEDEF_FROM_PARAM(i) \
typedef typename holder<p##i>::type pp##i;
#define MACRO_MEMBERDEF_FROM_PARAM(i) \
pp##i _m_p##i;

template <typename r,DEF_TYPENAMES(2),template<typename> class holder>
struct delegate_closure <r(DEF_TYPES(2)),holder> : object_run_tt<r>
{
	typedef r return_type;
	typedef delegate<r(DEF_TYPES(2))> delegate_t;
	DEF_BRACKET_REPEAT(MACRO_TYPEDEF_FROM_PARAM,2,)

public:
	delegate_closure() {}
	delegate_closure(delegate_closure const& o)
	{
		assign(o);
	}

	template <typename c>
	delegate_closure(c* f)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate_closure(c* _c, f _f)
	{
		assign(_c,_f);
	}

	template <typename c>
	delegate_closure(c* f,dummy,DEF_ARGS(2))
		: DEF_MEMBS_INIT(2)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate_closure(c* _c, f _f,dummy,DEF_ARGS(2))
		: DEF_MEMBS_INIT(2)
	{
		assign(_c,_f);
	}
	delegate_closure(delegate_t const& d,dummy,DEF_ARGS(2))
		: DEF_MEMBS_INIT(2)
	{
		assign(d);
	}

	template <typename c>
	void assign(c* f)
	{
		_m_delegate.assign(f);
	}
	template <typename c, typename f>
	void assign(c* _c, f _f)
	{
		_m_delegate.assign(_c,_f);
	}
	void assign(delegate_t const& d)
	{
		_m_delegate = d;
	}
	void assign(delegate_closure const& o)
	{
		_m_delegate = o._m_delegate;
		DEF_MEMBS_ASSIGN(2);
	}
	template <typename c>
	void assign(c* f,dummy,DEF_ARGS(2))
	{
		_m_delegate.assign(f);
		DEF_MEMBS_ASSIGN_FROMPARAM(2);
	}
	template <typename c, typename f>
	void assign(c* _c, f _f,dummy,DEF_ARGS(2))
	{
		_m_delegate.assign(_c,_f);
		DEF_MEMBS_ASSIGN_FROMPARAM(2);
	}
	void assign(delegate_t const& d,dummy,DEF_ARGS(2))
	{
		_m_delegate = d;
		DEF_MEMBS_ASSIGN_FROMPARAM(2);
	}

	template <typename c>
	void operator=(c* f)
	{
		assign(f);
	}
	void operator=(delegate_t const& d)
	{
		assign(d);
	}
	void operator=(delegate_closure const& o)
	{
		assign(o);
	}

	DEF_BRACKET_REPEAT(MACRO_ASSIGN_FROM_PARAM,2,)

	void clear()
	{
		_m_delegate.clear();
	}

	bool is_empty() const
	{
		return _m_delegate.is_empty();
	}

	r operator()(DEF_ARGS(2)) const
	{
		return _m_delegate(DEF_OBJS(2));
	}
	r operator()()
	{
		return _m_delegate(DEF_MEMBS(2));
	}
	virtual r run()
	{
		return operator()();
	}
	virtual void destruct()
	{
		this->~delegate_closure();
	}

private:
	delegate_t _m_delegate;
	//DEF_MEMBS_DEF(2)
	DEF_BRACKET_REPEAT(MACRO_MEMBERDEF_FROM_PARAM,2,)
};



#define DEF_MACRO_DELEGATE_CLOSURE(n) \
template <typename r,DEF_TYPENAMES(n),template<typename> class holder> \
struct delegate_closure <r(DEF_TYPES(n)),holder> : object_run_tt<r> \
{ \
	typedef r return_type; \
	typedef delegate<r(DEF_TYPES(n))> delegate_t; \
	DEF_BRACKET_REPEAT(MACRO_TYPEDEF_FROM_PARAM,n,) \
\
public: \
	delegate_closure() {} \
	delegate_closure(delegate_closure const& o) \
	{ \
		assign(o); \
	} \
\
	template <typename c> \
	delegate_closure(c* f) \
	{ \
		assign(f); \
	} \
	template <typename c, typename f> \
	delegate_closure(c* _c, f _f) \
	{ \
		assign(_c,_f); \
	} \
\
	template <typename c> \
	delegate_closure(c* f,dummy,DEF_ARGS(n)) \
		: DEF_MEMBS_INIT(n) \
	{ \
		assign(f); \
	} \
	template <typename c, typename f> \
	delegate_closure(c* _c, f _f,dummy,DEF_ARGS(n)) \
		: DEF_MEMBS_INIT(n) \
	{ \
		assign(_c,_f); \
	} \
	delegate_closure(delegate_t const& d,dummy,DEF_ARGS(n)) \
		: DEF_MEMBS_INIT(n) \
	{ \
		assign(d); \
	} \
\
	template <typename c> \
	void assign(c* f) \
	{ \
		_m_delegate.assign(f); \
	} \
	template <typename c, typename f> \
	void assign(c* _c, f _f) \
	{ \
		_m_delegate.assign(_c,_f); \
	} \
	void assign(delegate_t const& d) \
	{ \
		_m_delegate = d; \
	} \
	void assign(delegate_closure const& o) \
	{ \
		_m_delegate = o._m_delegate; \
		DEF_MEMBS_ASSIGN(n); \
	} \
	template <typename c> \
	void assign(c* f,dummy,DEF_ARGS(n)) \
	{ \
		_m_delegate.assign(f); \
		DEF_MEMBS_ASSIGN_FROMPARAM(n); \
	} \
	template <typename c, typename f> \
	void assign(c* _c, f _f,dummy,DEF_ARGS(n)) \
	{ \
		_m_delegate.assign(_c,_f); \
		DEF_MEMBS_ASSIGN_FROMPARAM(n); \
	} \
	void assign(delegate_t const& d,dummy,DEF_ARGS(n)) \
	{ \
		_m_delegate = d; \
		DEF_MEMBS_ASSIGN_FROMPARAM(n); \
	} \
\
	template <typename c> \
	void operator=(c* f) \
	{ \
		assign(f); \
	} \
	void operator=(delegate_t const& d) \
	{ \
		assign(d); \
	} \
	void operator=(delegate_closure const& o) \
	{ \
		assign(o); \
	} \
\
	DEF_BRACKET_REPEAT(MACRO_ASSIGN_FROM_PARAM,n,) \
\
	void clear() \
	{ \
		_m_delegate.clear(); \
	} \
\
	bool is_empty() const \
	{ \
		return _m_delegate.is_empty(); \
	} \
\
	r operator()(DEF_ARGS(n)) const \
	{ \
		return _m_delegate(DEF_OBJS(n)); \
	} \
	r operator()() \
	{ \
		return _m_delegate(DEF_MEMBS(n)); \
	} \
	virtual r run() \
	{ \
		return operator()(); \
	} \
	virtual void destruct() \
	{ \
		this->~delegate_closure(); \
	} \
\
private: \
	delegate_t _m_delegate; \
	/*DEF_MEMBS_DEF(n)*/ \
	DEF_BRACKET_REPEAT(MACRO_MEMBERDEF_FROM_PARAM,n,) \
};

DEF_MACRO_DELEGATE_CLOSURE(3)
DEF_MACRO_DELEGATE_CLOSURE(4)
DEF_MACRO_DELEGATE_CLOSURE(5)
DEF_MACRO_DELEGATE_CLOSURE(6)

___namespace2_end()
