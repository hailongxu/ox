

/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

//#include "../../cxx/delegate_task_one.h"

#include <new.h>
#include <assert.h>
#include "../ox/nsab.h"
#include "delegate_closure.h"
#include "task_decl.h"


#pragma once


___namespace2_begin(ox,mos)


template <typename t>
struct value_holder_tt
{
	typedef t type;
};
template <typename t>
struct value_holder_tt <t&>
{
	typedef t type;
};
template <typename t>
struct value_holder_tt <t const&>
{
	typedef t type;
};
template <typename t>
struct value_holder_tt <t const>
{
	typedef t type;
};

template <typename closure,typename r,typename bind_tn=void>
struct task_closure : task_tt<r>
{
	typedef closure closure_t;
	typedef r r_type;
	typedef task_tt<r> task_i;
	typedef bind_tn bind_t;

	task_closure()
	{
		name[0] = 0;
	}

	virtual r_type run()
	{
		return _m_closure.run();
	}
	virtual void destroy()
	{
		_m_closure.destruct();
		_delete(this);
	}

	static task_closure* _new() {return new task_closure;}
	static void _delete(task_closure* tc) {delete tc;}

	char name[32];
	bind_t bind;
	closure _m_closure;
};
template <typename closure,typename r>
struct task_closure <closure,r,void> : task_tt<r>
{
	typedef closure closure_t;
	typedef r r_type;
	typedef task_tt<r> task_i;
	typedef void bind_t;

	task_closure()
	{
		name[0] = 0;
	}

	virtual r_type run()
	{
		return _m_closure.run();
	}
	virtual void destroy()
	{
		_m_closure.destruct();
		_delete(this);
	}

	static task_closure* _new() {return new task_closure;}
	static void _delete(task_closure* tc) {delete tc;}

	char name[32];
	closure _m_closure;
};

template <typename r,typename bind_tn=void>
struct task_single
{
	typedef task_single self;
	typedef r r_type;
	typedef bind_tn bind_t;

	template <typename c>
	static task_closure<delegate_closure<r(),value_holder_tt>,r,bind_t>* make(c*_c)
	{
		typedef delegate_closure<r(),value_holder_tt> dc_t;
		typedef task_closure<dc_t,r,bind_t> task_t;
		task_t* task = task_t::_new();
		task->_m_closure.assign(_c);
		assert (task);
		return task;
	}
	static task_closure<delegate_closure<r(),value_holder_tt>,r,bind_t>* make(r(*_f)())
	{
		typedef delegate_closure<r(),value_holder_tt> dc_t;
		typedef task_closure<dc_t,r,bind_t> task_t;
		task_t* task = task_t::_new();
		task->_m_closure.assign(_f);
		assert (task);
		return task;
	}
	static task_closure<delegate_closure<r(),value_holder_tt>,r,bind_t>* make(delegate<r()> const& d)
	{
		typedef delegate_closure<r(),value_holder_tt> dc_t;
		typedef task_closure<dc_t,r,bind_t> task_t;
		task_t* task = task_t::_new();
		task->_m_closure.assign(d);
		assert (task);
		return task;
	}
	template <typename c>
	static task_closure<delegate_closure<r(),value_holder_tt>,r,bind_t>* make(c* _c,r(c::* _f)())
	{
	 	typedef delegate_closure<r(),value_holder_tt> dc_t;
		typedef task_closure<dc_t,r,bind_t> task_t;
		task_t* task = task_t::_new();
		assert (task);
		task->_m_closure.assign(_c,_f);
		return task;
	}

#define DEF_TASK_MAKER(n) \
	template <typename c,DEF_TYPENAMES(n)> \
	static task_closure<delegate_closure<r(DEF_TYPES(n)),value_holder_tt>,r,bind_t>* make(c*_c,DEF_ARGS(n)) \
	{ \
		typedef delegate_closure<r(DEF_TYPES(n)),value_holder_tt> dc_t; \
		typedef task_closure<dc_t,r,bind_t> task_t; \
		task_t* task = task_t::_new(); \
		task->_m_closure.assign(_c,dummy(),DEF_OBJS(n)); \
		assert (task); \
		return task; \
	} \
	template <DEF_TYPENAMES2(q,n),DEF_TYPENAMES(n)> \
	static task_closure<delegate_closure<r(DEF_TYPES2(q,n)),value_holder_tt>,r,bind_t>* make(r(*_f)(DEF_TYPES2(q,n)),DEF_ARGS(n)) \
	{ \
		typedef delegate_closure<r(DEF_TYPES2(q,n)),value_holder_tt> dc_t; \
		typedef task_closure<dc_t,r,bind_t> task_t; \
		task_t* task = task_t::_new(); \
		task->_m_closure.assign(_f,dummy(),DEF_OBJS(n)); \
		assert (task); \
		return task; \
	} \
	template <DEF_TYPENAMES(n)> \
	static task_closure<delegate_closure<r(DEF_TYPES(n)),value_holder_tt>,r,bind_t>* make(delegate<r(DEF_TYPES(n))> const& d,DEF_ARGS(n)) \
	{ \
		typedef delegate_closure<r(DEF_TYPES(n)),value_holder_tt> dc_t; \
		typedef task_closure<dc_t,r,bind_t> task_t; \
		task_t* task = task_t::_new(); \
		task->_m_closure.assign(d,dummy(),DEF_OBJS(n)); \
		assert (task); \
		return task; \
	} \
	template <typename c,DEF_TYPENAMES2(q,n),DEF_TYPENAMES2(p,n)> \
	static task_closure<delegate_closure<r(DEF_TYPES2(q,n)),value_holder_tt>,r,bind_t>* make(c* _c,r(c::* _f)(DEF_TYPES2(q,n)),DEF_ARGS(n)) \
	{ \
	 	typedef delegate_closure<r(DEF_TYPES2(q,n)),value_holder_tt> dc_t; \
		typedef task_closure<dc_t,r,bind_t> task_t; \
		task_t* task = task_t::_new(); \
		assert (task); \
		task->_m_closure.assign(_c,_f,dummy(),DEF_OBJS(n)); \
		return task; \
	}

	DEF_TASK_MAKER(1)
	DEF_TASK_MAKER(2)
	DEF_TASK_MAKER(3)
	DEF_TASK_MAKER(4)
	DEF_TASK_MAKER(5)
	DEF_TASK_MAKER(6)
	DEF_TASK_MAKER(7)
	DEF_TASK_MAKER(8)

#undef DEF_TASK_MAKER
};

template <typename r,typename bind_tn=int>
struct task_single_old : task_tt<r>
{
	typedef task_single_old self;
	typedef r r_type;
	typedef task_tt<r> task_t;
	typedef object_run_tt<r> object_run_t;
	typedef bind_tn bind_t;

	self()
	{
		_m_object_run = 0;
		name[0] = 0;
	}

	/// 0
	template <typename c>
	static self* make(c*_c)
	{
		typedef delegate_closure<r(),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_c);
		return obj;
	}
	static self* make(delegate<r()> const& d)
	{
		typedef delegate_closure<r(),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(d);
		return obj;
	}
	template <typename c>
	static self* make(c* _c,r(c::*_f)())
	{
		typedef delegate_closure<r(),value_holder_tt> task0_t;
		self* obj =  new self;
		obj->_m_object_run = new task0_t(_c,_f);
		return obj;
	}
	/// 1
	template <typename c,typename p0>
	static self* make(c*_c,p0 _p0)
	{
		typedef delegate_closure<r(p0),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_c,dummy(),_p0);
		return obj;
	}
	template <typename p0,typename q0>
	static self* make(r(*_f)(p0),q0 _p0)
	{
		typedef delegate_closure<r(p0),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_f,dummy(),_p0);
		return obj;
	}
	template <typename p0>
	static self* make(delegate<r(p0)> const& d,p0 _p0)
	{
		typedef delegate_closure<r(p0),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(d,dummy(),_p0);
		return obj;
	}
	template <typename c,typename q0,typename p0>
	static self* make(c* _c,r(c::*_f)(q0),p0 _p0)
	{
		typedef delegate_closure<r(q0),value_holder_tt> task_t;
		self* obj =  new self;
		obj->_m_object_run = new task_t(_c,_f,dummy(),_p0);
		return obj;
	}
	/// 2
	template <typename c,/*DEF_TYPENAMES(2)*/typename p0,typename p1>
	static self* make(c*_c,/*DEF_ARGS(2)*/p0 _p0,p1 _p1)
	{
		typedef delegate_closure<r(/*DEF_TYPES(2)*/p0,p1),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_c,dummy(),/*DEF_OBJS(2)*/_p0,_p1);
		return obj;
	}
	template <typename p0,typename p1,typename q0,typename q1>
	static self* make(r(*_f)(p0,p1),q0 _p0,q1 _p1)
	{
		typedef delegate_closure<r(p0,p1),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_f,dummy(),_p0,_p1);
		return obj;
	}
	template <typename p0,typename p1>
	static self* make(delegate<r(p0,p1)> const& d,p0 _p0,p1 _p1)
	{
		typedef delegate_closure<r(p0,p1),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1);
		return obj;
		return 0;
	}
	template <typename c,typename q0,typename q1,typename p0,typename p1>
	static self* make(c* _c,r(c::* _f)(q0,q1),p0 _p0,p1 _p1)
	{
		typedef delegate_closure<r(q0,q1),value_holder_tt> task_t;
		self* obj =  new self;
		obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1);
		return obj;
	}
	/// 3
	template <typename c,typename p0,typename p1,typename p2>
	static self* make(c*_c,p0 _p0,p1 _p1,p2 _p2)
	{
		typedef delegate_closure<r(p0,p1,p2),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2);
		return obj;
	}
	template <typename p0,typename p1,typename p2,typename q0,typename q1,typename q2>
	static self* make(r(*_f)(p0,p1,p2),q0 _p0,q1 _p1,q2 _p2)
	{
		typedef delegate_closure<r(p0,p1,p2),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_f,dummy(),_p0,_p1,_p2);
		return obj;
	}
	template <typename p0,typename p1,typename p2>
	static self* make(delegate<r(p0,p1,p2)> const& d,p0 _p0,p1 _p1,p2 _p2)
	{
		typedef delegate_closure<r(p0,p1,p2),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2);
		return obj;
	}
	template <typename c,typename q0,typename q1,typename q2,typename p0,typename p1,typename p2>
	static self* make(c* _c,r(c::*_f)(q0,q1,q2),p0 _p0,p1 _p1,p2 _p2)
	{
		typedef delegate_closure<r(q0,q1,q2),value_holder_tt> task_t;
		self* obj =  new self;
		obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1,_p2);
		return obj;
	}
	/// 4
	template <typename c,typename p0,typename p1,typename p2,typename p3>
	static self* make(c*_c,p0 _p0,p1 _p1,p2 _p2,p3 _p3)
	{
		typedef delegate_closure<r(p0,p1,p2,p3),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2,_p3);
		return obj;
	}
	template <typename p0,typename p1,typename p2,typename p3,typename q0,typename q1,typename q2,typename q3>
	static self* make(r(*_f)(p0,p1,p2,p3),q0 _p0,q1 _p1,q2 _p2,q3 _p3)
	{
		typedef delegate_closure<r(p0,p1,p2,p3),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_f,dummy(),_p0,_p1,_p2,_p3);
		return obj;
	}
	template <typename p0,typename p1,typename p2,typename p3>
	static self* make(delegate<r(p0,p1,p2,p3)> const& d,p0 _p0,p1 _p1,p2 _p2,p3 _p3)
	{
		typedef delegate_closure<r(p0,p1,p2,p3),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2,_p3);
		return obj;
		return 0;
	}
	template <typename c,typename q0,typename q1,typename q2,typename q3,typename p0,typename p1,typename p2,typename p3>
	static self* make(c* _c,r(c::*_f)(q0,q1,q2,q3),p0 _p0,p1 _p1,p2 _p2,p3 _p3)
	{
		typedef delegate_closure<r(q0,q1,q2,q3),value_holder_tt> task_t;
		self* obj =  new self;
		obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1,_p2,_p3);
		return obj;
	}
	/// 5
	template <typename c,typename p0,typename p1,typename p2,typename p3,typename p4>
	static self* make(c*_c,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
	{
		typedef delegate_closure<r(p0,p1,p2,p3,p4),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2,_p3,_p4);
		return obj;
	}
	template <typename p0,typename p1,typename p2,typename p3,typename p4,typename q0,typename q1,typename q2,typename q3,typename q4>
	static self* make(r(*_f)(p0,p1,p2,p3,p4),q0 _p0,q1 _p1,q2 _p2,q3 _p3,q4 _p4)
	{
		typedef delegate_closure<r(p0,p1,p2,p3,p4),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_f,dummy(),_p0,_p1,_p2,_p3,_p4);
		return obj;
	}
	template <typename p0,typename p1,typename p2,typename p3,typename p4>
	static self* make(delegate<r(p0,p1,p2,p3,p4)> const& d,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
	{
		typedef delegate_closure<r(p0,p1,p2,p3,p4),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2,_p3,_p4);
		return obj;
		return 0;
	}
	template <typename c,typename q0,typename q1,typename q2,typename q3,typename q4,typename p0,typename p1,typename p2,typename p3,typename p4>
	static self* make(c* _c,r(c::*_f)(q0,q1,q2,q3,q4),p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
	{
		typedef delegate_closure<r(q0,q1,q2,q3,q4),value_holder_tt> task_t;
		self* obj =  new self;
		obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1,_p2,_p3,_p4);
		return obj;
	}
	/// 6
	template <typename c,typename p0,typename p1,typename p2,typename p3,typename p4,typename p5>
	static self* make(c*_c,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
	{
		typedef delegate_closure<r(p0,p1,p2,p3,p4,p5),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2,_p3,_p4,_p5);
		return obj;
	}
	template <typename p0,typename p1,typename p2,typename p3,typename p4,typename p5,typename q0,typename q1,typename q2,typename q3,typename q4,typename q5>
	static self* make(r(*_f)(p0,p1,p2,p3,p4,p5),q0 _p0,q1 _p1,q2 _p2,q3 _p3,q4 _p4,q5 _p5)
	{
		typedef delegate_closure<r(p0,p1,p2,p3,p4,p5),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(_f,dummy(),_p0,_p1,_p2,_p3,_p4,_p5);
		return obj;
	}
	template <typename p0,typename p1,typename p2,typename p3,typename p4,typename p5>
	static self* make(delegate<r(p0,p1,p2,p3,p4)> const& d,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
	{
		typedef delegate_closure<r(p0,p1,p2,p3,p4,p5),value_holder_tt> ctask_t;
		self* obj =  new self;
		obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2,_p3,_p4,_p5);
		return obj;
		return 0;
	}
	template <typename c,typename q0,typename q1,typename q2,typename q3,typename q4,typename q5,typename p0,typename p1,typename p2,typename p3,typename p4,typename p5>
	static self* make(c* _c,r(c::*_f)(q0,q1,q2,q3,q4,q5),p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
	{
		typedef delegate_closure<r(q0,q1,q2,q3,q4,q5),value_holder_tt> task_t;
		self* obj =  new self;
		obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1,_p2,_p3,_p4,_p5);
		return obj;
	}

	virtual r_type run()
	{
		return _m_object_run->run();
		//destroy();
	}
	virtual void destroy()
	{
		_m_object_run->destruct();
		delete _m_object_run;
		delete this;
	}
	//run_t* run() const
	//{
	//	return _m_object_run;
	//}
	bind_t bind;
	char name[32];
private:
	object_run_t* _m_object_run;
};

typedef task_single<void,int> thread_task_helper;
typedef task_single<bool,int> timer_task_helper;


template <typename bind_tn,typename poster>
struct task_feedback : task_tt<void>
{
	typedef void r;
	typedef bind_tn bind_t;
	typedef task_tt<r> task_t;
	typedef delegate<r(task_feedback*)> complete_d;

	struct task_complete_t : task_t
	{
		virtual r run()
		{
			if (_m_task_feedback->_m_complete_interface)
				_m_task_feedback->_m_complete_interface->run();
			delete _m_task_feedback;
		}
		task_feedback* _m_task_feedback;
	};

	task_feedback()
	{
		_m_complete._m_task_feedback = this;
	}

	template <typename c>
	static task_feedback* make(c*_c)
	{
		typedef delegate_closure<r()> ctask_t;
		task_feedback* obj =  new task_feedback;
		obj->_m_task_interface = new ctask_t(_c);
		return obj;
	}
	template <typename c,typename p0>
	static task_feedback* make(c*_c,p0 _p0)
	{
		typedef delegate_closure<r(p0)> ctask_t;
		task_feedback* obj =  new task_feedback;
		obj->_m_task_interface = new ctask_t(_c,dummy(),_p0);
		return obj;
	}

	template <typename c>
	void assign_complete(c*_c)
	{
		typedef delegate_closure<r(task_feedback*)> ctask_t;
		_m_complete_interface = new ctask_t(_c,dummy(),this);
	}
	void assign_complete(delegate<r(task_feedback*)> const& d)
	{
		typedef delegate_closure<r(task_feedback*)> ctask_t;
		_m_complete_interface = new ctask_t(d,dummy(),this);
	}

	void set_complete_threadid(size_t tid=-1)
	{
		if (tid==-1)
			_m_complete_threadid = GetCurrentThreadId();
		else
			_m_complete_threadid = tid;
	}

	r operator()()
	{
		_m_task_interface->run();
		if (GetCurrentThreadId()==_m_complete_threadid)
		{
			if (_m_complete_interface)
				_m_complete.run();
		}
		else if (_m_complete_interface)
			_m_post(_m_complete_threadid,&_m_complete);
	}

	virtual r run()
	{
		return operator()();
	}
	//virtual void free()
	//{
	//	delete _m_task;
	//	delete this;
	//}

	task_t* task() const
	{
		return _m_task_interface;
	}
	task_t* complete_task() const
	{
		return _m_complete_interface;
	}
	size_t complete_threadid() const
	{
		return _m_complete_threadid;
	}

	bind_t bind;
private:
	task_t* _m_task_interface;
	task_t* _m_complete_interface;
	task_complete_t _m_complete;
	poster _m_post;
	size_t _m_complete_threadid;
};


___namespace2_end()