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
#include "../cxx/delegate.h"
#include "../cxx/multi_delegate.h"
#include "atomic_number.h"
#include "scope_raii.h"
#include "win_thread.h"
#include "win_queue_thread.h"
#include "task_feedback.h"



#pragma once




namespace ox
{
	template <typename thread_tn>
	struct win_multi_threads
	{
		typedef win_multi_threads self;

		typedef win_queue_thread thread_control_t;
		typedef thread_control_t::task_t task_t;
		typedef thread_tn thread_t;
		typedef typename thread_t::function_t function_t;
		typedef std::vector<thread_t*> thread_vector;
		typedef ox::task_single<int> tasker_t;
		typedef delegate<void(thread_t*)> added_d;
		typedef delegate<bool(thread_t*)> action_d;
		typedef delegate<void(size_t)> size_d;
		typedef multi_delegate<void(thread_t*,size_t)> exiting_d;
		typedef multi_delegate<void(void)> exited_d;
		typedef delegate<void()> unsafe_done_d;


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
			if (_m_is_sub_exiting) return false;
			_m_is_sub_exiting = 1;
			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				unsafe_do_stop();
			else if (!_m_thread->is_exiting())
				_m_thread->add(tasker_t::make(this,&self::unsafe_do_stop));
			return true;
		}
		void stop(size_t timeout_ms=-1)
		{
			astop(); 
			wait_subs(timeout_ms);
		}
		void stop_tid(size_t threadid)
		{
			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				unsafe_do_stop_tid(threadid);
			else if (!_m_thread->is_exiting())
				_m_thread->add(tasker_t::make(this,&self::unsafe_do_stop_tid,threadid));
		}

		void wait_subs(size_t timeout_ms=-1)
		{
			printf("wait_subs\n");
			HANDLE hwait = CreateEvent(0,FALSE,FALSE,0);
			std::vector<HANDLE>* handles = new std::vector<HANDLE>;
			assert (handles);
			if (_m_thread==0 || _m_thread->is_exiting() || ::GetCurrentThreadId()==_m_thread->threadid())
				do_wait_subs(hwait,handles,timeout_ms);
			else
				_m_thread->add(tasker_t::make(this,&self::do_wait_subs,hwait,handles,timeout_ms));
			DWORD id = WaitForSingleObject(hwait,timeout_ms);
			if (id==WAIT_OBJECT_0)
			{
				printf("get handles\n");
				if (!handles->empty())
				{
					::WaitForMultipleObjects(handles->size(),&handles->front(),TRUE,timeout_ms);
					printf("wait handles finished\n");
				}
			}
			else assert(false && "wait hevent error");
			::CloseHandle(hwait);
			printf("close handles\n");
			for (std::vector<HANDLE>::iterator i=handles->begin();i!=handles->end();++i)
				CloseHandle(*i);
			delete handles;
			printf("thread vector is empty\n");
			assert (_m_thread_vector.empty());
			//if (!_m_thread_vector.empty()) DebugBreak();
			_m_is_sub_exiting = 0;
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


		exiting_d& on_exiting() {return _m_on_exiting;}
		exited_d& on_exited() {return _m_on_exited;}
		exiting_d const& on_exiting() const {return _m_on_exiting;}
		exited_d const& on_exited() const {return _m_on_exited;}

	private:
		exited_d _m_on_exited;
		exiting_d _m_on_exiting;

		thread_t* create_thread_with_event(delegate<void(thread_t*)>& on_thread_newed,size_t your_id=-1,char const* your_name=0,bool bstart=true)
		{
			thread_t* th = internal_create_thread(your_id,your_name);
			if (!th) return 0;
			thread_t::exit_d exit_event(this,&self::on_thread_start_to_exit);
			if (th->on_exit()!=exit_event) th->on_exit() = exit_event;
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
				printf("get thread handle\n");
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

		typename thread_vector::iterator do_find_thread_vect(size_t threadid,task_t* task)
		{
			typedef thread_vector::iterator I;
			for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
			{
				thread_t& th = **i;
				if (th.threadid()==threadid)
				{
					if (task)
						task->run();
					return i;
				}
			}
			return _m_thread_vector.end();
		}

		void unsafe_do_stop()
		{
			_m_is_sub_exiting = _m_thread_vector.size();
			if (_m_thread_vector.empty())
			{
				do_on_exited();
				return;
			}
			typedef thread_vector::iterator I;
			for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
			{
				thread_t& th = **i;
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
		void on_thread_start_to_exit(thread_t* th)
		{
			printf("on_thread_start_to_exit\n");

			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid()
				|| _m_thread->is_exiting()) /// this condition must be is exiting by terminate
											/// or we set a flag that indicate the exit is done by terminate
				do_on_thread_exiting(th);
			else
				_m_thread->add(tasker_t::make(this,&self::do_on_thread_exiting,th));
		}

		void do_on_thread_exiting(thread_t* thread)
		{
			printf("do_on_thread_exiting\n");
			_m_on_exiting(thread,_m_thread_vector.size());
			typedef thread_vector::iterator I;
			I i = do_find_thread_vect(thread->threadid(),0);
			if (i!=_m_thread_vector.end())
			{
				//delete *i;
				_m_thread_vector.erase(i);
			}
			if (!_m_thread_vector.empty()) return;

			//if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				do_on_exited();
			//else
			//	_m_thread->add(tasker_t::make(this,&self::do_on_exited));
		}

		void do_on_exited()
		{
			printf("do_on_exited\n");
			//free_threads();
			assert(_m_thread_vector.empty());
			if (!_m_on_exited.is_empty())
				_m_on_exited();
		}

		void free_threads()
		{
			assert(!_m_is_sub_exiting);
			if (_m_is_sub_exiting) DebugBreak();
			typedef thread_vector::iterator I;
			for (I i=_m_thread_vector.begin();i!=_m_thread_vector.end();++i)
				delete *i;
			_m_thread_vector.clear();
		}

#if 0
		void unsafe_terminate(unsafe_done_d const& on_done)
		{
			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				unsafe_do_terminate(on_done);
			else if (!_m_thread->is_exiting())
				_m_thread->add(tasker_t::make(this,&self::unsafe_do_terminate,on_done));
		}
		void unsafe_terminate_tid(size_t threadid)
		{
			if (_m_thread==0 || ::GetCurrentThreadId()==_m_thread->threadid())
				unsafe_do_terminate_tid(threadid);
			else if (!_m_thread->is_exiting())
				_m_thread->add(tasker_t::make(this,&self::unsafe_do_terminate_tid,threadid));
		}
		void unsafe_do_terminate(unsafe_done_d const& on_done)
		{
			/// should call the on_done before line(*****)
			/// because unsafe terminate will call the control thread
			/// add, so, we should ensure, the control be notifed first
			/// and the add action will do nothing, all because the 
			/// critical section, wont be release when called by the
			/// thread terminated by the caller thread (maybe control thread).
			/// the next line on done -------
			if (!on_done.is_empty()) on_done();

			if (_m_thread_vector.empty())
			{
				do_on_exited();
				return;
			}
			/// ***** in case of th->unsafe_terminate() call stop next
			typedef thread_vector::iterator I;
			thread_vector threads = _m_thread_vector;
			for (I i=threads.begin();i!=threads.end();++i)
			{
				thread_t* th = _m_thread_vector.front();
				size_t threadid = th->threadid();
				th->on_exit().clear(); /// detach connection with control thread
				th->unsafe_terminate();
			}
			/// in case of th->unsafe_terminate() call terminate
			_m_thread_vector.clear();
			do_on_exited();
		}

		void unsafe_do_terminate_tid(size_t threadid)
		{
			thread_vector::iterator i = do_find_thread_vect(threadid,0);
			if (i==_m_thread_vector.end()) return;
			thread_t* th = *i;
			th->unsafe_terminate();
			/// again
			i = do_find_thread_vect(threadid,0);
			if (i==_m_thread_vector.end()) return;
			_m_thread_vector.erase(i);
		}
#endif


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
				if (thread->on_exit()!=exit_event) th->on_exit() = exit_event; // this action is not very safe
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
	};

} ///end of namespace ox
