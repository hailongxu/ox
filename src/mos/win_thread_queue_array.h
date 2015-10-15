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
#include "../ox/nsab.h"
#include "../cxx/delegate.h"
#include "../cxx/multi_delegate.h"
#include "atomic_number.h"
#include "scope_raii.h"
#include "task_feedback.h"
#include "win_thread_array.h"
#include "win_queue_thread.h"



#pragma once




___namespace2_begin(ox,mos)


struct win_thread_queue_array
	: win_thread_array<win_queue_thread>
{
	typedef win_thread_queue_array self;
	typedef win_thread_array<win_queue_thread> base;

	typedef base::thread_t thread_t;
	typedef thread_t::task_t task_t;
	typedef task_single<void> tasker_t;
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

	self()
	{}

	thread_t* create_and_add_thread(size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		thread_t* th = base::create_thread(your_id,your_name,false);
		if (!th)
			return 0;
		add_thread(th);
		if (bstart)
			th->start();
		return th;
	}

	void add_thread(thread_t* thread)
	{
		if (!thread->on_idle().is_empty())
			thread->on_idle().assign(this,&self::on_thread_idle);
		if (!thread->on_busy().is_empty())
			thread->on_busy().assign(this,&self::on_thread_busy);
		base::unsafe_add_thread(thread);
	}
	void add_task(task_t* task,size_t threadid)
	{
		scope_cs lock(base::_m_cs_vector);
		unsafe_add_task(task,threadid);
	}

	void unsafe_add_task(task_t* task,size_t threadid)
	{
		thread_t* th = base::unsafe_find(threadid);
		if (!th)
			return;
		if (!th->is_started())
			th->start();
		th->add(task);
	}

	idle_d& on_idle() {return _m_on_idle;}
	busy_d& on_busy() {return _m_on_busy;}
	idle_d const& on_idle() const {return _m_on_idle;}
	busy_d const& on_busy() const {return _m_on_busy;}

private:
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

private:
	idle_d _m_on_idle;
	busy_d _m_on_busy;
};


___namespace2_end()
