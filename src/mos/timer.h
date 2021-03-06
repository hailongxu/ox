
#include <Windows.h>
#include <set>
#include <vector>
#include "../ox/nsab.h"
#include "../cxx/delegate.h"
#include "atomic_number.h"
#include "task_decl.h"
#include "freeptr.h"



#pragma once


___namespace2_begin(ox,mos)


struct win_high_time_freq
{
	static LONGLONG freq()
	{
		LARGE_INTEGER _freq;
		QueryPerformanceFrequency(&_freq);
		return _freq.QuadPart;
	}
};
static LONGLONG const __win_high_time_freq = win_high_time_freq::freq();
struct win_high_time : win_high_time_freq
{
	static LONGLONG current_ticks()
	{
		LARGE_INTEGER curr_ticks;
		QueryPerformanceCounter(&curr_ticks);
		return curr_ticks.QuadPart;
	}
	static void current_ticks(LARGE_INTEGER& curr_ticks)
	{
		QueryPerformanceCounter(&curr_ticks);
	}
	static LONGLONG current_as_100nano_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return curr_ticks.QuadPart*1000*10/__win_high_time_freq*1000;
	}
	static LONGLONG current_as_micro_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return curr_ticks.QuadPart*1000/__win_high_time_freq*1000;
	}
	static LONGLONG current_as_milli_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return curr_ticks.QuadPart*1000/__win_high_time_freq;
	}
	static LONGLONG current_as_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return curr_ticks.QuadPart/__win_high_time_freq;
	}
	static LONGLONG current_as_double_100nano_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return LONGLONG(curr_ticks.QuadPart*100000000.0/__win_high_time_freq);
	}
	static LONGLONG current_as_double_micro_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return LONGLONG(curr_ticks.QuadPart*1000000.0/__win_high_time_freq);
	}
	static LONGLONG current_as_double_milli_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return LONGLONG(curr_ticks.QuadPart*1000.0/__win_high_time_freq);
	}
	static LONGLONG current_as_double_second()
	{
		LARGE_INTEGER curr_ticks;
		current_ticks(curr_ticks);
		return LONGLONG(curr_ticks.QuadPart*1.0/__win_high_time_freq);
	}
	static LONGLONG elapsed_seconds(LONGLONG const& from_ticks,LONGLONG const& to_ticks)
	{
		return elapsed_ticks(from_ticks,to_ticks)/__win_high_time_freq;
	}
	static LONGLONG elapsed_milli_seconds(LONGLONG const& from_ticks,LONGLONG const& to_ticks)
	{
		return elapsed_ticks(from_ticks,to_ticks)*1000/__win_high_time_freq;
	}
	static LONGLONG elapsed_macro_seconds(LONGLONG const& from_ticks,LONGLONG const& to_ticks)
	{
		return elapsed_ticks(from_ticks,to_ticks)*1000*1000/__win_high_time_freq;
	}
	static LONGLONG elapsed_ticks(LONGLONG const& from_ticks,LONGLONG const& to_ticks)
	{
		return to_ticks-from_ticks;
	}
	static LONGLONG elapsed_seconds(LONGLONG const& to_ticks)
	{
		return elapsed_seconds(current_ticks(),to_ticks);
	}
	static LONGLONG elapsed_milli_seconds(LONGLONG const& to_ticks)
	{
		return elapsed_milli_seconds(current_ticks(),to_ticks);
	}
	static LONGLONG elapsed_macro_seconds(LONGLONG const& to_ticks)
	{
		return elapsed_macro_seconds(current_ticks(),to_ticks);
	}
	static LONGLONG elapsed_ticks(LONGLONG const& to_ticks)
	{
		LARGE_INTEGER from; current_ticks(from);
		return elapsed_ticks(from.QuadPart,to_ticks);
	}
};
struct time_elapsed
{
	LARGE_INTEGER _m_from;
	LARGE_INTEGER _m_to;
	void begin()
	{
		win_high_time::current_ticks(_m_from);
	}
	void end()
	{
		win_high_time::current_ticks(_m_to);
	}
	void clear()
	{
		_m_from.QuadPart = 0;
		_m_to.QuadPart = 0;
	}
	LONGLONG elapsed_ticks() const
	{
		return win_high_time::elapsed_ticks(_m_from.QuadPart,_m_to.QuadPart);
	}
	LONGLONG elapsed_micro_seconds() const
	{
		return win_high_time::elapsed_macro_seconds(_m_from.QuadPart,_m_to.QuadPart);
	}
	LONGLONG elapsed_milli_seconds() const
	{
		return win_high_time::elapsed_milli_seconds(_m_from.QuadPart,_m_to.QuadPart);
	}
	LONGLONG elapsed_seconds() const
	{
		return win_high_time::elapsed_seconds(_m_from.QuadPart,_m_to.QuadPart);
	}
	double double_elapsed_micro_seconds() const
	{
		return elapsed_ticks()*1000000.0/__win_high_time_freq;
	}
	double double_elapsed_milli_seconds() const
	{
		return elapsed_ticks()*1000.0/__win_high_time_freq;
	}
	double double_elapsed_seconds() const
	{
		return elapsed_ticks()*1.0/__win_high_time_freq;
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

	typedef delegate<bool(size_t id,LONGLONG period_micro_second,void const* bind)> timer_event_d;
	typedef task_tt<bool> task_timer_t;

	struct timer_data
	{
		size_t id;
		LONGLONG period_100nano_seconds;
		LONGLONG time_coming_100nano_seconds;
		task_timer_t* task;
		timer_event_d event;
		void const* binded;
		//timer_control control;
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

	//static size_t make_timerid()
	//{
	//	static atomic_long static_id = 0;
	//	return ++static_id;
	//}

	win_timer_list()
	{
		_m_time_coming_100nano_second = -1;
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
		//_m_freq = win_high_time::freq();
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
		LONGLONG current_100nano_second = win_high_time::current_as_100nano_second();
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
		LONGLONG curr_begin = win_high_time::current_as_100nano_second();
		timer_data const& timer = **_m_timer_list.begin();
		return curr_begin>=timer.time_coming_100nano_seconds;//<=_m_time_coming_100nano_second;
	}
	void run_first_and_remove()
	{
		LONGLONG curr_begin = win_high_time::current_as_100nano_second();
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
			bool bcontinue = timer.task?timer.task->run():timer.event(timer.id,timer.period_100nano_seconds,timer.binded);
			_m_timer_list.erase(i);
			if (!bcontinue)
			{
				i = _m_timer_list.begin();
				if (timer.task) timer.task->destroy();
				delete ptimer;
				continue;
			}
			/// {{ timer combine
			curr_curr = win_high_time::current_as_100nano_second();
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
		td.binded = 0;
		td.time_coming_100nano_seconds = win_high_time::current_as_100nano_second() + td.period_100nano_seconds;
		if (td.time_coming_100nano_seconds<0) td.time_coming_100nano_seconds=0;

		timer_position i = _m_timer_list.insert(ptd);
		if (pos) *pos = i;
		return __error_null;
	}

	size_t add_timer(timer_event_d const& timer_event,LONGLONG period_micro_seconds,void const* binded,size_t timerid,timer_position* pos=0)
	{
		timer_data* ptd = new (std::nothrow) timer_data;
		if (!ptd) return __error_no_enough_memory;

		timer_data& td = *ptd;
		td.period_100nano_seconds = period_micro_seconds*10;
		td.id = timerid;
		td.task = 0;
		td.binded = 0;
		td.event = timer_event;
		td.time_coming_100nano_seconds = win_high_time::current_as_100nano_second() + td.period_100nano_seconds;
		if (td.time_coming_100nano_seconds<0) td.time_coming_100nano_seconds=0;

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

	timer_list _m_timer_list;
	LONGLONG _m_time_coming_100nano_second;
}; /// end of win high timer


namespace timer
{
	struct task_position
	{
		size_t timerid;
		size_t taskindex;
	};

	typedef delegate<void(size_t timerid,LONGLONG const& timeout_ms_end)> timer_new_d;

	template <typename task_tn>
	struct timer_resolution_task_system
	{
		/// type def itself
		typedef timer_resolution_task_system self;
		typedef task_tn task_t;
		typedef free_ptr<task_t> task_ptr;

		/// types defs'
		typedef delegate<bool(task_t&,task_position const&,void*)> action_d;
		typedef delegate<void(task_t const&,void const*)> task_tobe_destructed_d;
		typedef std::vector<task_t> task_list_t;
		struct timerid_t
		{
			timerid_t() : _m_id(0),_m_max_back(-1) {}
			size_t _m_id;
			size_t value() const {return _m_id;}
			size_t next() {if(++_m_id>_m_max_back) _m_id=0;return _m_id;}
			size_t distance(size_t a,size_t b) const
			{
				return b>=a ? b-a : (_m_max_back-a)+1+b;
			}
			size_t _m_max_back;/// [0,_m_max_back]
		};
		struct timerid_item
		{
			task_list_t tasklist;
			size_t index_tobe_send;
			size_t count_of_recved;
			size_t timerid;
			LONGLONG timeout_ms_end;
			bool is_all_recved() const
			{
				return count_of_recved>=tasklist.size();
			}
			bool is_all_sent() const
			{
				return index_tobe_send>=tasklist.size();
			}
			bool is_timeouted() const
			{
				return timeout_ms_end<=-1;
			}
		};
		typedef std::vector<timerid_item*> timerid_list_t;
		static size_t const __timerid_resolution_half_ms = 50;
		/// cons', dest'
		self()
		{
			_m_timerid_tobe_sent = 0;
			//add_null();
			_m_timerid_resolution_half = __timerid_resolution_half_ms;
		}
		void set_timerid_resolution(size_t resolution_ms)
		{
			_m_timerid_resolution_half = resolution_ms/2;
		}
		/// methods
		bool is_any_task_to_send() const
		{
			if (_m_timerid_list.empty()) return false;
			for (timerid_list_t::const_iterator i=_m_timerid_list.begin();i!=_m_timerid_list.end();++i)
			{
				timerid_item* item = *i;
				if (!item) continue;
				if (item->is_all_sent()) continue;
				return true;
			}
			return false;
		}
		//void add_null()
		//{
		//	timerid_item* item = new (std::nothrow) timerid_item;
		//	assert (item);
		//	item->index_tobe_send = 0;
		//	item->count_of_recved = 0;
		//	item->timerid = _m_timerid.value();
		//	item->timeout_ms_end = ox::mos::win_high_time::currrent_as_milli_second()+_m_timerid_resolution;
		//	_m_timerid_list.push_back(item);
		//	if (!on_timer_new.is_empty()) on_timer_new(item->timerid,item->timeout_ms_end);
		//}
		task_position add_task(task_t const& task,LONGLONG const& task_time_end)
		{
			task_position pos;
			pos.taskindex = -1;
			pos.timerid = -1;
			int tid = find_proper_timerid(task_time_end);
			if (tid>=0)
				pos.timerid = _m_timerid.value();
			else /// new a timer id
			{
				pos.timerid = _m_timerid.next();
				timerid_item* item = new (std::nothrow) timerid_item;
				item->index_tobe_send = 0;
				item->count_of_recved = 0;
				item->timerid = pos.timerid;
				item->timeout_ms_end = task_time_end;
				_m_timerid_list.push_back(item);
				if (!on_timer_new.is_empty()) on_timer_new(pos.timerid,task_time_end);
			}
			timerid_item* item = get_timer_item(pos.timerid);
			item->tasklist.push_back(task);
			pos.taskindex = item->tasklist.size()-1;
			return pos;
		}
		struct event_destructor
		{
			event_destructor(task_tobe_destructed_d const& on_task_tobe_destructed,void const* bind)
				: _m_on_task_tobe_destructed(on_task_tobe_destructed)
				, _m_bind(bind)
			{}
			void operator()(task_t const& task) const
			{
				if (!_m_on_task_tobe_destructed.is_empty())
					_m_on_task_tobe_destructed(task,_m_bind);
			}
			void const* _m_bind;
			task_tobe_destructed_d const& _m_on_task_tobe_destructed;
		};
		struct event_destructor_null
		{
			template <typename t> void operator()(t const&) {}
		};
		size_t clear_timerid(size_t timerid)
		{
			return clear_timerid<event_destructor_null>(timerid,event_destructor_null());
		}
		size_t clear_timerid(size_t timerid,task_tobe_destructed_d const& on_task_tobe_destructed,void const* bind)
		{
			return clear_timerid<event_destructor>(timerid,event_destructor(on_task_tobe_destructed,bind));
		}
		template <typename event_d>
		size_t clear_timerid(size_t timerid,event_d event)
		{
			size_t count = 0;
			timerid_item* item = get_timer_item(timerid);
			if (item==0) return count;
			for (task_list_t::iterator i=item->tasklist.begin();i!=item->tasklist.end();++i)
			{
				task_t& task = *i;
				if (!task) continue;
				++count;
				event(task);
				task.~task_t();
			}
			item->tasklist.clear();
			item->count_of_recved = 0;
			item->index_tobe_send = 0;
			try_erase_begin();
			return count;
		}
		void clear_task(task_position const& tp
			,task_tobe_destructed_d const& on_task_tobe_destructed,void const* bind
			,size_t* count)
		{
			clear_task(tp.timerid,tp.taskindex,on_task_tobe_destructed,bind,count);
		}
		void clear_task(size_t timerid,size_t subindex//,task_ptr taskptr
			,task_tobe_destructed_d const& on_task_tobe_destructed,void const* bind,size_t* pcount)
		{
			size_t count = 0;
			do {
				timerid_item* item = get_timer_item(timerid);
				if (item==0) break;
				task_ptr ptask = get_task(item,subindex);
				if (ptask.is_empty()) break;
				item->count_of_recved++;
				count ++;
				if (!on_task_tobe_destructed.is_empty())
					on_task_tobe_destructed(ptask.value(),bind);
				ptask.value().~task_t();
				try_erase_begin();
			} while(0);
			if (pcount) *pcount=count;
		}
		struct find_first_action
		{
			bool operator()(task_t& task,task_position const& tp,void* bind)
			{
				_m_finded = true;
				_m_tp = tp;
				return false;
			}
			bool _m_finded;
			task_position _m_tp;
		};
		task_position find_next_tobe_sent(bool* bfinded) const
		{
			find_first_action action;
			action._m_finded = false;
			action._m_tp.timerid = -1;
			action._m_tp.taskindex = -1;
			foreach_task_tobe_sent(action_d(&action),0);
			if (bfinded) *bfinded = action._m_finded;
			return action._m_tp;
		}
		bool clear_next_task_tobe_sent(
			task_tobe_destructed_d const& on_task_tobe_destructed,void const* bind,size_t* pcount)
		{
			bool is_finded_tp = false;
			task_position tp = find_next_tobe_sent(&is_finded_tp);
			if (!is_finded_tp) return false;
			timerid_item* item = get_timer_item(tp.timerid);
			if (item==0 || item->index_tobe_send!=tp.taskindex)
			{
				assert (false && "not find timerid or the task index is not equal to the next index to be sent");
				if (pcount) *pcount=0;
				return false;
			}
			assert (item->index_tobe_send<item->tasklist.size());
			item->index_tobe_send++; /// okay the index is right
			clear_task(tp,on_task_tobe_destructed,bind,pcount);
			return true;
		}
		void foreach_task_tobe_sent(action_d action,void* bind) const
		{
			for (int i=0;i<_m_timerid_list.size();++i)
			{
				timerid_item* item = _m_timerid_list[i];
				assert (item->timerid<=_m_timerid.value());
				task_position pos;
				pos.timerid = item->timerid;
				for (size_t j=item->index_tobe_send;j<item->tasklist.size();++j)
				{
					task_t& task = item->tasklist[j];
					pos.taskindex = j;
					if (!action(task,pos,bind)) break;
					item->index_tobe_send++;
				}
				assert (item->index_tobe_send <= item->tasklist.size());
			}
		}
		//template <typename action_tn>
		//void move_to_done(action_tn& action,size_t timerid)
		//{
		//	timerid_item* item = get_timer_item(timerid);
		//	for (size_t i=item->index_tobe_send;i<item->tasklist.size();++i)
		//	{
		//		task_t& task = item->tasklist[i];
		//		action(task);
		//	}
		//	item->index_tobe_send = item->tasklist.size();
		//}
		task_ptr get_task(task_position const& pos)
		{
			timerid_item* item = get_timer_item(pos.timerid);
			if (!item) return task_ptr();
			return get_task(item,pos.taskindex);
		}
		task_ptr get_task(timerid_item* item,size_t taskindex)
		{
			if (taskindex>=item->tasklist.size())
				return task_ptr();
			task_t& ret = item->tasklist[taskindex];
			return task_ptr(&ret);
		}
		timerid_item* get_timer_item(size_t timerid)
		{
			if (_m_timerid_list.empty())
				return 0;
			size_t timerid_begin = _m_timerid_list.front()->timerid;
			size_t timerid_back = _m_timerid_list.back()->timerid;
			size_t timerid_distance = _m_timerid.distance(timerid_begin,timerid);
			if (timerid_distance>_m_timerid_list.size())
			{
				/// all the task has been completed
				return 0;
			}
			timerid_item* item = _m_timerid_list[timerid_distance];
			if (!item)
			{
				assert (false && "some error happens");
				return 0;
			}
			return item;
		}
		/// we can not erase timer id from middle
		void try_erase_begin()
		{
			while (!_m_timerid_list.empty())
			{
				timerid_item* item = _m_timerid_list.front();
				if (!item)
				{
					_m_timerid_list.erase(_m_timerid_list.begin());
					continue;
				}
				if (!item->is_all_recved()) break;
				for (task_list_t::iterator i=item->tasklist.begin();i!=item->tasklist.end();++i)
					assert (!(*i));
				_m_timerid_list.erase(_m_timerid_list.begin());
				delete item;
			}
		}
		size_t timerid_begin() const
		{
			assert (!_m_timerid_list.empty());
			//if (_m_timerid_list.empty()) return -1;
			return _m_timerid_list.front().timerid;
		}
		size_t timerid_end() const
		{
			assert (!_m_timerid_list.empty());
			//if (_m_timerid_list.empty()) return -1;
			return _m_timerid_list.back().timerid+1;
		}
		size_t timerid_back() const
		{
			assert (!_m_timerid_list.empty());
			//if (_m_timerid_list.empty()) return -1;
			return _m_timerid_list.back().timerid;
		}
		/// -1: no result, should create a new timerid
		int find_proper_timerid(LONGLONG const& timeout_end) const
		{
			typedef timerid_list_t::reverse_iterator I;
			static size_t __max_count = 8;
			if (_m_timerid_list.empty()) return -1;
			size_t list_count = _m_timerid_list.size();
			for (int i=list_count-1;i>=0;--i)
			{
				if (i+__max_count<list_count) break;
				assert (_m_timerid_list[i]);
				timerid_item& ti = *_m_timerid_list[i];
				if (is_timeout_hit(timeout_end,ti.timeout_ms_end))
					return ti.timerid;
			}
			return -1;
		}
		bool is_timeout_hit(size_t timeout_ms_end,size_t timeout_ms_std) const
		{
			int dist = timeout_ms_std>timeout_ms_end
				? timeout_ms_std-timeout_ms_end
				: timeout_ms_end-timeout_ms_std;
			return dist<=_m_timerid_resolution_half;
		}
		timerid_list_t _m_timerid_list;
		timerid_t _m_timerid;
		size_t _m_timerid_tobe_sent;
		size_t _m_timerid_resolution_half;
		timer_new_d on_timer_new;
	};
}
	
	
___namespace2_end()