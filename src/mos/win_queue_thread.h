
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
#include "../ox/nsab.h"
#include "../cxx/delegate.h"
#include "../cxx/multi_delegate.h"
#include "delegate_closure.h"
#include "atomic_number.h"
#include "scope_raii.h"
#include "safe_double_queue.h"
#include "task_feedback.h"
#include "win_thread.h"


#pragma once




/*
1. on_shedule_gap()
2. transfer queue, that is, promote the efficiency of swaping queue, when there is
thousands of thread being prepare to add message to the wait or next queue
3. is-idle(), show that the thread must be at idle, but not-idle is not accurate //absolutely
4. condition queue
5. threads: thread pool, all threads will trigger pool when it starts or stops
6. register server or function
7. clear(), will clear all the queue
8. notify self, post self
9. the listener can sniff the certain event, that is, we have two options, one is for
architecture to design the flow, the other is for the programmer to do at their will
10. return suspend, exit, go on next, instead of true/false, on the run function
11. thread suspend, kill from system, or from self
12. the atomic unit of schedule is the whole procedure of run()
*/


___namespace2_begin(ox,mos)


struct win_ui_thread;
struct win_queue_thread : win_thread<unsigned()>
{
	friend win_ui_thread;
	typedef win_queue_thread self;
	enum {__highid,__normalid,__controlid,__exit1};
	enum control_enum {__clear};
	typedef delegate<void(self*)> idle_d;
	typedef delegate<void(self*)> busy_d;
	typedef delegate<void(self*)> exit_d;
	typedef delegate<void(self*)> final_d;
	typedef thread_task_t task_t;
	typedef delegate<void(HANDLE)> event_arrived_d;
	typedef win_thread<unsigned()> base;

private:
	typedef safe_double_queue_tt<task_t*> task_queue;
	typedef task_queue control_queue;
	typedef task_queue::object_ptr task_pptr;
	enum enum_inner_control {__exit2,__suspend};
	struct service_item_t
	{
		service_item_t() : handle(0) {}
		service_item_t(HANDLE h,event_arrived_d _event) : handle(h),event(_event) {}
		HANDLE handle;
		event_arrived_d event;
	};

public:
	~win_queue_thread()
	{
		assert (!base::is_running());
		if (!base::is_running()) return;
		if (!_m_is_destroyed_at_destruction) DebugBreak();
		stop();
	}
	self()
	{
		init(0,-1);
	}
	//win_queue_thread(char const* your_name)
	//{
	//	init(your_name,-1);
	//}
	self(size_t your_id,char const* your_name)
	{
		init(your_name,your_id);
	}
	self(self const&);
	self& operator=(self const&);

	int add(task_t* task)
	{
		if (!_m_normal_queue.is_add_enabled() || is_exiting()) return -1;
		assert (!is_exiting());
		_m_normal_queue.add(task);
		return 0;
	}
	int add2(task_t* task,size_t/*threadid, compatible with multi-thread*/)
	{
		return add(task);
	}
	int add_high(task_t* task)
	{
		if (!_m_high_queue.is_add_enabled() || is_exiting()) return -1;
		assert (!is_exiting());
		_m_high_queue.add(task);
		return 0;
	}
	int add2_high(task_t* task,size_t/*threadid, compatible with multi-thread*/)
	{
		return add_high(task);
	}

	bool clear()
	{
		typedef task_single<void> stask;
		add(stask::make(this,&self::do_clear_runing));
		_m_normal_queue.clear_outer();
		_m_high_queue.clear_outer();
		_m_control_queue.clear_outer();
		return true;
	}

	bool is_started() const
	{
		return !_m_exit_enabled.value() && base::is_started();
	}

	bool is_exiting() const
	{
		return 0!=_m_exit_enabled.value();
	}

	bool is_busy() const
	{
		return _m_is_busy.value()!=0;
	}
	bool is_idle() const
	{
		return _m_is_busy.value()==0;
	}

	void enable_stopped_automatically(bool isauto=1)
	{
		_m_is_destroyed_at_destruction = isauto;
	}

	bool start()
	{
		return start_from(false);
	}
	bool start_here()
	{
		return start_from(true);
	}

	bool start_from(bool here)
	{
		if (is_started())
			return true;
		_m_normal_queue.init();
		_m_high_queue.init();
		_m_control_queue.init();

		_m_htaskadded[__normalid] = _m_normal_queue.event_added_handle();
		_m_htaskadded[__highid] = _m_high_queue.event_added_handle();
		_m_htaskadded[__controlid] = _m_control_queue.event_added_handle();
		_m_htaskadded[__exit1] = CreateEvent(0,TRUE,FALSE,0);
		_m_hcontrol[__exit2] = _m_htaskadded[__exit1];
		_m_hcontrol[__suspend] = CreateEvent(0,TRUE,TRUE,0);

		assert(!base::on_run().is_empty());
		base::init_handleid();
		_m_exit_enabled = 0;
		_m_service_size = 0;
		if (here) return base::start_here();
		return base::start();
	}

	bool stop_next()
	{
		if (get_is_exiting_or_set()) return false;
		BOOL b = SetEvent(_m_hcontrol[__exit2]);
		assert(b);
		return true;
		//if (!b)
		//{
		//	DWORD err = GetLastError();
		//	assert(false && "SetEvent Error");
		//}
	}
	bool stop_till()
	{
		return 0==add(task_single<void>::make(this,&self::do_stop_util));
	}

	wait_enum stop()
	{
		//HANDLE handle = INVALID_HANDLE_VALUE;
		bool b = is_stop_allowed(/*handle*/);
		if (!b) return __notallowed;
		//assert (handle == _m_sudo_thread_handle);
		stop_next();
		return join_thread_id(_m_threadid);
	}

	void suspend_next()
	{
		ResetEvent(_m_hcontrol[__suspend]);
	}
	void resume()
	{
		SetEvent(_m_hcontrol[__suspend]);
	}

	void add_enabled(bool b)
	{
		_m_normal_queue.add_enabled(b);
	}

	bool is_any_runing_task() const
	{
		return _m_normal_queue.is_empty() && _m_high_queue.is_empty();
	}

	bool is_any_task() const
	{
		return is_any_runing_task() && _m_control_queue.is_empty();
	}

	size_t wait_added_event() const
	{
		DWORD id = WaitForMultipleObjects(4+_m_service_size,_m_htaskadded,FALSE,INFINITE);
		if (id!=WAIT_FAILED)
			return id-WAIT_OBJECT_0;
		return WAIT_FAILED;
	}
	bool wait_control_event() const
	{
		DWORD id = WaitForMultipleObjects(2,_m_hcontrol,FALSE,INFINITE);
		bool ret = id!=WAIT_FAILED;
		assert(ret);
		return ret;
	}

	void register_event_service(HANDLE hevent,event_arrived_d event_arrived)
	{
		add(task_single<void>::make(this,&self::do_register_event_service,hevent,event_arrived));
	}

	exit_d& on_exit() {return _m_on_exit;}
	exit_d const& on_exit() const {return _m_on_exit;}
	final_d& on_final() {return _m_on_final;}
	final_d const& on_final() const {return _m_on_final;}
	idle_d& on_idle() {return _m_on_idle;}
	idle_d const& on_idle() const {return _m_on_idle;}
	busy_d& on_busy() {return _m_on_busy;}
	busy_d const& on_busy() const {return _m_on_busy;}

#if 0
	void unsafe_terminate()
	{
		assert (GetCurrentThreadId()!=_m_threadid);
		stop_next();
		if (is_idle()) return;
		base::unsafe_terminate();
	}
#endif		
	
private:
	void on_internal_exit(base* thread)
	{
		assert (thread==this);
		clear_resource();
		if (_m_on_exit.is_empty()) return;
		_m_on_exit(this);
	}
	void on_internal_final_exit(base* thread)
	{
		assert (thread==this);
		if (_m_on_final.is_empty()) return;
		_m_on_final(this);
	}
	unsigned run()
	{
		/// exit flag by action
		bool is_busying = false;

		label_control_processing:

		if (_m_exit_enabled)
			return 0;
		/// whether we need to suspend
		WaitForSingleObject(_m_hcontrol[__suspend],INFINITE);
		if (_m_exit_enabled)
			return 0;

		/// before we do, check control and high queue first
		/// control task to do
		{
			task_pptr runing = _m_control_queue.get();
			if (runing.is_empty())
			{
				_m_control_queue.swap();
				runing = _m_control_queue.get();
			}
			if (!runing.is_empty())
			{
				if (!is_busying && !on_busy().is_empty()) _m_on_busy(this);
				is_busying = true;
				_m_is_busy = 1;
				_m_control_queue.runing->pop();
				runing.value()->run();
				runing.value()->destroy();
				_m_is_busy = 0;
				goto label_control_processing;
			}
		}

		/// high task to do
		{
			/// whether we need to suspend
			task_pptr runing = _m_high_queue.get();
			if (runing.is_empty())
			{
				_m_high_queue.swap();
				runing = _m_high_queue.get();
			}
			if (!runing.is_empty())
			{
				if (!is_busying && !on_busy().is_empty()) _m_on_busy(this);
				is_busying = true;
				_m_is_busy = 1;
				_m_high_queue.runing->pop();
				runing.value()->run();
				runing.value()->destroy();
				_m_is_busy = 0;
				goto label_control_processing;
			}
		}

		/// here, no high task
		/// normal task to do
		{
			task_pptr runing = _m_normal_queue.get();
			if (runing.is_empty())
			{
				_m_normal_queue.swap();
				runing = _m_normal_queue.get();
			}
			if (!runing.is_empty())
			{
				if (!is_busying && !on_busy().is_empty()) _m_on_busy(this);
				is_busying = true;
				_m_normal_queue.runing->pop();
				_m_is_busy = 1;
				runing.value()->run();
				runing.value()->destroy();
				_m_is_busy = 0;
				goto label_control_processing;
			}
		}

		/// no control, no high task, no normal task, what we do next? wait event, 
		/// until we find task arrived

		/// whether we need to suspend
		if (!wait_control_event())
		{
			assert(false && "control event error happens");
			return -1;
		}

		/// is idle
		if (!is_any_task())
		{
			is_busying = false;
			if (!_m_on_idle.is_empty())
				_m_on_idle(this);
		}

		size_t eid = wait_added_event();
		if (eid == WAIT_FAILED)
		{
			assert(false && "wait event error happens");
			return -1;
		}
		else if (eid >= 4)
		{
			_m_service_array[eid-4].event(_m_service_array[eid-4].handle);
		}

		goto label_control_processing;
	}

	void clear_resource()
	{
		_m_normal_queue.uninit();
		_m_high_queue.uninit();
		_m_control_queue.uninit();
		CloseHandle(_m_hcontrol[__exit2]);
		CloseHandle(_m_hcontrol[__suspend]);
		_m_hcontrol[__exit2] = 0;
		_m_hcontrol[__suspend] = 0;
		_m_exit_enabled=1;
	}

	void do_clear_runing()
	{
		if (GetCurrentThreadId()!=_m_threadid)
		{
			assert(false && "current thread is not the thread class!");
			return;
		}
		_m_normal_queue.clear_runing();
		_m_high_queue.clear_runing();
		_m_control_queue.clear_runing();
	}
	void do_register_event_service(HANDLE hevent,event_arrived_d event_arrived)
	{
		assert(_m_service_size<2);
		if (_m_service_size>=2) return;
		_m_service_array[_m_service_size].handle = hevent;
		_m_service_array[_m_service_size].event = event_arrived;
		_m_htaskadded[4+_m_service_size] = hevent;
		_m_service_size++;
	}
	void do_stop_util()
	{
		stop_next();
	}
	void init(char const* your_name,size_t your_id)
	{
		_m_is_destroyed_at_destruction=0;
		_m_hcontrol[0]=0;
		_m_hcontrol[1]=0;
		_m_exit_enabled=0;
		_m_is_busy=0;
		//_m_sudo_thread_handle=HANDLE(-1);
		//_m_threadid=-1;
		_m_service_size = 0;
		base::on_run().assign(this,&self::run);
		base::on_exit().assign(this,&self::on_internal_exit);
		base::on_final().assign(this,&self::on_internal_final_exit);
		base::init(your_name,your_id);
	}

protected:
	/// when the function call, you'd call stop next and join
	/// just like the stop function stop
	bool is_stop_allowed(/*HANDLE& h*/)
	{
		bool is_allowed = (GetCurrentThreadId()!=_m_threadid);
		assert (is_allowed);
		return is_allowed;
		//if (!is_allowed) return __notallowed;
		//if (_m_exit_enabled==1) return __on_exiting;
		//_m_exit_enabled = 1;
		//HANDLE handle = _m_sudo_thread_handle; /// or return
		//h = handle;
		//if (handle==INVALID_HANDLE_VALUE) return __not_started;
		//return __allowed;
	}
	bool get_is_exiting_or_set()
	{
		return _m_exit_enabled.assign_and_return_old(1);
	}

private:
	task_queue _m_normal_queue;
	task_queue _m_high_queue;
	control_queue _m_control_queue;
	HANDLE _m_htaskadded[4+2];
	HANDLE _m_hcontrol[2+2];
	atomic_long _m_exit_enabled;
	exit_d _m_on_exit;
	final_d _m_on_final;
	idle_d _m_on_idle;
	busy_d _m_on_busy;
	service_item_t _m_service_array[2];
	size_t _m_service_size;
	atomic_long _m_is_busy;
	atomic_long _m_is_destroyed_at_destruction;
}; /// end of win queue thread

	
___namespace2_end()
