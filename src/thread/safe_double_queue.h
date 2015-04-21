/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */


#include <windows.h>
#include <queue>
#include <assert.h>
#include "../cxx/delegate.h"
#include "delegate_closure.h"
#include "atomic_number.h"
#include "scope_raii.h"



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
archetecter to design the flow, the other is for the programer to do at their will
10. return suspend, exit, go on next, instead of true/false, on the run function
11. thread suspend, kill from system, or from self
12. the atomic unit of schedule is the whole procedure of run()
*/


namespace ox
{
	template <typename t>
	struct free_ptr
	{
		free_ptr(t* p=0):_m_p(p) {}
		t* operator->() {return _m_p;}
		t& operator*() {return *_m_p;}
		free_ptr& operator=(t* p) {_m_p=p;return *this;}
		bool is_empty() const {return _m_p==0;}
		t& operator()() {return *_m_p;}
	private:
		t* _m_p;
	};

	template <typename object_tn>
	struct safe_double_queue_tt
	{
		typedef object_tn object_t;
		typedef std::queue<object_t> single_task_queue_t;
		typedef typename single_task_queue_t::reference object_reference;
		typedef free_ptr<object_t> object_ptr;

		safe_double_queue_tt()
		{
			construct();
		}
		~safe_double_queue_tt()
		{
		}
		void construct()
		{
			waiting = &waiting_object;
			runing = &runing_object;
			_m_is_waiting_more = 0;
			_m_is_runing_more = 0;
			_m_htaskadded = 0;
			_m_is_add_enabled = 1;
		}
		void init()
		{
			construct();
			_m_htaskadded = CreateEvent(0,FALSE,FALSE,0);
		}
		void uninit()
		{
			CloseHandle(_m_htaskadded);
			_m_htaskadded = 0;
		}
		HANDLE event_added_handle() const
		{
			return _m_htaskadded;
		}
		bool add_enabled(bool b)
		{
			_m_is_add_enabled = b?1:0;
		}
		bool is_add_enabled() const
		{
			return _m_is_add_enabled.value()!=0;
		}
		void add(object_t const& object)
		{
			if (!_m_is_add_enabled)
				return;
			scope_cs swap_outer_sync(_m_critical_section_outer);
			scope_cs swap_inner_sync(_m_critical_section_inner);
			waiting->push(object);
			_m_is_waiting_more = 1;
			if (_m_htaskadded!=0) SetEvent(_m_htaskadded);
		}
		void swap()
		{
			scope_cs swap_inner_sync(_m_critical_section_inner);
			unsafe_swap();
		}
		void unsafe_swap()
		{
			single_task_queue_t* queue = waiting;
			waiting = runing;
			runing = queue;
			_m_is_waiting_more = !(waiting->empty());
			_m_is_runing_more = !(runing->empty());
			ResetEvent(_m_htaskadded);
		}
		void clear_outer()
		{
			scope_cs swap_outer_sync(_m_critical_section_outer);
			while(!waiting_object.empty())
				waiting_object.pop();
		}
		/// for using inside
		void clear_runing()
		{
			while(!runing_object.empty())
				runing_object.pop();
		}
		object_ptr get()
		{
			while (1)
			{
				if (runing->empty())
				{
					_m_is_runing_more = 1;
					return object_ptr(0);
				}
				object_reference object = runing->front();
				//if (object==0)
				//	continue;
				return object_ptr(&object);
			}
			return object_ptr(0);
		}
		object_ptr pop()
		{
			object_ptr obj = get();
			if (obj.is_empty())
				return obj;
			runing->pop();
			return obj;
		}
		object_ptr pop_swap()
		{
			object_ptr obj = get();
			if (obj.is_empty())
			{
				unsafe_swap();
				obj = get();
			}
			if (!obj.is_empty())
				runing->pop();
			return obj;
		}
		object_ptr safe_pop_swap()
		{
			scope_cs swap_inner_sync(_m_critical_section_inner);
			return pop_swap();
		}
		bool is_waiting_empty() const
		{
			return !_m_is_waiting_more.value();
		}
		bool is_runing_empty() const
		{
			return !_m_is_runing_more.value();
		}
		bool is_empty() const
		{
			return !is_waiting_empty() && !is_runing_empty();
		}
		single_task_queue_t* waiting;
		single_task_queue_t* runing;
		single_task_queue_t waiting_object;
		single_task_queue_t runing_object;
		HANDLE _m_htaskadded;
		atomic_long _m_is_waiting_more;
		atomic_long _m_is_runing_more;
		atomic_long _m_is_add_enabled;
		critical_section _m_critical_section_outer;
		critical_section _m_critical_section_inner;
	};

	template <typename object_tn>
	struct safe_single_queue_tt
	{
		typedef object_tn object_t;
		typedef std::queue<object_t> single_task_queue_t;
		typedef free_ptr<object_t> object_ptr;

		void add(object_t obj)
		{
			scope_cs sync(_m_critical_section);
			_m_queue.push(obj);
		}
		object_ptr get()
		{
			scope_cs sync(_m_critical_section);
			if (_m_queue.empty())
				return object_ptr(0);
			object_t& obj = _m_queue.front();
			_m_queue.pop();
			return object_ptr(&obj);
		}
		size_t size()
		{
			scope_cs sync(_m_critical_section);
			return _m_queue.size();
		}

	private:
		single_task_queue_t _m_queue;
		critical_section _m_critical_section;
	};

	template <typename object_tn>
	struct unsafe_single_queue_tt
	{
		typedef object_tn object_t;
		typedef std::queue<object_t> single_task_queue_t;
		typedef free_ptr<object_t> object_ptr;

		void add(object_t obj)
		{
			_m_queue.push(obj);
		}
		object_ptr get()
		{
			if (_m_queue.empty())
				return object_ptr(0);
			object_t& obj = _m_queue.front();
			_m_queue.pop();
			return object_ptr(&obj);
		}
		size_t size()
		{
			return _m_queue.size();
		}

	private:
		single_task_queue_t _m_queue;
	};
} ///end of namespace ox
