
#include <Windows.h>
#include <set>
#include "../cxx/delegate.h"
#include "atomic_number.h"
#include "task_decl.h"



#pragma once


namespace ox
{
	struct win_high_time
	{
		LARGE_INTEGER _m_freq;
		void init()
		{
			QueryPerformanceFrequency(&_m_freq);
		}
		static LONGLONG freq()
		{
			LARGE_INTEGER _freq;
			QueryPerformanceFrequency(&_freq);
			return _freq.QuadPart;
		}
		static LARGE_INTEGER current()
		{
			LARGE_INTEGER curr;
			QueryPerformanceCounter(&curr);
			return curr;
		}
		static void current(LARGE_INTEGER& curr)
		{
			QueryPerformanceCounter(&curr);
		}
		static LONGLONG currrent_as_100nano_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart*10000/freq_ticks*1000;
		}
		static LONGLONG currrent_as_micro_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart*1000/freq_ticks*10001;
		}
		static LONGLONG currrent_as_milli_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart*1000/freq_ticks;
		}
		static LONGLONG currrent_as_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart/freq_ticks;
		}
		static LONGLONG currrent_as_double_100nano_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart*100000000.0/freq_ticks;
		}
		static LONGLONG currrent_as_double_micro_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart*1000000.0/freq_ticks;
		}
		static LONGLONG currrent_as_double_milli_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart*1000.0/freq_ticks;
		}
		static LONGLONG currrent_as_double_second(LONGLONG const& freq_ticks)
		{
			LARGE_INTEGER curr;
			current(curr);
			return curr.QuadPart*1.0/freq_ticks;
		}
	};
	struct time_elapsed
	{
		LARGE_INTEGER _m_from;
		LARGE_INTEGER _m_to;
		//LARGE_INTEGER _m_freq;
		void begin()
		{
			win_high_time::current(_m_from);
		}
		void end()
		{
			win_high_time::current(_m_to);
		}
		void clear()
		{
			_m_from.QuadPart = 0;
			_m_to.QuadPart = 0;
		}
		LONGLONG elapsed_ticks() const
		{
			return (_m_to.QuadPart-_m_from.QuadPart);
		}
		LONGLONG elapsed_micro_seconds() const
		{
			return elapsed_ticks()*1000000/win_high_time::freq();
		}
		LONGLONG elapsed_milli_seconds() const
		{
			return elapsed_ticks()*1000/win_high_time::freq();
		}
		LONGLONG elapsed_seconds() const
		{
			return elapsed_ticks()/win_high_time::freq();
		}
		double double_elapsed_micro_seconds() const
		{
			return elapsed_ticks()*1000000.0/win_high_time::freq();
		}
		double double_elapsed_milli_seconds() const
		{
			return elapsed_ticks()*1000.0/win_high_time::freq();
		}
		double double_elapsed_seconds() const
		{
			return elapsed_ticks()*1.0/win_high_time::freq();
		}
	};
	struct win_waitable_timer_engine
	{
		typedef HANDLE handle_t;
		handle_t _m_timer_handle;
		bool init()
		{
			_m_timer_handle = CreateWaitableTimerA(0,FALSE,0);
			return is_handle_valid();
		}
		static bool is_handle_valid(handle_t const& handle) {return handle!=0;}
		bool is_handle_valid() const {return is_handle_valid(_m_timer_handle);}
		handle_t handle() const {return _m_timer_handle;}
		void set_after_100nano(LONGLONG rel_100nano_seconds)
		{
			LARGE_INTEGER duration_rel_100nano_seconds;
			duration_rel_100nano_seconds.QuadPart = rel_100nano_seconds;
			BOOL okay = SetWaitableTimer(_m_timer_handle,&duration_rel_100nano_seconds,0,0,0,FALSE);
		}
		void uninit()
		{
			HANDLE handle = _m_timer_handle;
			_m_timer_handle = INVALID_HANDLE_VALUE;
			if (handle!=INVALID_HANDLE_VALUE)
				::CloseHandle(handle);
		}
	};

	struct win_timer_list
	{
		//typedef communication_const constant;
		//typedef communication_const::error_selfdefine_enum error_enum;

		//typedef delegate<bool(size_t id,LONGLONG period_micro_second,void const* bind)> event_d;
		typedef task_tt<bool> task_timer_t;

		struct timer_data
		{
			size_t id;
			LONGLONG period_100nano_seconds;
			//event_d event;
			task_timer_t* task;
			//void const* binded;
			//timer_control control;
			LONGLONG time_coming_100nano_seconds;
		};
		struct is_less
		{
			bool operator()(timer_data* const& a,timer_data* const& b)
			{
				return a->time_coming_100nano_seconds < b->time_coming_100nano_seconds;
			}
		};
		typedef std::multiset<timer_data*,is_less> timer_list;
		typedef timer_list::iterator timer_position;

		static size_t make_timerid()
		{
			static atomic_long static_id = 0;
			return ++static_id;
		}

		win_timer_list()
		{
			_m_time_coming_100nano_second = -1;
			_m_freq = win_high_time::freq();
		}

		~win_timer_list()
		{
			uninit();
		}

		//HANDLE handle() const {return _m_timer_engine.handle()/*_m_timer_handle*/;}

		/// return error-id
		size_t init()
		{
			//_m_timer_handle = CreateWaitableTimerA(0,FALSE,0);
			//if (!_m_timer_engine::is_handle_valid()/*0 == _m_timer_handle*/)
			//	return GetLastError();
			_m_freq = win_high_time::freq();
			return 0;
		}

		void uninit()
		{
			//HANDLE handle = _m_timer_handle;
			//_m_timer_handle = INVALID_HANDLE_VALUE;
			//if (handle!=INVALID_HANDLE_VALUE)
			//	::CloseHandle(handle);
			clear();
		}

		enum error_id_enum
		{
			__error_null = 0,
			__error_no_timer,
			__error_no_enough_memory,
		};

		//size_t start()
		//{
		//	if (_m_timer_list.empty())
		//	{
		//		_m_time_coming_100nano_second = -1;
		//		return __error_no_timer;
		//	}
		//	timer_list::iterator i = _m_timer_list.begin();
		//	timer_data const* ptimer = *i;
		//	timer_data const& timer = *ptimer;
		//	LARGE_INTEGER duration_rel_100nano_seconds;
		//	LONGLONG current_100nano_second = win_high_time::currrent_as_100nano_second(_m_freq);
		//	duration_rel_100nano_seconds.QuadPart = current_100nano_second-timer.time_coming_100nano_seconds;
		//	if (duration_rel_100nano_seconds.QuadPart>=0)
		//		duration_rel_100nano_seconds.QuadPart = 0;
		//	_m_time_coming_100nano_second = timer.time_coming_100nano_seconds;
		//	BOOL okay = SetWaitableTimer(_m_timer_handle,&duration_rel_100nano_seconds,0,0,0,FALSE);
		//	return okay==TRUE?0:GetLastError();
		//}

		size_t calculate_next_timer(LONGLONG* relative_100nano)
		{
			if (_m_timer_list.empty())
			{
				_m_time_coming_100nano_second = -1;
				return __error_no_timer;
			}
			timer_list::iterator i = _m_timer_list.begin();
			timer_data const* ptimer = *i;
			timer_data const& timer = *ptimer;
			//LARGE_INTEGER duration_rel_100nano_seconds;
			LONGLONG current_100nano_second = win_high_time::currrent_as_100nano_second(_m_freq);
			LONGLONG relative_100nano_second = current_100nano_second-timer.time_coming_100nano_seconds;
			//duration_rel_100nano_seconds.QuadPart = current_100nano_second-timer.time_coming_100nano_seconds;
			//if (duration_rel_100nano_seconds.QuadPart>=0)
			//	duration_rel_100nano_seconds.QuadPart = 0;
			if (relative_100nano)
			{
				if (relative_100nano_second>=0) relative_100nano_second=0;
				*relative_100nano = relative_100nano_second;
			}
			_m_time_coming_100nano_second = timer.time_coming_100nano_seconds;
			return __error_null;
		}

		bool is_empty() const
		{
			return _m_timer_list.empty();
		}
		bool is_first_coming() const
		{
			if (_m_timer_list.empty()) return false;
			LONGLONG curr_begin = win_high_time::currrent_as_100nano_second(_m_freq);
			timer_data const& timer = **_m_timer_list.begin();
			return curr_begin>=timer.time_coming_100nano_seconds;//<=_m_time_coming_100nano_second;
		}
		void run_first_and_remove()
		{
			LONGLONG curr_begin = win_high_time::currrent_as_100nano_second(_m_freq);
			LONGLONG curr_curr = 0;
			LONGLONG curr_real = 0;
			/// may be somebody erase the timer, just for safety
			if (_m_timer_list.empty())
			{
				_m_time_coming_100nano_second = -1;
				return;
			}
			//printf("---------\n");
			timer_list::iterator i = _m_timer_list.begin();
			while (i!=_m_timer_list.end())
			{
				timer_data* ptimer = *i;
				timer_data& timer = *ptimer;
				if (timer.time_coming_100nano_seconds>_m_time_coming_100nano_second)
					break;
				bool bcontinue = timer.task->run();
				_m_timer_list.erase(i);
				if (!bcontinue)
				{
					i = _m_timer_list.begin();
					timer.task->destroy();
					delete ptimer;
					continue;
				}
				/// {{ timer combine
				curr_curr = win_high_time::currrent_as_100nano_second(_m_freq);
				curr_real = _m_time_coming_100nano_second + (curr_curr-curr_begin);
				LONGLONG distance = curr_real - ptimer->time_coming_100nano_seconds;
				LONGLONG tail = distance % ptimer->period_100nano_seconds;
				distance -= tail;
				if (distance<=0) distance = ptimer->period_100nano_seconds;
				ptimer->time_coming_100nano_seconds += distance;
				/// }} timer combine
				_m_timer_list.insert(ptimer);
				i = _m_timer_list.begin();
			}
			//printf("===========\n");
		}
		void on_timer(HANDLE timer_handle)
		{
			run_first_and_remove();
		}

		//size_t add_timer_with_control(thread_task_t* task,LONGLONG period_micro_seconds,size_t timerid,timer_position* pos=0)
		//{
		//	timer_data* ptd = new (std::nothrow) timer_data;
		//	if (!ptd) return __error_no_enough_memory;

		//	timer_data& td = *ptd;
		//	td.period_100nano_seconds = period_micro_seconds*10;
		//	td.id = timerid;
		//	td.event.clear();
		//	td.task = task;
		//	td.binded = 0;
		//	td.time_coming_100nano_seconds = win_high_time::currrent_as_100nano_second(_m_freq) + td.period_100nano_seconds;

		//	timer_position i = _m_timer_list.insert(ptd);
		//	if (pos) *pos = i;
		//	return __error_null;
		//}

		size_t add_timer(task_timer_t* task,LONGLONG period_micro_seconds,size_t timerid,timer_position* pos=0)
		{
			timer_data* ptd = new (std::nothrow) timer_data;
			if (!ptd) return __error_no_enough_memory;

			timer_data& td = *ptd;
			td.period_100nano_seconds = period_micro_seconds*10;
			td.id = timerid;
			td.task = task;
			td.time_coming_100nano_seconds = win_high_time::currrent_as_100nano_second(_m_freq) + td.period_100nano_seconds;

			timer_position i = _m_timer_list.insert(ptd);
			if (pos) *pos = i;
			return __error_null;
		}

		//size_t add_timer(size_t timerid,LONGLONG period_micro_seconds,event_d const& timer_event,void const* binded,timer_position* pos=0)
		//{
		//	timer_data* ptd = new (std::nothrow) timer_data;
		//	if (!ptd) return __error_no_enough_memory;

		//	timer_data& td = *ptd;
		//	td.period_100nano_seconds = period_micro_seconds*10;
		//	td.id = timerid;
		//	td.event = timer_event;
		//	td.binded = binded;
		//	td.time_coming_100nano_seconds = win_high_time::currrent_as_100nano_second(_m_freq) + td.period_100nano_seconds;

		//	timer_position i = _m_timer_list.insert(ptd);
		//	if (pos) *pos = i;
		//	return __error_null;
		//}

		void erase_timer(size_t timerid)
		{
			timer_list::iterator i = _m_timer_list.begin();
			while (i!=_m_timer_list.end())
			{
				timer_data* timer = *i;
				if (timer->id!=timerid) continue;
				_m_timer_list.erase(i);
				timer->task->destroy();
				delete timer;
				break;
			}
		}

		void unsafe_erase_timer(timer_position pos)
		{
			_m_timer_list.erase(pos);
		}

		void clear()
		{
			timer_list::iterator i = _m_timer_list.begin();
			while (i!=_m_timer_list.end())
			{
				timer_data* timer = *i;
				i = _m_timer_list.erase(i);
				delete timer;
			}
		}

		//HANDLE _m_timer_handle;
		//timer_engine_t _m_timer_engine;
		timer_list _m_timer_list;
		LONGLONG _m_freq;
		LONGLONG _m_time_coming_100nano_second;
	}; /// end of win high timer
	
}