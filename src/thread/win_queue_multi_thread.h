/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */


#include <windows.h>
#include <assert.h>


#include <vector>
#include "atomic_number.h"
#include "scope_raii.h"
#include "../cxx/delegate.h"
#include "../cxx/multi_delegate.h"
#include "win_queue_thread.h"
#include "task_feedback.h"
#include "win_multi_thread.h"



#pragma once




namespace ox
{
	struct win_queue_multi_threads : win_multi_threads<win_queue_thread>
	{
		typedef win_queue_multi_threads self;
		typedef win_multi_threads<win_queue_thread> base;

		typedef base::thread_t thread_t;
		typedef thread_t::task_t task_t;
		typedef ox::task_single<int> tasker_t;
		typedef base::added_d added_d;
		typedef thread_t::idle_d idle_d;
		typedef thread_t::busy_d busy_d;


		struct future
		{
			void wait()
			{
				::WaitForSingleObject(hevent,INFINITE);
			}
			HANDLE hevent;
			size_t size;
		};

		struct on_event_thread_added
		{
			on_event_thread_added(self* _me,added_d const& added)
				: me(_me), on_added(added)
			{}
			self* me;
			added_d on_added;
			bool is_empty() const
			{
				return false;
			}
			void operator()(thread_t* thread)
			{
				if (!me->_m_on_idle.is_empty()) thread->on_idle().assign(me,&self::on_thread_idle);
				if (!me->_m_on_busy.is_empty()) thread->on_busy().assign(me,&self::on_thread_busy);
				if (on_added.is_empty()) return;
				on_added(thread);
			}
		};

		self()
		{}
		self(size_t your_id,char const* your_name)
		{
			set_id(your_id,your_name);
		}

		thread_t* create_thread(size_t your_id=-1,char const* your_name=0,bool bstart=true)
		{
			thread_t* th = base::internal_create_thread(your_id,your_name);
			if (!th) return 0;
			add_thread(th);
			if (bstart) th->start();
			return th;
		}

		void add_thread(thread_t* thread)
		{
			add_thread(thread,added_d());
		}

		void add_thread(thread_t* thread,added_d on_added)
		{
			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				self::do_add_thread(thread,on_added);
			else
				_m_thread->add(tasker_t::make(this,&self::do_add_thread,thread,on_added));
		}

		void async_add(task_t* task)
		{
			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				task->run();
			else
				_m_thread->add(task);
		}

		void add_task(task_t* task,size_t threadid)
		{
			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				unsafe_do_add_task(threadid,task);
			else
				_m_thread->add(tasker_t::make(this,&self::unsafe_do_add_task,threadid,task));
		}

		void on_thread_idle(thread_t* thread)
		{
			if (!_m_on_idle.is_empty())
				_m_on_idle(thread);
		}
		void on_thread_busy(thread_t* thread)
		{
			if (!_m_on_busy.is_empty())
				_m_on_busy(thread);
		}

		idle_d& on_idle() {return _m_on_idle;}
		busy_d& on_busy() {return _m_on_busy;}
		idle_d const& on_idle() const {return _m_on_idle;}
		busy_d const& on_busy() const {return _m_on_busy;}

	private:
		idle_d _m_on_idle;
		busy_d _m_on_busy;


		void unsafe_do_add_task(size_t threadid,task_t* task)
		{
			thread_t* th = unsafe_do_find_thread(threadid,0);
			if (!th)
				return;
			if (!th->is_started())
				th->start();
			th->add(task);
		}

		static void on_finded_then_add_task(thread_t* thread,task_t* task)
		{
			if (!thread || !task)
				return;
			if (!thread->is_started())
				thread->start();
			thread->add(task);
		}

		void do_add_thread(thread_t* thread,added_d on_added)
		{
			on_event_thread_added internal_on_added (this,on_added); 
			base::do_add_thread(thread,internal_on_added);
		}
	};

} ///end of namespace ox
