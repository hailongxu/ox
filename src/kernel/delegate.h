

/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

//#include "../../cxx/delegate_one.h"


#ifndef ___DELEGAGE_MACRO_INCLUDED
#define ___DELEGAGE_MACRO_INCLUDED




#include <new.h>
//#include <string.h>

#include "macro_params.h"


#pragma once


template <bool,typename a,typename b>
struct iff
{
	typedef a type;
};
template <typename a,typename b>
struct iff <false,a,b>
{
	typedef b type;
};

template <typename class_function>
struct is_mf_const
{
	static bool const value = false;
};
template <typename c,typename r>
struct is_mf_const <r(c::*)()const>
{
	static bool const value = true;
};
template <typename c,typename r,typename p1>
struct is_mf_const <r(c::*)(p1)const>
{
	static bool const value = true;
};
template <typename c,typename r,typename p1,typename p2>
struct is_mf_const <r(c::*)(p1,p2)const>
{
	static bool const value = true;
};
template <typename c,typename r,typename p1,typename p2,typename p3>
struct is_mf_const <r(c::*)(p1,p2,p3)const>
{
	static bool const value = true;
};


template <typename function_tn>
struct delegate;

template <typename r>
struct delegate <r()>
{
	typedef r return_type;
	template <typename f> friend bool operator==(delegate<f> const&,delegate<f> const&);

private:
	template <typename r>
	struct vfunction
	{
		virtual r invoke() const = 0;
		virtual void* object() const = 0;
		virtual void set_object(void*) = 0;
		virtual size_t size() const = 0;
	};

	template <typename r>
	struct cfunction : vfunction <r>
	{
		typedef cfunction self;
		typedef r (*function)();
		virtual r invoke() const
		{
			return (*f)();
		}
		virtual void* object() const
		{
			return 0;
		}
		virtual void set_object(void*)
		{}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		function f;
	};

	template <typename c, typename r>
	struct xfunction : vfunction <r>
	{
		typedef xfunction self;
		virtual r invoke() const
		{
			return (*o)();
		}
		virtual void* object() const
		{
			return o;
		}
		virtual void set_object(void* obj)
		{
			o = (c*)obj;
		}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		c* o;
	};

	template <bool bconstf,typename c, typename r>
	struct mfunction : vfunction <r>
	{
		typedef mfunction self;
		typedef r (c::*function_nc)();
		typedef r (c::*function_c)() const;
		typedef typename iff<bconstf,function_c,function_nc>::type function;
		virtual r invoke() const
		{
			return (o->*f)();
		}
		virtual void* object() const
		{
			return o;
		}
		virtual void set_object(void* obj)
		{
			o = (c*)obj;
		}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		c* o;
		function f;
	};

public:
	delegate(): vf(0) {}
	delegate(delegate const& o)
	{
		assign(o);
	}
	template <typename c>
	delegate(c* f)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate(c* _c, f _f)
	{
		assign(_c,_f);
	}

	delegate& assign(r(*f)())
	{
		cfunction<r>* pxf = new(buffer)cfunction<r>;
		pxf->f = f;
		vf = pxf;
		return *this;
	}
	template <typename c>
	delegate& assign(c* f)
	{
		xfunction<c,r>* pxf = new(buffer)xfunction<c,r>;
		pxf->o = f;
		vf = pxf;
		return *this;
	}
	template <typename c, typename f>
	delegate& assign(c* _c, f _f)
	{
		typedef mfunction<is_mf_const<f>::value,c,r> mf;
		mf* pmf = new(buffer)mf;
		pmf->o = _c;
		pmf->f = _f;
		vf = pmf;
		return *this;
	}
	delegate& assign(delegate const& o)
	{
		memcpy(buffer,o.buffer,sizeof(buffer));
		vf = (vfunction<r>*)buffer;
		if (o.is_empty()) vf = 0;
		return *this;
	}

	template <typename c>
	delegate& operator=(c* f)
	{
		return assign(f);
	}
	delegate& operator=(delegate const& o)
	{
		return assign(o);
	}

	bool is_empty() const
	{
		return vf==0;
	}
	void clear()
	{
		vf = 0;
	}

	r operator()() const
	{
		return vf->invoke();
	}
	template <typename object_tn>
	object_tn* object() const
	{
		return (object_tn*)(vf->object());
	}
	template <typename object_tn>
	void set_object(object_tn* obj)
	{
		return vf->set_object(obj);
	}

private:
	vfunction<r>* vf;
	char buffer[16];
};

template <typename r,typename p1>
struct delegate <r(p1)>
{
	typedef r return_type;
	template <typename f> friend bool operator==(delegate<f> const&,delegate<f> const&);

private:
	template <typename r, typename p1>
	struct vfunction
	{
		virtual r invoke(p1 _p1) const = 0;
		virtual void* object() const = 0;
		virtual void set_object(void*) = 0;
		virtual size_t size() const = 0;
	};

	template <typename r, typename p1>
	struct cfunction : vfunction <r,p1>
	{
		typedef cfunction self;
		typedef r (*function)(p1);
		virtual r invoke(p1 _p1) const
		{
			return (*f)(_p1);
		}
		virtual void* object() const
		{
			return 0;
		}
		virtual void set_object(void* obj)
		{}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		function f;
	};

	template <typename c, typename r, typename p1>
	struct xfunction : vfunction <r,p1>
	{
		typedef xfunction self;
		virtual r invoke(p1 _p1) const
		{
			return (*o)(_p1);
		}
		virtual void* object() const
		{
			return o;
		}
		virtual void set_object(void* obj)
		{
			o = (c*)obj;
		}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		c* o;
	};

	template <bool bconstf,typename c, typename r, typename p1>
	struct mfunction : vfunction <r,p1>
	{
		typedef mfunction self;
		typedef r (c::*function_nc)(p1);
		typedef r (c::*function_c)(p1) const;
		typedef typename iff<bconstf,function_c,function_nc>::type function;
		virtual r invoke(p1 _p1) const
		{
			return (o->*f)(_p1);
		}
		virtual void* object() const
		{
			return o;
		}
		virtual void set_object(void* obj)
		{
			o = (c*)obj;
		}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		c* o;
		function f;
	};

public:
	delegate(): vf(0) {}
	delegate(delegate const& o)
	{
		assign(o);
	}
	template <typename c>
	delegate(c* f)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate(c* _c, f _f)
	{
		assign(_c,_f);
	}

	delegate& assign(r(*f)(p1))
	{
		typedef cfunction<r,p1> cf;
		cf* pcf = new(buffer)cf;
		pcf->f = f;
		vf = pcf;
		return *this;
	}
	template <typename c>
	delegate& assign(c* f)
	{
		typedef xfunction<c,r,p1> xf;
		xf* pxf = new(buffer)xf;
		pxf->o = f;
		vf = pxf;
		return *this;
	}
	template <typename c, typename f>
	delegate& assign(c* _c, f _f)
	{
		typedef mfunction<is_mf_const<f>::value,c,r,p1> mf;
		mf* pmf = new(buffer)mf;
		pmf->o = _c;
		pmf->f = _f;
		vf = pmf;
		return *this;
	}
	delegate& assign(delegate const& o)
	{
		memcpy(buffer,o.buffer,sizeof(buffer));
		vf = (vfunction<r,p1>*)buffer;
		if (o.is_empty()) vf = 0;
		return *this;
	}

	template <typename c>
	delegate& operator=(c* f)
	{
		return assign(f);
	}
	delegate& operator=(delegate const& o)
	{
		return assign(o);
	}

	bool is_empty() const
	{
		return vf==0;
	}
	void clear()
	{
		vf = 0;
	}

	r operator()(p1 _p1) const
	{
		return vf->invoke(_p1);
	}
	template <typename object_tn>
	object_tn* object() const
	{
		return (object_tn*)(vf->object());
	}
	template <typename object_tn>
	void set_object(object_tn* obj)
	{
		return vf->set_object(obj);
	}

private:
	vfunction<r,p1>* vf;
	char buffer[16];
};

template <typename r,DEF_TYPENAMES(2)>
struct delegate <r(DEF_TYPES(2))>
{
	typedef r return_type;
	template <typename f> friend bool operator==(delegate<f> const&,delegate<f> const&);

private:public:
	template <typename r,DEF_TYPENAMES(2)>
	struct vfunction
	{
		virtual r invoke(DEF_ARGS(2)) const = 0;
		virtual void* object() const = 0;
		virtual void set_object(void*) = 0;
		virtual size_t size() const = 0;
	};

	template <typename r,DEF_TYPENAMES(2)>
	struct cfunction : vfunction <r,DEF_TYPES(2)>
	{
		typedef cfunction self;
		typedef r (*function)(DEF_TYPES(2));
		virtual r invoke(DEF_ARGS(2)) const
		{
			return (*f)(DEF_OBJS(2));
		}
		virtual void* object() const
		{
			return 0;
		}
		virtual void set_object(void* obj)
		{}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		function f;
	};

	template <typename c, typename r,DEF_TYPENAMES(2)>
	struct xfunction : vfunction <r,DEF_TYPES(2)>
	{
		typedef xfunction self;
		virtual r invoke(DEF_ARGS(2)) const
		{
			return (*o)(DEF_OBJS(2));
		}
		virtual void* object() const
		{
			return o;
		}
		virtual void set_object(void* obj)
		{
			o = (c*)obj;
		}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		c* o;
	};

	template <bool bconstf,typename c, typename r,DEF_TYPENAMES(2)>
	struct mfunction : vfunction <r,DEF_TYPES(2)>
	{
		typedef mfunction self;
		typedef r (c::*function_nc)(DEF_TYPES(2));
		typedef r (c::*function_c)(DEF_TYPES(2)) const;
		typedef typename iff<bconstf,function_c,function_nc>::type function;
		virtual r invoke(DEF_ARGS(2)) const
		{
			return (o->*f)(DEF_OBJS(2));
		}
		virtual void* object() const
		{
			return o;
		}
		virtual void set_object(void* obj)
		{
			o = (c*)obj;
		}
		virtual size_t size() const
		{
			return sizeof(self);
		}
		c* o;
		function f;
	};

public:
	delegate(): vf(0) {}
	delegate(delegate const& o)
	{
		assign(o);
	}
	template <typename c>
	delegate(c* f)
	{
		assign(f);
	}
	template <typename c, typename f>
	delegate(c* _c, f _f)
	{
		assign(_c,_f);
	}

	delegate& assign(r(*f)(DEF_TYPES(2)))
	{
		typedef cfunction<r,DEF_TYPES(2)> cf;
		cf* pcf = new(buffer)cf;
		pcf->f = f;
		vf = pcf;
		return *this;
	}
	template <typename c>
	delegate& assign(c* f)
	{
		typedef xfunction<c,r,DEF_TYPES(2)> xf;
		xf* pxf = new(buffer)xf;
		pxf->o = f;
		vf = pxf;
		return *this;
	}
	template <typename c, typename f>
	delegate& assign(c* _c, f _f)
	{
		typedef mfunction<is_mf_const<f>::value,c,r,DEF_TYPES(2)> mf;
		mf* pmf = new(buffer)mf;
		pmf->o = _c;
		pmf->f = _f;
		vf = pmf;
		return *this;
	}
	delegate& assign(delegate const& o)
	{
		memcpy(buffer,o.buffer,sizeof(buffer));
		vf = (vfunction<r,DEF_TYPES(2)>*)buffer;
		if (o.is_empty()) vf = 0;
		return *this;
	}

	template <typename c>
	delegate& operator=(c* f)
	{
		return assign(f);
	}
	delegate& operator=(delegate const& o)
	{
		return assign(o);
	}

	bool is_empty() const
	{
		return vf==0;
	}
	void clear()
	{
		vf = 0;
	}

	r operator()(DEF_ARGS(2)) const
	{
		return vf->invoke(DEF_OBJS(2));
	}
	template <typename object_tn>
	object_tn* object() const
	{
		return (object_tn*)(vf->object());
	}
	template <typename object_tn>
	void set_object(object_tn* obj)
	{
		vf->set_object(obj);
	}
	
private:
	vfunction<r,DEF_TYPES(2)>* vf;
	char buffer[16];
};

 
#define DEF_MACRO_DELEGATE(n) \
template <typename r,DEF_TYPENAMES(n)> \
struct delegate <r(DEF_TYPES(n))> \
{ \
	typedef r return_type; \
	template <typename f> friend bool operator==(delegate<f> const&,delegate<f> const&); \
 \
private: \
	template <typename r,DEF_TYPENAMES(n)> \
	struct vfunction \
	{ \
		virtual r invoke(DEF_ARGS(n)) const = 0; \
		virtual void* object() const = 0; \
		virtual void set_object(void*) = 0; \
		virtual size_t size() const = 0; \
	}; \
 \
	template <typename r,DEF_TYPENAMES(n)> \
	struct cfunction : vfunction <r,DEF_TYPES(n)> \
	{ \
		typedef cfunction self; \
		typedef r (*function)(DEF_TYPES(n)); \
		virtual r invoke(DEF_ARGS(n)) const \
		{ \
			return (*f)(DEF_OBJS(n)); \
		} \
		virtual void* object() const \
		{ \
			return 0; \
		} \
		virtual void set_object(void* obj) \
		{} \
		virtual size_t size() const \
		{ \
			return sizeof(self); \
		} \
		function f; \
	}; \
 \
	template <typename c, typename r,DEF_TYPENAMES(n)> \
	struct xfunction : vfunction <r,DEF_TYPES(n)> \
	{ \
		typedef xfunction self; \
		virtual r invoke(DEF_ARGS(n)) const \
		{ \
			return (*o)(DEF_OBJS(n)); \
		} \
		virtual void* object() const \
		{ \
			return o; \
		} \
		virtual void set_object(void* obj) \
		{ \
			o = (c*)obj; \
		} \
		virtual size_t size() const \
		{ \
			return sizeof(self); \
		} \
		c* o; \
	}; \
 \
	template <bool bconstf,typename c, typename r,DEF_TYPENAMES(n)> \
	struct mfunction : vfunction <r,DEF_TYPES(n)> \
	{ \
		typedef mfunction self; \
		typedef r (c::*function_nc)(DEF_TYPES(n)); \
		typedef r (c::*function_c)(DEF_TYPES(n)) const; \
		typedef typename iff<bconstf,function_c,function_nc>::type function; \
		virtual r invoke(DEF_ARGS(n)) const \
		{ \
			return (o->*f)(DEF_OBJS(n)); \
		} \
		virtual void* object() const \
		{ \
			return o; \
		} \
		virtual void set_object(void* obj) \
		{ \
			o = (c*)obj; \
		} \
		virtual size_t size() const \
		{ \
			return sizeof(self); \
		} \
		c* o; \
		function f; \
	}; \
 \
public: \
	delegate(): vf(0) {} \
	delegate(delegate const& o) \
	{ \
		assign(o); \
	} \
	template <typename c> \
	delegate(c* f) \
	{ \
		assign(f); \
	} \
	template <typename c, typename f> \
	delegate(c* _c, f _f) \
	{ \
		assign(_c,_f); \
	} \
	template <typename func> \
	delegate& assign(r(*f)(DEF_TYPES(n))) \
	{ \
		typedef cfunction<r,DEF_TYPES(n)> cf; \
		cf* pcf = new(buffer)cf; \
		pcf->o = f; \
		vf = pcf; \
		return *this; \
	} \
	template <typename c> \
	delegate& assign(c* f) \
	{ \
		typedef xfunction<c,r,DEF_TYPES(n)> xf; \
		xf* pxf = new(buffer)xf; \
		pxf->o = f; \
		vf = pxf; \
		return *this; \
	} \
	template <typename c, typename f> \
	delegate& assign(c* _c, f _f) \
	{ \
		typedef mfunction<is_mf_const<f>::value,c,r,DEF_TYPES(n)> mf; \
		mf* pmf = new(buffer)mf; \
		pmf->o = _c; \
		pmf->f = _f; \
		vf = pmf; \
		return *this; \
	} \
	delegate& assign(delegate const& o) \
	{ \
		memcpy(buffer,o.buffer,sizeof(buffer)); \
		vf = (vfunction<r,DEF_TYPES(n)>*)buffer; \
		if (o.is_empty()) vf = 0; \
		return *this; \
	} \
 \
	template <typename c> \
	delegate& operator=(c* f) \
	{ \
		return assign(f); \
	} \
	delegate& operator=(delegate const& o) \
	{ \
		return assign(o); \
	} \
 \
	bool is_empty() const \
	{ \
		return vf==0; \
	} \
	void clear() \
	{ \
		vf = 0; \
	} \
 \
	r operator()(DEF_ARGS(n)) const \
	{ \
		return vf->invoke(DEF_OBJS(n)); \
	} \
	template <typename object_tn> \
	object_tn* object() const \
	{ \
		return (object_tn*)(vf->object()); \
	} \
	template <typename object_tn> \
	void set_object(object_tn* obj) \
	{ \
		vf->object(obj); \
	} \
 \
private: \
	char buffer[16]; \
	vfunction<r,DEF_TYPES(n)>* vf; \
};


DEF_MACRO_DELEGATE(3)
DEF_MACRO_DELEGATE(4)
DEF_MACRO_DELEGATE(5)
DEF_MACRO_DELEGATE(6)
DEF_MACRO_DELEGATE(7)
DEF_MACRO_DELEGATE(8)
DEF_MACRO_DELEGATE(9)


template <typename function_tn>
bool operator==(delegate<function_tn> const& a,delegate<function_tn> const& b)
{
	bool is_a_null = a.is_empty();
	bool is_b_null = b.is_empty();
	return (is_a_null && is_b_null) || (!is_a_null && !is_b_null && 0==memcmp(a.buffer,b.buffer,a.vf->size()));
}
template <typename function1_tn,typename function2_tn>
bool operator==(delegate<function1_tn> const& a,delegate<function2_tn> const& b)
{
	return false;
}
template <typename function_tn>
bool operator!=(delegate<function_tn> const& a,delegate<function_tn> const& b)
{
	return !operator==(a,b);
}
template <typename function1_tn,typename function2_tn>
bool operator!=(delegate<function1_tn> const& a,delegate<function2_tn> const& b)
{
	return !operator==(a,b);
}
//
///*
//	now at present, we just suport 9 parameters
//	example :
//
//	we list the three pop usage:
//
//
//	/// the type define
//
//	/// c funtion
//	int f(int)
//	{
//		return 1;
//	}
//	/// class function
//	struct Int
//	{
//		int operator()(int) { return 1; }
//		int ff(int) { return 2; }
//	};
//
//	/// first, define the delegate type
//	typedef delegate<int(int)> d;
//
//	/// second, bind one
//
//	/// bind c function
//	d d1(&f);
//	d1(3); /// execute the function
//	d d2;
//	d2.assign(&f);
//
//	/// third: execute
//	d2(3); /// execute the function f(3)
//
//
//	//////////////////////////////////////////
//
//	/// bind class functor
//	Int n;
//	d d3(&n); /// bind the class functor
//	d3(3); /// execute the functor
//
//
//	//////////////////////////////////////////
//
//	/// bind class member function
//	Int n;
//	d d3(&n,&Int::ff); /// bind the class member function
//	d3(3); /// execute the class member function of Int::ff(3)
//*/




#endif ///___DELEGAGE_MACRO_INCLUDED