
#include <Windows.h>
#include <set>
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
	static LARGE_INTEGER current_ticks()
	{
		LARGE_INTEGER curr;
		QueryPerformanceCounter(&curr);
		return curr;
	}
	static void current_ticks(LARGE_INTEGER& curr)
	{
		QueryPerformanceCounter(&curr);
	}
	static LONGLONG currrent_as_100nano_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart*10000/__win_high_time_freq*1000;
	}
	static LONGLONG currrent_as_micro_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart*1000/__win_high_time_freq*10001;
	}
	static LONGLONG currrent_as_milli_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart*1000/__win_high_time_freq;
	}
	static LONGLONG currrent_as_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart/__win_high_time_freq;
	}
	static LONGLONG currrent_as_double_100nano_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart*100000000.0/__win_high_time_freq;
	}
	static LONGLONG currrent_as_double_micro_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart*1000000.0/__win_high_time_freq;
	}
	static LONGLONG currrent_as_double_milli_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart*1000.0/__win_high_time_freq;
	}
	static LONGLONG currrent_as_double_second()
	{
		LARGE_INTEGER curr;
		current_ticks(curr);
		return curr.QuadPart*1.0/__win_high_time_freq;
	}
	static LONGLONG elapsed_seconds(LONGLONG const& from,LONGLONG const& to)
	{
		return elapsed_ticks(from,to)/__win_high_time_freq;
	}
	static LONGLONG elapsed_milli_seconds(LONGLONG const& from,LONGLONG const& to)
	{
		return elapsed_ticks(from,to)*1000/__win_high_time_freq;
	}
	static LONGLONG elapsed_macro_seconds(LONGLONG const& from,LONGLONG const& to)
	{
		return elapsed_ticks(from,to)*1000*1000/__win_high_time_freq;
	}
	static LONGLONG elapsed_ticks(LONGLONG const& from,LONGLONG const& to)
	{
		return to-from;
	}
};
struct time_elapsed
{
	LARGE_INTEGER _m_from;
	LARGE_INTEGER _m_to;
	//LARGE_INTEGER _m_freq;
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
		//_m_freq = win_high_time::freq();
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
		LONGLONG current_100nano_second = win_high_time::currrent_as_100nano_second();
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
		LONGLONG curr_begin = win_high_time::currrent_as_100nano_second();
		timer_data const& timer = **_m_timer_list.begin();
		return curr_begin>=timer.time_coming_100nano_seconds;//<=_m_time_coming_100nano_second;
	}
	void run_first_and_remove()
	{
		LONGLONG curr_begin = win_high_time::currrent_as_100nano_second();
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
			curr_curr = win_high_time::currrent_as_100nano_second();
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
		td.time_coming_100nano_seconds = win_high_time::currrent_as_100nano_second() + td.period_100nano_seconds;

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

	template <typename task_tn>
	struct timer_resolution_task_system
	{
	
		/// type def itself
		typedef timer_resolution_task_system self;
		typedef task_tn task_t;
		typedef free_ptr<task_t> task_ptr;

		/// types defs'
		typedef delegate<bool(task_t&,task_position const&,void*)> action_d;
		typedef std::vector<task_t> task_list_t;
		struct timerid_t
		{
			timerid_t() : _m_id(0),_m_max(-1) {}
			size_t _m_id;
			size_t value() const {return _m_id;}
			size_t next() {if(++_m_id>_m_max) _m_id=0;return _m_id;}
			size_t _m_max;
		};
		struct timerid_item
		{
			task_list_t tasklist;
			size_t index_tobe_send;
			size_t count_of_recved;
			size_t timerid;
			bool is_all_recved() const
			{
				return count_of_recved>=tasklist.size();
			}
			bool is_all_sent() const
			{
				return index_tobe_send>=tasklist.size();
			}
		};
		typedef std::vector<timerid_item*> timerid_list_t;
		/// cons', dest'
		self()
		{
			_m_last_time_begin = ox::mos::win_high_time::currrent_as_milli_second();
			_m_milli_second_before = ox::mos::win_high_time::currrent_as_milli_second()+_m_timerid_resolution;
			_m_timerid_tobe_sent = 0;
			add_null();
		}
		void set_timerid_resolution(size_t resolution_ms)
		{
			_m_timerid_resolution = resolution_ms;
		}
		/// methods
		bool is_any_task_to_send() const
		{
			if (_m_timerid_list.empty()) return false;
			for (timerid_list_t::iterator i=_m_timerid_list.begin();i!=_m_timerid_list.end();++i)
			{
				timerid_item* item = *i;
				if (!item) continue;
				if (item->is_all_sent()) continue;
				return true;
			}
			return false;
		}
		void add_null()
		{
			timerid_item* item = new (std::nothrow) timerid_item;
			item->index_tobe_send = 0;
			item->count_of_recved = 0;
			item->timerid = _m_timerid.value();
			_m_timerid_list.push_back(item);
		}
		task_position add_task(task_t task)
		{
			return add_task(task,ox::mos::win_high_time::currrent_as_milli_second());
		}
		task_position add_task(task_t task,size_t task_time_begin)
		{
			task_position pos;
			pos.taskindex = -1;
			pos.timerid = -1;
			if (task_time_begin<_m_last_time_begin)
			{
				assert (false && "task time begin is not valid");
				return pos;
			}
			if (task_time_begin)
				pos.timerid = _m_timerid.value();
			if (task_time_begin>_m_milli_second_before)
			{
				_m_milli_second_before = task_time_begin+_m_timerid_resolution;
				pos.timerid = _m_timerid.next();
				timerid_item* item = new (std::nothrow) timerid_item;
				item->index_tobe_send = 0;
				item->count_of_recved = 0;
				item->timerid = pos.timerid;
				_m_timerid_list.push_back(item);
			}
			timerid_item* item = get_timer_item(pos.timerid);
			item->tasklist.push_back(task);
			pos.taskindex = item->tasklist.size()-1;
			return pos;
		}
		bool clear_task(size_t timerid,size_t subindex,task_ptr taskptr)
		{
			timerid_item* item = get_timer_item(timerid);
			if (item==0) return false;
			task_ptr ptask = get_task(item,subindex);
			if (ptask.is_empty()) return false;
			item->count_of_recved++;
			if (!taskptr.is_empty()) taskptr()=*ptask;
			ptask().~task_t();
			memset(ptask.pointer(),0,sizeof(task_t));
			if (!item->is_all_recved()) return false;
			try_erase_begin();
			return true;
		}
		void foreach_task_tobe_sent(action_d action,void* bind)
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
					action(task,pos,bind);
				}
				item->index_tobe_send = item->tasklist.size();
			}
		}
		//void foreach_task_tobe_recv(action_d action,void* bind)
		//{
		//	bool is_break = false;
		//	for (int i=0;i<_m_timerid_list.size();++i)
		//	{
		//		timerid_item* item = _m_timerid_list[i];
		//		for (size_t j=item->index_tobe_send;j<item->tasklist.size();++j)
		//		{
		//			task_t& task = item->tasklist[j];
		//			task_position pos;
		//			pos.timerid = item->timerid;
		//			pos.taskindex = j;
		//			is_break = action(task,pos,bind);
		//			if (is_break) break;
		//		}
		//		if (is_break) break;
		//	}
		//}
		template <typename action_tn>
		void move_to_done(action_tn& action,size_t timerid)
		{
			timerid_item* item = get_timer_item(timerid);
			for (size_t i=item->index_tobe_send;i<item->tasklist.size();++i)
			{
				task_t& task = item->tasklist[i];
				action(task);
			}
			item->index_tobe_send = item->tasklist.size();
		}
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
			bool is_valid_timerid = (timerid>=timerid_begin && timerid<=timerid_back);
			if (!is_valid_timerid)
			{
				/// all the task has been completed
				return 0;
			}
			size_t timerid_index = timerid-timerid_begin;
			timerid_item* item = _m_timerid_list[timerid_index];
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
					assert (*i==0);
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
		timerid_list_t _m_timerid_list;
		timerid_t _m_timerid;
		size_t _m_milli_second_before;
		size_t _m_last_time_begin;
		size_t _m_timerid_tobe_sent;
		size_t _m_timerid_resolution;
	};
}
	
	
___namespace2_end()