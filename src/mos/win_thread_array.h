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
#include "win_thread.h"
#include "task_feedback.h"



#pragma once




___namespace2_begin(ox,mos)


template <typename thread_tn>
struct win_thread_array
{
	typedef win_thread_array self;

	typedef thread_tn thread_t;
	typedef typename thread_t::function_t function_t;
	typedef std::vector<thread_t*> thread_vector;
	typedef typename thread_vector::iterator thi;
	typedef thread_task_helper tasker_t;
	typedef delegate<void(thread_t*)> added_d;
	typedef multi_delegate<void(thread_t*,size_t)> exiting_d;
	typedef multi_delegate<void()> exited_d;


	~win_thread_array()
	{
		scope_cs lock(_m_cs_vector);
		_m_thread_vector.clear();
	}
	self()
	{
	}
	bool add_thread(thread_t* thread)
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_add_thread(thread);
	}
	bool unsafe_add_thread(thread_t* thread)
	{
		if (!thread)
			return false;
		assert(thread);
		thread_t* th = unsafe_find(thread->threadid());
		if (th)
			return false;
		thread->register_destroy(delete_thread);
		thread->on_exit().assign(this,&self::on_thread_exiting);
		_m_thread_vector.push_back(thread);
		return true;
	}

	void erase_thread(size_t threadid)
	{
		thi i = unsafe_find(threadid);
		if (i==_m_thread_vector.end())
			return;
		thread_t* th = *i;
		th->stop();
		th->join();
		_m_thread_vector.erase(i);
	}

	thread_t* find(size_t threadid)
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_find(threadid);
	}

	thread_t* unsafe_find(size_t threadid)
	{
		thi i = unsafe_ifind(threadid);
		if (i==_m_thread_vector.end())
			return 0;
		return *i;
	}

	thi ifind(size_t threadid)
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_find(threadid);
	}
	thi unsafe_ifind(size_t threadid)
	{
		typedef thread_vector::iterator I;
		for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
		{
			thread_t& th = **i;
			if (th.threadid()==threadid)
				return i;
		}
		return _m_thread_vector.end();
	}

	size_t size() const
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_size();
	}
	size_t unsafe_size() const
	{
		return _m_thread_vector.size();
	}
	bool is_empty() const
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_is_empty();
	}
	bool unsafe_is_empty() const
	{
		return _m_thread_vector.empty();
	}

	template <typename c>
	thread_t* create_and_add_thread(c* f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_create_and_add_thread(f,your_id,your_name,bstart);
	}
	template <typename c>
	thread_t* unsafe_create_and_add_thread(c* f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		thread_t* th = create_thread(f,your_id,your_name,bstart);
		unsafe_add_thread(th);
		return th;
	}

	template <typename c>
	thread_t* create_thread(c* f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		function_t runf(f);
		thread_t* th = create_thread(runf,your_id,your_name,bstart);
		return th;
	}

	template <typename c, typename f>
	thread_t* create_and_add_thread(c* _c, f _f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_create_and_add_thread(_c,_f,your_id,your_name,bstart);
	}
	template <typename c, typename f>
	thread_t* unsafe_create_and_add_thread(c* _c, f _f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		thread_t* th = create_thread(_c,_f,your_id,your_name,bstart);
		unsafe_add_thread(th);
		return th;
	}

	template <typename c, typename f>
	thread_t* create_thread(c* _c, f _f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		function_t runf(_c,_f);
		thread_t* th = create_thread(runf,your_id,your_name,bstart);
		return th;
	}

	thread_t* create_and_add_thread(function_t const& fun,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		scope_cs lock(_m_cs_vector);
		return unsafe_create_and_add_thread(fun,your_id,your_name,bstart);
	}

	thread_t* unsafe_create_and_add_thread(function_t const& fun,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		thread_t* th = create_thread(fun,your_id,your_name,bstart);
		unsafe_add_thread(th);
		return th;
	}

	thread_t* create_thread(function_t const& fun,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		thread_t* th = new thread_t(your_id,your_name);
		assert(th && "create thread error because of not enough memory");
		th->on_run().assign(fun);
		if (bstart)
			th->start();
		return th;
	}

	thread_t* create_thread(size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		thread_t* th = new thread_t(your_id,your_name);
		assert(th && "create thread error because of not enough memory");
		if (bstart)
			th->start();
		return th;
	}


	void stop_all(size_t timeout_ms=-1)
	{
		typedef std::vector<HANDLE> handle_vector;

		handle_vector handles;
		{
			scope_cs lock(_m_cs_vector);
			for (thi i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
			{
				thread_t* th = *i;
				HANDLE h = th->create_thread_handle();
				handles.push_back(h);
				th->stop_next();
			}
			if (handles.empty())
				return;
		}

		::WaitForMultipleObjects(handles.size(),&handles.front(),TRUE,timeout_ms);

		/// close all the handles created at the top of the function
		for (std::vector<HANDLE>::iterator i=handles.begin();i!=handles.end();++i)
		{
			HANDLE h = *i;
			CloseHandle(h);
		}
		
		// trigger the exited event
		if (!_m_on_exited.is_empty())
			_m_on_exited();
	}

	void stop_notified(size_t threadid)
	{
		scope_cs lock(_m_cs_vector);
		unsafe_stop_notified(threadid);
	}
	void unsafe_stop_notified(size_t threadid)
	{
		thi i = find(threadid);
		if (i==_m_thread_vector.end())
			return;
		thread_t* th = *i;
		_m_thread_vector.erase(i);
		th->stop_next();
	}

	exiting_d& on_exiting()
	{
		return _m_on_exiting;
	}
	exiting_d const& on_exiting() const
	{
		return _m_on_exiting;
	}
	exited_d& on_exited()
	{
		return _m_on_exited;
	}
	exited_d const& on_exited() const
	{
		return _m_on_exited;
	}

private:
	void on_thread_exiting(thread_t* th)
	{
		scope_cs lock(_m_cs_vector);
		thi i = unsafe_ifind(th->threadid());
		if (i==_m_thread_vector.end())
			return;
		_m_thread_vector.erase(i);
	}
	static void delete_thread(void* thread)
	{
		thread_t* th = (thread_t*)thread;
		ox::utl::win_debug().let_format(
			"win_thread_array.delete_thread(th^:%p,id:%X,yname:%s,yid:%id)\n",
			th,th->threadid(),th->your_name(),th->your_id());
		delete th;
	}

protected:
	critical_section _m_cs_vector;
private:
	exiting_d _m_on_exiting;
	exited_d _m_on_exited;
	thread_vector _m_thread_vector;
};

___namespace2_end()
