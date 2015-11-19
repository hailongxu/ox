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
#include "win_thread_array.h"
#include "safe_double_queue.h"
#include "task_feedback.h"



#pragma once




___namespace2_begin(ox,mos)


struct win_thread_pool_array
{
	typedef win_thread_pool_array self;
	typedef win_thread<unsigned()> thread_t;
	typedef win_thread_array<thread_t> multi_thread_t;
	typedef delegate<void(thread_t*)> started_d;
	typedef multi_thread_t::exiting_d exiting_d;
	typedef multi_thread_t::exited_d exited_d;

	typedef thread_task_t task_t;
	typedef safe_single_ptrqueue_tt<task_t> task_queue;
	typedef task_queue::object_ptr task_pptr;

	enum {__normalid,__highid,__exit};


	static size_t const __thread_count_max_default = -1;
	static size_t const __thread_count_min_defualt = 1;

	~win_thread_pool_array()
	{
		CloseHandle(_m_hcontrol[__exit]);
	}
	self()
	{
		_m_hcontrol[__normalid] = _m_normal_queue.handle_added();
		_m_hcontrol[__highid] = _m_high_queue.handle_added();
		_m_hcontrol[__exit] = CreateEvent(0,TRUE,FALSE,0);
	}

	size_t size() const
	{
		_m_multi_thread.size();
	}
	/// return threadid
	size_t create_thread()
	{
		return create_thread(-1,"");
	}
	size_t create_thread(size_t your_id,char const* your_name)
	{
		thread_t* th = _m_multi_thread.create_thread(your_id,your_name,false);
		if (!th)
			return 0;
		th->on_started().assign(this,&self::on_thread_started);
		th->on_run().assign(this,&self::pool_thread_run);
		_m_multi_thread.add_thread(th);
		th->start();
		return th->threadid();
	}

	void add(thread_task_t* task)
	{
		add_to_queue(task,false);
	}
	void add_high(thread_task_t* task)
	{
		add_to_queue(task,true);
	}

	void stop(size_t timeout_ms=-1)
	{
		SetEvent(_m_hcontrol[__exit]);
		_m_multi_thread.stop_all(timeout_ms);
	}
	void stop_notified()
	{
		SetEvent(_m_hcontrol[__exit]);
		_m_multi_thread.stop_notified_all();
	}
	void wait(size_t timeout_ms=-1)
	{
		_m_multi_thread.wait(timeout_ms);
	}

	started_d& on_started() {return _m_on_started;}
	exiting_d& on_exiting() {return _m_multi_thread.on_exiting();}
	exited_d& on_exited() {return _m_multi_thread.on_exited();}
	started_d const& on_started() const {return _m_on_started;}
	exiting_d const& on_exiting() const {return _m_multi_thread.on_exiting();}
	exited_d const& on_exited() const {return _m_multi_thread.on_exited();}

private:
	/// run in control thread
	void on_thread_started(thread_t* thread)
	{
		if (_m_on_started.is_empty()) return;
		_m_on_started(thread);
	}

	void add_to_queue(thread_task_t* task,bool is_high)
	{
		is_high?_m_high_queue.add(task):_m_normal_queue.add(task);
	}

	task_t* pop_task()
	{
		task_t* t = _m_high_queue.get();
		if (!t)
			t = _m_normal_queue.get();
		return t;
	}

	size_t wait_event() const
	{
		DWORD id = WaitForMultipleObjects(3,_m_hcontrol,FALSE,INFINITE);
		if (id==WAIT_OBJECT_0+0 || id==WAIT_OBJECT_0+1)
			return true;
		if (id==WAIT_FAILED || id==WAIT_OBJECT_0+2)
			return false;
		return false;
	}

	bool can_normal_task_run() const
	{
		return true;
	}

	/// pool thread proc
	unsigned pool_thread_run()
	{
		while(1)
		{
			bool bwaited = wait_event();
			if (!bwaited) /// to exit
			{
				return -1;
			}

			while (1)
			{
				task_t* runing = pop_task();
				if (!runing)
					break;
				runing->run();
				runing->destroy();
			}
		}
	}

private: /// data members
	started_d _m_on_started;
	multi_thread_t _m_multi_thread;
	task_queue _m_normal_queue;
	task_queue _m_high_queue;
	HANDLE _m_hcontrol[3];
};


___namespace2_end()
