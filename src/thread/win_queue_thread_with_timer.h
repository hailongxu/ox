



#include "win_queue_thread.h"
#include "timer.h"

#pragma once


namespace ox
{

	struct win_queue_thread_with_timer : win_queue_thread
	{
		typedef win_queue_thread_with_timer self;
		typedef win_timer_list timer_list_t;
		typedef win_timer_list::task_timer_t task_timer_t;
		typedef win_timer_list::timer_position timer_position;
		typedef win_waitable_timer_engine timer_engine;
		//typedef timer_t::event_d timer_event_d;
		typedef win_queue_thread::task_t task_t;
		typedef win_queue_thread::event_arrived_d event_arrived_d;

		void start()
		{
			if (is_started()) return;
			win_queue_thread::start();
			_m_timer_list.init();
			_m_timer_engine.init();
			register_event_service(_m_timer_engine.handle(),event_arrived_d(this,&self::on_timer_handle_arrived));
		}

		void stop_next()
		{
			win_queue_thread::add(thread_task_helper::make(this,&self::do_stop));
		}

		wait_enum stop()
		{
			HANDLE handle = INVALID_HANDLE_VALUE;
			bool b = is_stop_allowed(/*handle*/);
			if (!b) return __notallowed;
			//assert (handle == _m_sudo_thread_handle);
			stop_next();
			return join_thread_id(_m_threadid);
		}

#if 0
		void unsafe_terminate()
		{
			win_queue_thread::unsafe_terminate();
			do_stop();
		}
#endif

		size_t add_timer_micro(task_timer_t* timer_task,LONGLONG period_micro_seconds,size_t timerid=-1,timer_position* pos=0)
		{
			if (is_exiting()) return 0;
			//size_t timerid = timer_t::make_timerid();
			if (GetCurrentThreadId()==_m_threadid)
				do_add_timer(timer_task,period_micro_seconds,timerid,pos);
			else
				win_queue_thread::add(task_single<void>::make(
					this,&self::do_add_timer,timer_task,period_micro_seconds,timerid,pos));
			return timerid;
		}
		size_t add_timer_milli(task_timer_t* timer_task,LONGLONG period_milli_seconds,size_t timerid=-1,timer_position* pos=0)
		{
			return add_timer_micro(timer_task,period_milli_seconds*1000,timerid,pos);
		}
		size_t add_timer_second(task_timer_t* timer_task,LONGLONG period_seconds,size_t timerid=-1,timer_position* pos=0)
		{
			return add_timer_micro(timer_task,period_seconds*1000*1000,timerid,pos);
		}
		void erase_timer(size_t timerid)
		{
			if (is_exiting()) return;
			if (threadid()==::GetCurrentThreadId())
			{
				do_erase_timer(timerid);
				return;
			}
			win_queue_thread::add(thread_task_helper::make(
				this,&self::do_erase_timer,timerid));
		}
		/// you should ensure the pos has not been erased in timer list
		void unsafe_erase_timer(timer_position pos)
		{
			if (is_exiting()) return;
			if (threadid()==::GetCurrentThreadId())
			{
				do_unsafe_erase_timer(pos);
				return;
			}
			win_queue_thread::add(thread_task_helper::make(
				this,&self::do_unsafe_erase_timer,pos));
		}

	private:
		void do_add_timer(task_timer_t* task_timer,LONGLONG period_micro_seconds,size_t timerid,timer_position* pos)
		{
			_m_timer_list.add_timer(task_timer,period_micro_seconds,timerid,pos);
			LONGLONG relative_100nano = 0;
			_m_timer_list.calculate_next_timer(&relative_100nano);
			_m_timer_engine.set_after_100nano(relative_100nano);
		}
		void on_timer_handle_arrived(HANDLE timer_handle)
		{
			if (is_exiting()) return;
			_m_timer_list.on_timer(_m_timer_engine.handle());
			LONGLONG relative_100nano = 0;
			_m_timer_list.calculate_next_timer(&relative_100nano);
			_m_timer_engine.set_after_100nano(relative_100nano);
		}
		void do_erase_timer(size_t timerid)
		{
			_m_timer_list.erase_timer(timerid);
			LONGLONG relative_100nano = 0;
			_m_timer_list.calculate_next_timer(&relative_100nano);
			_m_timer_engine.set_after_100nano(relative_100nano);
		}
		void do_unsafe_erase_timer(timer_position pos)
		{
			_m_timer_list.unsafe_erase_timer(pos);
			LONGLONG relative_100nano = 0;
			_m_timer_list.calculate_next_timer(&relative_100nano);
			_m_timer_engine.set_after_100nano(relative_100nano);
		}
		void do_stop()
		{
			_m_timer_engine.uninit();
			_m_timer_list.uninit();
			win_queue_thread::stop_next();
		}

	private:
		timer_engine _m_timer_engine;
		timer_list_t _m_timer_list;
	};

} /// end of namespace ox
