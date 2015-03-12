
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
#include <process.h>
#include "atomic_number.h"
#include "scope_raii.h"
#include "delegate_closure.h"
#include "task_feedback.h"



#pragma once



namespace ox
{
	template <typename function_decl_tn>
	struct win_thread
	{
		typedef win_thread self;
		typedef function_decl_tn function_decl;
		typedef delegate_closure<function_decl> function_closure;
		typedef typename function_closure::delegate_t function_t;
		typedef delegate<void(self*)> stop_d;
		typedef delegate<void(self*)> exit_d;
		typedef delegate<void(self*)> started_d;
		enum wait_enum
		{
			__okay=WAIT_OBJECT_0,
			__allowed=0, /// just for detective
			__notallowed,
			__on_exiting,
			__not_started,
			__abandoned=WAIT_ABANDONED,
			__timeout=WAIT_TIMEOUT,
			__failed=WAIT_FAILED
		};
		enum phrase_enum
		{
			__th_not_started = 0,
			__th_start_accepted,
			__th_about_to_start,
			__th_start_called_failly,
			__th_start_called_successfully,
			__th_running,
			__th_about_to_exit,
			__th_exited,
		};

	public:
		~win_thread()
		{
			assert(!is_running());
			if (is_running()) DebugBreak();
			//assert (_m_phrase!=__th_running);
			//if (_m_phrase==__th_running) DebugBreak();
		}
		win_thread(size_t your_id=0,char const* your_name=0)
		{
			_m_your_name[0]=0;
			init(your_name,your_id);
		}
		win_thread(function_t const& fun,size_t your_id=0,char const* your_name=0)
			: _m_function(fun)
		{
			_m_your_name[0]=0;
			init(your_name,your_id);
		}

		template <typename c>
		win_thread(c* f)
		{
			_m_your_name[0]=0;
			init();
			function.assign(f);
		}
		template <typename c, typename f>
		win_thread(c* _c, f _f)
		{
			_m_your_name[0]=0;
			init();
			function.assign(_c,_f);
		}
		win_thread(function_t const& d)
		{
			_m_your_name[0]=0;
			init();
			function.assign(d);
		}
		win_thread(function_closure const& o)
		{
			_m_your_name[0]=0;
			init();
			function.assign(o);
		}

		bool is_running() const
		{
			if (_m_threadid==(size_t)-1) return false;
			DWORD exit_code = 0;
			//BOOL b = GetExitCodeThread(_m_sudo_thread_handle,&exit_code);
			//assert (b);
			//return b && exit_code==STILL_ACTIVE;
			bool b = _m_phrase.value()==__th_start_called_successfully || _m_phrase.value()==__th_running;
			return b;
		}
		bool is_started() const
		{
			return _m_threadid != -1;
		}

		void set_id(size_t your_id,char const* your_name)
		{
			_m_your_id = your_id;
			if (your_name)
			{
				size_t len = strlen(your_name);
				if (len>31) len=31;
				memmove(_m_your_name,your_name,len);
				memset(_m_your_name+len,0,sizeof(_m_your_name)-len);
			}
		}
		char const* your_name() const
		{
			return _m_your_name;
		}
		size_t your_id() const
		{
			return _m_your_id;
		}
		size_t phrase() const
		{
			return _m_phrase.value();
		}

		bool start()
		{
			if (is_started())
				return true;
			_m_threadid = begin_thread();
			return is_started();
		}
		template <typename p1>
		bool start(p1 _p1)
		{
			if (is_started())
				return;
			_m_function.assign_param<0>(_p1);
			_m_threadid = begin_thread();
			return is_started();
		}
		bool start_here()
		{
			_m_threadid = GetCurrentThreadId();
			thread_start_param start_param = {0,this};
			_m_phrase = __th_about_to_start;
			thread_proc(&start_param);
			return is_started();
		}
		void stop_next()
		{
			if (_m_stop.is_empty()) return;
			_m_stop(this);
		}
		wait_enum join(size_t timeout_ms=-1)
		{
			bool is_allowed = (GetCurrentThreadId()!=_m_threadid);
			assert (is_allowed);
			if (!is_allowed) return __notallowed;
			//HANDLE handle = OpenThread(0,FALSE,_m_threadid);
			//if (handle==0) return __okay;
			//HANDLE handle = _m_sudo_thread_handle;
			wait_enum ret = join_thread_id(_m_threadid);
			//CloseHandle(handle);
			return ret;
		}
		static wait_enum join_thread_handle(HANDLE thread_hanlde,size_t timeout_ms=-1)
		{
			DWORD id = ::WaitForSingleObject(thread_hanlde,timeout_ms);
			return wait_enum(id);
		}
		static wait_enum join_thread_id(size_t threadid,size_t timeout_ms=-1)
		{
			HANDLE handle = OpenThread(SYNCHRONIZE,FALSE,threadid);
			if (handle==0) return __okay;
			DWORD id = ::WaitForSingleObject(handle,timeout_ms);
			CloseHandle(handle);
			return wait_enum(id);
		}

		//HANDLE sudo_handle() const
		//{
		//	return _m_sudo_thread_handle;
		//}

		size_t threadid() const
		{
			return _m_threadid;
		}
		/// you should remember to close the handle
		/// or else handle leaking will occur later
		HANDLE create_thread_handle() const
		{
			HANDLE handle = OpenThread(SYNCHRONIZE,FALSE,_m_threadid);
			return handle;
		}

		started_d& on_started() {return _m_on_started;}
		started_d const& on_started() const {return _m_on_started;}
		stop_d& to_stop() {return _m_stop;}
		stop_d const& to_stop() const {return _m_stop;}
		exit_d& on_exit() {return _m_on_exit;}
		exit_d const& on_exit() const {return _m_on_exit;}

		function_closure& on_run()
		{
			return _m_function;
		}

#if 0
		void unsafe_terminate()
		{
			TerminateThread(_m_sudo_thread_handle,0);
			if (!_m_on_exit.is_empty())
				_m_on_exit(this);
		}
#endif

	protected:
		unsigned run()
		{
			return _m_function();
		}

		struct thread_start_param
		{
			HANDLE _m_event_handle;
			self* const _m_this;
		};
		static unsigned __stdcall thread_proc(void* param)
		{
			thread_start_param* start_param = (thread_start_param*)param;
			self* me = start_param->_m_this;
			me->_m_phrase = __th_running;
			//me->_m_sudo_thread_handle = GetCurrentThread();
			win_manual_event::set_signaled(start_param->_m_event_handle);
			if (!me->_m_on_started.is_empty())
				me->_m_on_started(me);
#ifdef _DEBUG
			{
				char buff [128];
				sprintf (buff,"thread:[%s,%u] id:[%u] started\n",me->_m_your_name,me->_m_your_id,me->_m_threadid);
				OutputDebugStringA(buff);
			}
#endif
			unsigned exitcode = me->run();
			if (!me->_m_on_exit.is_empty())
				me->_m_on_exit(me);
#ifdef _DEBUG
			{
				char buff [128];
				sprintf (buff,"thread:[%s,%u] id:[%u] NORMALLY exited with:[%u]\n",me->_m_your_name,me->_m_your_id,me->_m_threadid,exitcode);
				OutputDebugStringA(buff);
			}
#endif
			me->_m_phrase = __th_about_to_exit;
			return exitcode;
		}

		/// return thread-id
		size_t begin_thread()
		{
#ifdef _DEBUG
			{
				char buff [128];
				sprintf (buff,"begin thread:[%s,%u]\n",_m_your_name,_m_your_id);
				OutputDebugStringA(buff);
			}
#endif
			_m_phrase = __th_start_accepted;
			uintptr_t h = uintptr_t(-1);
			size_t threadid = 0;
			{
				win_manual_event event_started(false);
				thread_start_param start_param = {event_started.handle(),this};
				_m_phrase = __th_about_to_start;
				h = _beginthreadex(0,0,thread_proc,&start_param,0,&threadid);
				if (h!=uintptr_t(-1))
				{
					_m_phrase = __th_start_called_failly;
					CloseHandle((HANDLE)h);
				}
				_m_phrase = __th_start_called_successfully;
				scope_manual_event lock(event_started);
			}
#ifdef _DEBUG
			{
				char buff [128];
				sprintf (buff,"begin thread :[%s,%u] return h:[%p] id:[%u]\n",_m_your_name,_m_your_id,h,threadid);
				OutputDebugStringA(buff);
			}
#endif
			return threadid;
		}

		void init(char const* your_name=0,size_t your_id=0)
		{
			_m_phrase = __th_not_started;
			init_handleid();
			_m_your_id = your_id;
			set_id(your_id,your_name);
		}

		void init_handleid()
		{ 
			//_m_sudo_thread_handle = HANDLE(-1); /// because reuturn -1 when beginthreadex failed
			_m_threadid = -1;
		}

		//void clear()
		//{
		//	_m_thread_handle = (HANDLE)-1;
		//	_m_threadid = -1;
		//	_m_your_id = -1;
		//	memset(_m_your_name,0,sizeof(_m_your_name));
		//}
	protected:
		atomic_long _m_phrase;
		//HANDLE _m_sudo_thread_handle;
		size_t _m_threadid;
		char _m_your_name[32];
		size_t _m_your_id;
		function_closure _m_function;
		//atomic_long _m_exit_enabled;
		stop_d _m_stop;
		exit_d _m_on_exit;
		started_d _m_on_started;
	};

} ///end of namespace ox
