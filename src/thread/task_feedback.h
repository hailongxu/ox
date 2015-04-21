

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
//#include <string.h>

#include "delegate_closure.h"
#include "task_decl.h"


#pragma once


namespace ox
{
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

	template <typename r,typename bind_tn=int>
	struct task_single : task_tt<r>
	{
		typedef r r_type;
		typedef task_tt<r> task_t;
		typedef object_run_tt<r> object_run_t;
		typedef bind_tn bind_t;

		task_single()
		{
			_m_object_run = 0;
			name[0] = 0;
		}

		/// 0
		template <typename c>
		static task_single* make(c*_c)
		{
			typedef ox::delegate_closure<r(),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(_c);
			return obj;
		}
		static task_single* make(delegate<r()> const& d)
		{
			typedef ox::delegate_closure<r(),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(d);
			return obj;
		}
		template <typename c>
		static task_single* make(c* _c,r(c::*_f)())
		{
			typedef ox::delegate_closure<r(),value_holder_tt> task0_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new task0_t(_c,_f);
			return obj;
		}
		/// 1
		template <typename c,typename p0>
		static task_single* make(c*_c,p0 _p0)
		{
			typedef ox::delegate_closure<r(p0),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(_c,dummy(),_p0);
			return obj;
		}
		template <typename p0>
		static task_single* make(delegate<r(p0)> const& d,p0 _p0)
		{
			typedef ox::delegate_closure<r(p0),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(d,dummy(),_p0);
			return obj;
		}
		template <typename c,typename q0,typename p0>
		static task_single* make(c* _c,r(c::*_f)(q0),p0 _p0)
		{
			typedef ox::delegate_closure<r(q0),value_holder_tt> task_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new task_t(_c,_f,dummy(),_p0);
			return obj;
		}
		/// 2
		template <typename c,typename p0,typename p1>
		static task_single* make(c*_c,p0 _p0,p1 _p1)
		{
			typedef ox::delegate_closure<r(p0,p1),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1);
			return obj;
		}
		template <typename p0,typename p1>
		static task_single* make(delegate<r(p0,p1)> const& d,p0 _p0,p1 _p1)
		{
			typedef ox::delegate_closure<r(p0,p1),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1);
			return obj;
			return 0;
		}
		template <typename c,typename q0,typename q1,typename p0,typename p1>
		static task_single* make(c* _c,r(c::* _f)(q0,q1),p0 _p0,p1 _p1)
		{
			typedef ox::delegate_closure<r(q0,q1),value_holder_tt> task_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1);
			return obj;
		}
		/// 3
		template <typename c,typename p0,typename p1,typename p2>
		static task_single* make(c*_c,p0 _p0,p1 _p1,p2 _p2)
		{
			typedef ox::delegate_closure<r(p0,p1,p2),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2);
			return obj;
		}
		template <typename p0,typename p1,typename p2>
		static task_single* make(delegate<r(p0,p1,p2)> const& d,p0 _p0,p1 _p1,p2 _p2)
		{
			typedef ox::delegate_closure<r(p0,p1,p2),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2);
			return obj;
		}
		template <typename c,typename q0,typename q1,typename q2,typename p0,typename p1,typename p2>
		static task_single* make(c* _c,r(c::*_f)(q0,q1,q2),p0 _p0,p1 _p1,p2 _p2)
		{
			typedef ox::delegate_closure<r(q0,q1,q2),value_holder_tt> task_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1,_p2);
			return obj;
		}
		/// 4
		template <typename c,typename p0,typename p1,typename p2,typename p3>
		static task_single* make(c*_c,p0 _p0,p1 _p1,p2 _p2,p3 _p3)
		{
			typedef ox::delegate_closure<r(p0,p1,p2,p3),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2,_p3);
			return obj;
		}
		template <typename p0,typename p1,typename p2,typename p3>
		static task_single* make(delegate<r(p0,p1,p2,p3)> const& d,p0 _p0,p1 _p1,p2 _p2,p3 _p3)
		{
			typedef ox::delegate_closure<r(p0,p1,p2,p3),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2,_p3);
			return obj;
			return 0;
		}
		template <typename c,typename q0,typename q1,typename q2,typename q3,typename p0,typename p1,typename p2,typename p3>
		static task_single* make(c* _c,r(c::*_f)(q0,q1,q2,q3),p0 _p0,p1 _p1,p2 _p2,p3 _p3)
		{
			typedef ox::delegate_closure<r(q0,q1,q2,q3),value_holder_tt> task_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1,_p2,_p3);
			return obj;
		}
		/// 5
		template <typename c,typename p0,typename p1,typename p2,typename p3,typename p4>
		static task_single* make(c*_c,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
		{
			typedef ox::delegate_closure<r(p0,p1,p2,p3,p4),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2,_p3,_p4);
			return obj;
		}
		template <typename p0,typename p1,typename p2,typename p3,typename p4>
		static task_single* make(delegate<r(p0,p1,p2,p3,p4)> const& d,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
		{
			typedef ox::delegate_closure<r(p0,p1,p2,p3,p4),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2,_p3,_p4);
			return obj;
			return 0;
		}
		template <typename c,typename q0,typename q1,typename q2,typename q3,typename q4,typename p0,typename p1,typename p2,typename p3,typename p4>
		static task_single* make(c* _c,r(c::*_f)(q0,q1,q2,q3,q4),p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4)
		{
			typedef ox::delegate_closure<r(q0,q1,q2,q3,q4),value_holder_tt> task_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new task_t(_c,_f,dummy(),_p0,_p1,_p2,_p3,_p4);
			return obj;
		}
		/// 6
		template <typename c,typename p0,typename p1,typename p2,typename p3,typename p4,typename p5>
		static task_single* make(c*_c,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
		{
			typedef ox::delegate_closure<r(p0,p1,p2,p3,p4,p5),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(_c,dummy(),_p0,_p1,_p2,_p3,_p4,_p5);
			return obj;
		}
		template <typename p0,typename p1,typename p2,typename p3,typename p4,typename p5>
		static task_single* make(delegate<r(p0,p1,p2,p3,p4)> const& d,p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
		{
			typedef ox::delegate_closure<r(p0,p1,p2,p3,p4,p5),value_holder_tt> ctask_t;
			task_single* obj =  new task_single;
			obj->_m_object_run = new ctask_t(d,dummy(),_p0,_p1,_p2,_p3,_p4,_p5);
			return obj;
			return 0;
		}
		template <typename c,typename q0,typename q1,typename q2,typename q3,typename q4,typename q5,typename p0,typename p1,typename p2,typename p3,typename p4,typename p5>
		static task_single* make(c* _c,r(c::*_f)(q0,q1,q2,q3,q4,q5),p0 _p0,p1 _p1,p2 _p2,p3 _p3,p4 _p4,p5 _p5)
		{
			typedef ox::delegate_closure<r(q0,q1,q2,q3,q4,q5),value_holder_tt> task_t;
			task_single* obj =  new task_single;
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
			typedef ox::delegate_closure<r()> ctask_t;
			task_feedback* obj =  new task_feedback;
			obj->_m_task_interface = new ctask_t(_c);
			return obj;
		}
		template <typename c,typename p0>
		static task_feedback* make(c*_c,p0 _p0)
		{
			typedef ox::delegate_closure<r(p0)> ctask_t;
			task_feedback* obj =  new task_feedback;
			obj->_m_task_interface = new ctask_t(_c,dummy(),_p0);
			return obj;
		}

		template <typename c>
		void assign_complete(c*_c)
		{
			typedef ox::delegate_closure<r(task_feedback*)> ctask_t;
			_m_complete_interface = new ctask_t(_c,dummy(),this);
		}
		void assign_complete(delegate<r(task_feedback*)> const& d)
		{
			typedef ox::delegate_closure<r(task_feedback*)> ctask_t;
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

} ///end of namespace ox