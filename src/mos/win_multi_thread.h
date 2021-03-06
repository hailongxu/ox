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
#include "win_queue_thread.h"
#include "task_feedback.h"



#pragma once




___namespace2_begin(ox,mos)


template <typename thread_tn>
struct win_multi_threads
{
	typedef win_multi_threads self;

	typedef win_queue_thread thread_control_t;
	typedef thread_control_t::task_t task_t;
	typedef thread_tn thread_t;
	typedef typename thread_t::function_t function_t;
	typedef std::vector<thread_t*> thread_vector;
	typedef std::vector<HANDLE> handle_vector;
	typedef thread_task_helper tasker_t;
	typedef delegate<void(thread_t*)> added_d;
	typedef delegate<bool(thread_t*)> action_d;
	typedef delegate<void(size_t)> size_d;
	typedef multi_delegate<void(thread_t*,size_t)> exiting_d;
	typedef multi_delegate<void()> exited_d;
	typedef delegate<void()> unsafe_done_d;
	typedef delegate<void(void const*)> on_stopped_d;

	struct stop_handle
	{
		stop_handle():handle(0){}
		void* handle; /// event handle
		void close() {handle?CloseHandle(handle),handle=0:0;}
	};


	~win_multi_threads()
	{
		free_threads();
	}
	self()
	{
		_m_thread = 0;
		_m_is_sub_exiting = 0;
		set_id(0,0);
	}
	self(thread_control_t* thread)
	{
		_m_thread = thread;
		_m_is_sub_exiting = 0;
		set_id(0,0);
	}
	self(size_t your_id,char const* your_name)
	{
		set_id(your_id,your_name);
	}

	void set_id(size_t your_id,char const* your_name=0)
	{
		_m_your_id = your_id;
		memset(_m_your_name,0,sizeof(_m_your_name));
		if (your_name)
		{
			size_t len = strlen(your_name);
			if (len>31) len=31;
			memmove(_m_your_name,your_name,len);
		}
	}

	void bind(thread_control_t* thread)
	{
		_m_thread = thread;
	}
	void unbind()
	{
		_m_thread = 0;
	}
	thread_control_t* thread() const
	{
		return _m_thread;
	}

	void add_thread(thread_t* thread)
	{
		add_thread(thread,added_d());
	}

	void add_thread(thread_t* thread,added_d on_added)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			do_add_thread(thread,on_added);
		else
			_m_thread->add(tasker_t::make(this,&self::do_add_thread<thread_t,added_d>,thread,on_added));
	}

	struct on_thread_proc_init
	{
		on_thread_proc_init(function_t const& f):fun(f) {}
		function_t fun;
		void operator()(thread_t* th)
		{
			th->on_run().assign(fun);
		}
	};

	template <typename c>
	thread_t* create_thread(c* f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		function_t runf(f);
		thread_t* th = create_thread(runf,your_id,your_name,bstart);
		return th;
	}

	template <typename c, typename f>
	thread_t* create_thread(c* _c, f _f,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		function_t runf(_c,_f);
		thread_t* th = create_thread(runf,your_id,your_name,bstart);
		return th;
	}

	thread_t* create_thread(function_t const& fun,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		on_thread_proc_init fun_init(fun);
		delegate<void(thread_t*)> init(&fun_init);
		thread_t* th = create_thread_with_event(init,your_id,your_name,bstart);
		return th;
	}

	thread_t* create_thread(size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		return create_thread_with_event(delegate<void(thread_t*)>(),your_id,your_name,bstart);
	}

	void async_add(task_t* task)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			task->run();
		else
			_m_thread->add(task);
	}

	void try_create_thread(size_t thread_max_size,bool bstart=true,size_t your_threadid=-1)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			do_try_create_thread(bstart,thread_max_size,your_threadid);
		else
			_m_thread->add(tasker_t::make(this,&self::do_try_create_thread,thread_max_size,bstart,your_threadid));
	}

	thread_t* afind(size_t threadid,task_t* task)
	{
		assert (_m_thread);
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
		{
			return unsafe_do_find_thread(threadid,task);
		}
		_m_thread->add(tasker_t::make(this,&self::unsafe_do_find_thread_withnoreturn,threadid,task));
		return 0;
	}
	bool astop()
	{
		ox::utl::win_debug().let_format("win_multi_thread astop called\n");
		if (_m_is_sub_exiting>0)
		{
			ox::utl::win_debug().let_format("win_multi_thread return FALSE[%d]; control[id:%X,yname:%s,yid:%u]\n",
				_m_is_sub_exiting.value(),_m_thread->threadid(),_m_thread->your_name(),_m_thread->your_id());
			return false;
		}
		ox::utl::win_debug().let_format("win_multi_thread [%d] start to call internal_astop; control[id:%X,yname:%s,yid:%u]\n",
			_m_is_sub_exiting.value(),_m_thread->threadid(),_m_thread->your_name(),_m_thread->your_id());
		_m_is_sub_exiting = 1;
		HANDLE hstop = win_manual_event::create();
		_m_stop_handle.handle = hstop;
		return internal_astop(on_stopped_event,hstop);
	}
	void stop(size_t timeout_ms=-1)
	{
		bool b = astop();
		wait_subs(timeout_ms);
	}
	void stop_tid(size_t threadid)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			unsafe_do_stop_tid(threadid);
		else if (!_m_thread->is_exiting())
			_m_thread->add(tasker_t::make(this,&self::unsafe_do_stop_tid,threadid));
	}

	/// you should call astop, and then call wait, or else, there will be some wrong here maybe
	void wait_subs(size_t timeout_ms=-1)
	{
		assert (_m_is_sub_exiting!=0);
		/// wait handles
		scope_auto_event::wait(_m_stop_handle.handle,timeout_ms);
		_m_stop_handle.close();
	}

	void foreach_do(action_d action)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			do_foreach_do(action);
		else
			_m_thread->add(tasker_t::make(this,&self::do_foreach_do,action));
	}

	void async_size(size_d const& on_size_return)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			do_size(on_size_return);
		else
			_m_thread->add(tasker_t::make(this,&self::do_size,on_size_return));
	}
	size_t unsafe_size() const
	{
		return _m_thread_vector.size();
	}

	bool remove_thread(thread_t* th)
	{
		typedef thread_vector::iterator I;
		for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
		{
			if (*i==th)
			{
				_m_thread_vector.erase(i);
				return true;
			}
		}
		return false;
	}
	void move_to_exit_queue(thread_t* thread)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			do_move_to_exit_queue(thread);
		else
			_m_thread->add(tasker_t::make(this,&self::do_move_to_exit_queue,thread));
	}

	exiting_d& on_exiting() {return _m_on_exiting;}
	exited_d& on_exited() {return _m_on_exited;}
	exiting_d const& on_exiting() const {return _m_on_exiting;}
	exited_d const& on_exited() const {return _m_on_exited;}

private:
	delegate_closure<void(void const*)> _m_on_stopped;
	exited_d _m_on_exited;
	exiting_d _m_on_exiting;
	stop_handle _m_stop_handle;

	thread_t* create_thread_with_event(delegate<void(thread_t*)>& on_thread_newed,size_t your_id=-1,char const* your_name=0,bool bstart=true)
	{
		thread_t* th = internal_create_thread(your_id,your_name);
		if (!th) return 0;
		//thread_t::exit_d exit_event(this,&self::on_thread_start_to_exit);
		//if (th->on_exit()!=exit_event) th->on_exit() = exit_event;
		if (!on_thread_newed.is_empty()) on_thread_newed(th);
		add_thread(th,bstart?added_d(event_start_thread):added_d());
		return th;
	}
	static void event_start_thread(thread_t* th)
	{
		assert (th);
		th->start();
	}

	struct action_get_thread_handles
	{
		action_get_thread_handles(std::vector<HANDLE>& handles)
			: thread_handles(handles)
		{}
		bool operator()(thread_t* thread)
		{
			thread_handles.push_back(thread->create_thread_handle());
			return true;
		}
		std::vector<HANDLE>& thread_handles;
	};
	struct destructor
	{
		destructor(HANDLE handle)
			: _m_handle(handle)
		{}
		~destructor()
		{
			SetEvent(_m_handle);
		}
		HANDLE _m_handle;
	};
	void do_wait_subs(HANDLE hevent,std::vector<HANDLE>* handles,size_t timeout_ms)
	{
		assert (handles);
		destructor free_handle(hevent);
		if (unsafe_size()==0)
			return;
		action_get_thread_handles action(*handles);
		do_foreach_do(action_d(&action));
	}
	//void do_get_sub_handles(HANDLE hwait,handle_vector* handles)
	//{
	//	for (handle_vector::iterator i=_m_handle_vector_for_stop.begin();i!=_m_handle_vector_for_stop.end();++i)
	//		if (*i) handles->push_back(*i);
	//	win_auto_event::set_signaled(hwait);
	//}

	void do_size(size_d const& on_size_return)
	{
		if (on_size_return.is_empty())
			return;
		on_size_return(_m_thread_vector.size());
	}

	void do_try_create_thread(size_t thread_max_size,bool bstart=true,size_t your_threadid=-1)
	{
		if (_m_thread_vector.size()>=thread_max_size)
			return;
		create_thread(0,your_threadid,bstart);
	}

	void unsafe_do_find_thread_withnoreturn(size_t threadid,task_t* task)
	{
		unsafe_do_find_thread(threadid,task);
	}

	struct find_action_t
	{
		find_action_t() : _m_task(0) {}
		find_action_t(task_t* task) : _m_task(task) {}
		template <typename t>
		void operator()(t)
		{
			if (_m_task)
				_m_task->run();
		}
		task_t* _m_task;
	};
	void do_move_to_exit_queue(thread_t* thread)
	{
		typedef typename thread_vector::iterator I;
		I i = do_find_thread_vect_tt(_m_exited_vector,thread,find_action_t());
		if (i==_m_exited_vector.end())
			_m_exited_vector.push_back(thread);
		//{
		//	//assert (false);
		//	//DebugBreak();
		//}
		i = do_find_thread_vect_tt(_m_thread_vector,thread,find_action_t());
		//if (i==_m_thread_vector.end())
		//{
		//	//assert (false);
		//	//DebugBreak();
		//}
		if (i!=_m_thread_vector.end())
			_m_thread_vector.erase(i);
	}
	typename thread_vector::iterator do_find_thread_vect(size_t threadid,task_t* task)
	{
		return do_find_thread_vect_tt(_m_thread_vector,threadid,find_action_t(task));
	}
	template <typename action_tn>
	typename thread_vector::iterator do_find_thread_vect_tt(thread_vector& vec,size_t threadid,action_tn action)
	{
		typedef thread_vector::iterator I;
		for (I i=vec.begin();i!=vec.end();++i)
		{
			thread_t& th = **i;
			if (th.threadid()==threadid)
			{
				action(i);
				return i;
			}
		}
		return vec.end();
	}
	template <typename action_tn>
	typename thread_vector::iterator do_find_thread_vect_tt(thread_vector& vec,thread_t* thread,action_tn action)
	{
		typedef thread_vector::iterator I;
		for (I i=vec.begin();i!=vec.end();++i)
		{
			thread_t* th = *i;
			if (th==thread)
			{
				action(i);
				return i;
			}
		}
		return vec.end();
	}

	void unsafe_do_stop()
	{
		ox::utl::win_debug().let_format("win_multi_thread.unsafe_do_stop current:%X, control-thread[id:%X,yname:%s,yid:%u]\n",
			GetCurrentThreadId(),_m_thread->threadid(),_m_thread->your_name(),_m_thread->your_id());
	
		long old_count = _m_is_sub_exiting.add_and_return_old(_m_thread_vector.size());
		assert (old_count==1);
		if (_m_thread_vector.empty())
		{
			do_on_exited();
			return;
		}
		typedef thread_vector::iterator I;
		for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
		{
			thread_t& th = **i;
			ox::utl::win_debug().let_format(
				"win_multi_thread.unsafe_do_stop control-thread[id:%X,yname:%s,yid:%u], "
				"stop_next(id:%X,yname:%s,yid:%u)\n",
				_m_thread->threadid(),_m_thread->your_name(),_m_thread->your_id(),
				th.threadid(),th.your_name(),th.your_id());
			th.stop_next();
		}
	}

	void unsafe_do_stop_tid(size_t threadid)
	{
		thread_vector::iterator i = do_find_thread_vect(threadid,0);
		if (i==_m_thread_vector.end()) return;
		thread_t* th = *i;
		th->stop_next();
		_m_thread_vector.erase(i);
	}

	void do_foreach_do(action_d action)
	{
		typedef thread_vector::iterator I;
		for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
		{
			thread_t* th = *i;
			if (!action(th)) break;
		}
	}

	static void on_finded_then_add_task(thread_t* thread,task_t* task)
	{
		if (!thread || !task)
			return;
		if (!thread->is_started())
			thread->start();
		thread->add(task);
	}

	/// run in sub-thread
	void on_thread_to_destroy(thread_t* thread)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid()
			|| _m_thread->is_exiting()) /// this condition must be is exiting by terminate
			/// or we set a flag that indicate the exit is done by terminate
			do_on_thread_destroy(thread->threadid());
		else
			_m_thread->add(tasker_t::make(this,&self::do_on_thread_destroy,thread->threadid()));
	}
	/// run in sub-thread
	void on_thread_start_to_exit(thread_t* th)
	{
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid()
			|| _m_thread->is_exiting()) /// this condition must be is exiting by terminate
										/// or we set a flag that indicate the exit is done by terminate
			do_on_thread_exiting(th);
		else
			_m_thread->add(tasker_t::make(this,&self::do_on_thread_exiting,th));
	}

	void do_on_thread_exiting(thread_t* thread)
	{
		ox::utl::win_debug().let_format(
			"win_multi_thread.do_on_thread_exiting(th^:%p,id:%X,yname:%s,yid:%id)\n",
			thread,thread->threadid(),thread->your_name(),thread->your_id());
		do_move_to_exit_queue(thread);
		_m_on_exiting(thread,_m_thread_vector.size()+1);
		if (_m_thread_vector.empty()) do_on_exited();
	}
	void do_on_thread_destroy(size_t threadid)
	{
		typename thread_vector::iterator i = do_find_thread_vect_tt(_m_exited_vector,threadid,find_action_t());
		if (_m_exited_vector.end()==i) return;
		thread_t* th = *i;
		delete th;
		_m_exited_vector.erase(i);
	}

	/// run in control thread
	void do_on_exited()
	{
		if (!_m_thread_vector.empty())
		{
			assert(_m_thread_vector.empty());
			/// some thing FITAL error happens
			DebugBreak();
		}

		//_m_is_sub_exiting = -1; /// exited all
		handle_vector handles;
		for (thread_vector::iterator i=_m_exited_vector.begin();i!=_m_exited_vector.end();++i)
		{
			thread_t& th = **i;
			HANDLE h = th.create_thread_handle();
			if (h!=0) handles.push_back(h);
		}
		if (!handles.empty())
			::WaitForMultipleObjects(handles.size(),&handles.front(),TRUE,-1/*timeout_ms*/);
		/// here, all the threads has been closed, no any in OS
		for (handle_vector::iterator i=handles.begin();i!=handles.end();++i)
			CloseHandle(*i);

		/// so, we can set the flag to sub exiting to zero
		ox::utl::win_debug().let_format("win_multi_thread current:%X, control-thread[id:%X,yname:%s,yid:%u]: do_on_exited\n",
			GetCurrentThreadId(),_m_thread->threadid(),_m_thread->your_name(),_m_thread->your_id());
		_m_is_sub_exiting = -1; /// to the original state
		if (!_m_on_stopped.is_empty())
			_m_on_stopped(); /// the wait handle is set to signal
		if (!_m_on_exited.is_empty())
			_m_on_exited();
		/// delete all thread objects
		for (thread_vector::iterator i=_m_exited_vector.begin();i!=_m_exited_vector.end();++i)
		{
			thread_t* th = *i;
			size_t tid = th->threadid();
			ox::utl::win_debug().let_format("delete thread[id:%X,your-name:%s,your-id:%u]\n",
				tid,th->your_name(),th->your_id());
			delete th;
		}
		_m_exited_vector.clear();
	}
	bool internal_astop(on_stopped_d const& event_stopped,HANDLE param)
	{
		_m_on_stopped.assign(event_stopped);
		_m_on_stopped.assign_param<0>(param);
		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
			unsafe_do_stop();
		else if (!_m_thread->is_exiting())
			_m_thread->add(tasker_t::make(this,&self::unsafe_do_stop));
		return true;
	}

	static void on_stopped_event(void const* handle)
	{
		/// here, the handles has been prepared
		ox::utl::win_debug().let_format("set_signaled to wait handle\n");
		win_manual_event::set_signaled((HANDLE)handle);
	}

	void free_threads()
	{
		assert(_m_is_sub_exiting<=0);
		/// -1, means no sub threads
		/// 0: maybe occurs some error
		if (_m_is_sub_exiting>0) DebugBreak();
		typedef thread_vector::iterator I;
		for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
			delete *i;
		_m_thread_vector.clear();
	}

//#if 0
//	void unsafe_terminate(unsafe_done_d const& on_done)
//	{
//		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
//			unsafe_do_terminate(on_done);
//		else if (!_m_thread->is_exiting())
//			_m_thread->add(tasker_t::make(this,&self::unsafe_do_terminate,on_done));
//	}
//	void unsafe_terminate_tid(size_t threadid)
//	{
//		if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
//			unsafe_do_terminate_tid(threadid);
//		else if (!_m_thread->is_exiting())
//			_m_thread->add(tasker_t::make(this,&self::unsafe_do_terminate_tid,threadid));
//	}
//	void unsafe_do_terminate(unsafe_done_d const& on_done)
//	{
//		/// should call the on_done before line(*****)
//		/// because unsafe terminate will call the control thread
//		/// add, so, we should ensure, the control be notifed first
//		/// and the add action will do nothing, all because the 
//		/// critical section, wont be release when called by the
//		/// thread terminated by the caller thread (maybe control thread).
//		/// the next line on done -------
//		if (!on_done.is_empty()) on_done();
//
//		if (_m_thread_vector.empty())
//		{
//			do_on_exited();
//			return;
//		}
//		/// ***** in case of th->unsafe_terminate() call stop next
//		typedef thread_vector::iterator I;
//		thread_vector threads = _m_thread_vector;
//		for (I i=threads.begin();i!=threads.end();++i)
//		{
//			thread_t* th = _m_thread_vector.front();
//			size_t threadid = th->threadid();
//			th->on_exit().clear(); /// detach connection with control thread
//			th->unsafe_terminate();
//		}
//		/// in case of th->unsafe_terminate() call terminate
//		_m_thread_vector.clear();
//		do_on_exited();
//	}
//
//	void unsafe_do_terminate_tid(size_t threadid)
//	{
//		thread_vector::iterator i = do_find_thread_vect(threadid,0);
//		if (i==_m_thread_vector.end()) return;
//		thread_t* th = *i;
//		th->unsafe_terminate();
//		/// again
//		i = do_find_thread_vect(threadid,0);
//		if (i==_m_thread_vector.end()) return;
//		_m_thread_vector.erase(i);
//	}
//#endif


protected:
	thread_t* unsafe_do_find_thread(size_t threadid,task_t* task)
	{
		thread_vector::iterator i = do_find_thread_vect(threadid,task);
		return i==_m_thread_vector.end()? 0:*i;
	}
	thread_t* internal_create_thread(size_t your_id=-1,char const* your_name=0)
	{
		if (your_id==-1) your_id = _m_your_id;
		if (your_name==0) your_name = _m_your_name;
		thread_t* th = new thread_t(your_id,your_name);
		assert(th && "create thread error because of not enough memory");
		return th;
	}
	template <typename thread_tn,typename added_d_tn>
	void do_add_thread(thread_tn* thread,added_d_tn on_added)
	{
		typedef thread_tn thread_t;
		assert(thread);
		thread_t* th = 0;
		if (thread->threadid()!=-1)
			th = unsafe_do_find_thread(thread->threadid(),0);
		if (th==0)
		{
			_m_thread_vector.push_back(thread);
			internal_sort_threads();
			//thread->on_exit_before().assign(this,&self::on_thread_exit_before);
			thread_t::exit_d exit_event(this,&self::on_thread_start_to_exit);
			thread_t::destroy_d destroy_event(this,&self::on_thread_to_destroy);
			if (thread->on_exit()!=exit_event) thread->on_exit() = exit_event; // this action is not very safe
			thread->on_destroy() = destroy_event;
			if (!on_added.is_empty())
				on_added(thread);
		}
		else
		{
			assert(false && "the thread has in");
		}
	}

private:
	static bool is_less(thread_t* a,thread_t* b)
	{
		return a->threadid()<b->threadid();
	}
	void internal_sort_threads()
	{
		//std::sort(_m_thread_vector.begin(),_m_thread_vector.end(),is_less);
	}
	size_t internal_find(size_t threadid)
	{
		//std::binary_search(_m_thread_vector.begin(),_m_thread_vector.end(),threadid,is_less);
	}

protected:
	thread_control_t* _m_thread;
private:
	atomic_long _m_is_sub_exiting;
	size_t _m_your_id;
	char _m_your_name[32];
	thread_vector _m_thread_vector;
	thread_vector _m_exited_vector;
};


___namespace2_end()
