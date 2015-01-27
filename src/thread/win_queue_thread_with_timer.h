



#include "win_queue_thread.h"
#include "timer.h"

#pragma once


namespace ox
{

	struct win_queue_thread_with_timer : win_queue_thread
	{
		typedef win_queue_thread_with_timer self;
		typedef win_high_timer timer_t;
		typedef win_high_timer::event_d timer_event_d;
		typedef win_queue_thread::task_t task_t;
		typedef win_queue_thread::event_arrived_d event_arrived_d;

		void start()
		{
			if (is_started()) return;
			win_queue_thread::start();
			_m_timer.init();
			register_event_service(_m_timer._m_timer_handle,event_arrived_d(this,&self::on_timer_handle_arrived));
		}

		void stop_next()
		{
			win_queue_thread::add(task_single<int>::make(this,&self::do_stop));
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

		size_t add_timer_micro(LONGLONG period_micro_seconds,timer_event_d const& timer_event,void const* binded,timer_t::timer_position* pos=0)
		{
			if (is_exiting()) return 0;
			size_t timerid = win_high_timer::make_timerid();
			if (GetCurrentThreadId()==_m_threadid)
				do_add_timer(timerid,period_micro_seconds,timer_event,binded,pos);
			else
				win_queue_thread::add(task_single<int>::make(
					this,&self::do_add_timer,timerid,period_micro_seconds,timer_event,binded,pos));
			return timerid;
		}
		size_t add_timer_milli(LONGLONG period_milli_seconds,timer_event_d const& timer_event,void const* binded,timer_t::timer_position* pos=0)
		{
			return add_timer_micro(period_milli_seconds*1000,timer_event,binded,pos);
		}
		size_t add_timer_second(LONGLONG period_seconds,timer_event_d const& timer_event,void const* binded,timer_t::timer_position* pos=0)
		{
			return add_timer_micro(period_seconds*1000*1000,timer_event,binded,pos);
		}
		void erase_timer(size_t timerid)
		{
			if (is_exiting()) return;
			if (threadid()==::GetCurrentThreadId())
			{
				do_erase_timer(timerid);
				return;
			}
			win_queue_thread::add(task_single<int>::make(
				this,&self::do_erase_timer,timerid));
		}
		/// you should ensure the pos has not been erased in timer list
		void unsafe_erase_timer(timer_t::timer_position pos)
		{
			if (is_exiting()) return;
			if (threadid()==::GetCurrentThreadId())
			{
				do_unsafe_erase_timer(pos);
				return;
			}
			win_queue_thread::add(task_single<int>::make(
				this,&self::do_unsafe_erase_timer,pos));
		}

	private:
		void do_add_timer(size_t timerid,LONGLONG period_micro_seconds,timer_event_d event,void const* binded,timer_t::timer_position* pos)
		{
			_m_timer.add_timer(timerid,period_micro_seconds,event,binded,pos);
			_m_timer.start();
		}
		void on_timer_handle_arrived(HANDLE timer_handle)
		{
			if (is_exiting()) return;
			_m_timer.on_timer(_m_timer.handle());
			_m_timer.start();
		}
		void do_erase_timer(size_t timerid)
		{
			_m_timer.erase_timer(timerid);
			_m_timer.start();
		}
		void do_unsafe_erase_timer(timer_t::timer_position pos)
		{
			_m_timer.unsafe_erase_timer(pos);
			_m_timer.start();
		}
		void do_stop()
		{
			_m_timer.uninit();
			win_queue_thread::stop_next();
		}

	private:
		timer_t _m_timer;
	};

} /// end of namespace ox
