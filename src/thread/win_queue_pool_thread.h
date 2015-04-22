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


#include "win_queue_multi_thread.h"
#include "task_feedback.h"
#include <vector>



#pragma once




namespace ox
{
	struct win_queue_pool_thread
	{
		typedef win_queue_pool_thread self;
		typedef win_queue_thread thread_t;
		typedef win_queue_multi_threads multi_thread_t;
		typedef delegate<void(thread_t*)> started_d;
		typedef delegate<bool(size_t idle_count,size_t all_count)> can_normal_task_run_d;
		typedef multi_thread_t::exiting_d exiting_d;
		typedef multi_thread_t::exited_d exited_d;
		typedef unsafe_single_queue_tt<thread_task_t*> queue_t;
		typedef std::vector<size_t> numlist_t;
		typedef multi_thread_t::tasker_t tasker_t;
		typedef multi_thread_t::unsafe_done_d unsafe_done_d;

		static size_t const __thread_count_max_default = -1;
		static size_t const __thread_count_min_defualt = 1;

		self(size_t thread_max=__thread_count_max_default,
			size_t thread_min=__thread_count_min_defualt)
		{
			_m_thread_max_count = thread_max;
			_m_thread_min_count = thread_min;
			_m_multi_thread.on_idle().assign(this,&self::on_thread_idle);
			_m_multi_thread.on_exiting().add(this,&self::on_thread_exiting);
		}
		self(size_t your_id,char const* your_name)
		{
			_m_thread_max_count = __thread_count_max_default;
			_m_thread_min_count = __thread_count_min_defualt;
			_m_multi_thread.on_idle().assign(this,&self::on_thread_idle);
			_m_multi_thread.on_exiting().add(this,&self::on_thread_exiting);
			set_id(your_id,your_name);
		}

		void set_id(size_t yourid,char const* your_name=0)
		{
			_m_multi_thread.set_id(yourid,your_name);
		}
		void bind(thread_t* thread)
		{
			_m_multi_thread.bind(thread);
		}
		void unbind()
		{
			_m_multi_thread.unbind();
		}

		void add(thread_task_t* task)
		{
			_m_multi_thread.async_add(tasker_t::make(this,&self::do_add,task,false));
		}
		void add_high(thread_task_t* task)
		{
			_m_multi_thread.async_add_high(tasker_t::make(this,&self::do_add,task,true));
		}

		void set_max_min(size_t thread_max_size=__thread_count_max_default,
			size_t thread_min_size=__thread_count_min_defualt)
		{
			/// for the starting property setting
			if (!_m_multi_thread.thread()->is_started())
			{
				do_set_max_min(thread_max_size,thread_min_size);
				return;
			}
			_m_multi_thread.async_add(
				tasker_t::make(this,&self::do_set_max_min,thread_max_size,thread_min_size));
		}
		bool astop()
		{
			return _m_multi_thread.astop();
		}
		void stop(size_t timeout_ms=-1)
		{
			_m_multi_thread.stop(timeout_ms);
		}

		void wait_subs(size_t timeout_ms=-1)
		{
			_m_multi_thread.wait_subs(timeout_ms);
			_m_idle.clear();
		}

		started_d& on_started() {return _m_on_started;}
		exiting_d& on_exiting() {return _m_multi_thread.on_exiting();}
		exited_d& on_exited() {return _m_multi_thread.on_exited();}
		started_d const& on_started() const {return _m_on_started;}
		exiting_d const& on_exiting() const {return _m_multi_thread.on_exiting();}
		exited_d const& on_exited() const {return _m_multi_thread.on_exited();}
		can_normal_task_run_d& on_can_normal_task_run() {return _m_on_can_normal_task_run;}
		can_normal_task_run_d const& on_can_normal_task_run() const {return _m_on_can_normal_task_run;}
		
#if 0
		void unsafe_terminate()
		{
			unsafe_terminate(unsafe_done_d());
		}
		void unsafe_terminate(unsafe_done_d const& on_done)
		{
			_m_multi_thread.unsafe_terminate(on_done);
		}
#endif
	private:
		void do_add(thread_task_t* task,bool is_high)
		{
			size_t tid = deque_idle();
			if (tid!=0 && (is_high || can_normal_task_run()))
			{
				_m_multi_thread.add_task(task,tid);
				return;
			}
			add_to_queue(task,is_high);
			if (_m_multi_thread.unsafe_size()>=_m_thread_max_count)
				return;
			thread_t* th = _m_multi_thread.create_thread(-1,0,false);
			th->on_started().assign(this,&self::on_thread_started);
			th->start();
			//_m_multi_thread.create_thread(-1,0,true);
		}
		void on_thread_idle(thread_t* thread)
		{
			assert (thread->threadid()!=-1);
			if (_m_multi_thread.thread()==0 || ::GetCurrentThreadId()==_m_multi_thread.thread()->threadid())
				do_on_thread_idle(thread);
			_m_multi_thread.thread()->add(tasker_t::make(this,&self::do_on_thread_idle,thread));
		}
		void do_on_thread_idle(thread_t* thread)
		{
			size_t threadid = thread->threadid();
			typedef queue_t::object_ptr object_ptr;

			bool is_high = false;
			object_ptr ptr = get_from_queue(is_high);
			if (!ptr.is_empty() && (is_high || can_normal_task_run()))
			{
				thread->add(*ptr);
				return;
			}

			size_t all_size = _m_multi_thread.unsafe_size();
			if (all_size>_m_thread_min_count)
			{
				//assert(false && "something happened error");
				//_m_multi_thread.unsafe_terminate_tid(threadid);
				erase_idle(threadid);
				thread->stop_next();
				return;
			}
			enque_idle(threadid);
		}
		void on_thread_exiting(thread_t* thread,size_t size)
		{
			erase_idle(thread->threadid());
		}
		numlist_t::iterator find(numlist_t& list,size_t number)
		{
			typedef numlist_t::iterator I;
			for (I i=list.begin();i!=list.end();++i)
			{
				if (*i==number)
					return i;
			}
			return list.end();
		}

		size_t deque_idle()
		{
			if (_m_idle.empty())
				return 0;
			size_t ret = _m_idle.back();
			_m_idle.pop_back();
			return ret;
		}
		void enque_idle(size_t threadid)
		{
			typedef numlist_t::iterator I; 
			for (I i=_m_idle.begin();i!=_m_idle.end();++i)
				if (*i==threadid)
					return;
			_m_idle.push_back(threadid);
		}
		void erase_idle(size_t threadid)
		{
			typedef numlist_t::iterator I; 
			for (I i=_m_idle.begin();i!=_m_idle.end();++i)
				if (*i==threadid)
				{
					_m_idle.erase(i);
					return;
				}
		}

		void do_try_add_thread()
		{
			if (_m_multi_thread.unsafe_size()>=_m_thread_max_count)
				return;
			_m_multi_thread.create_thread(-1,0,true);
		}

		struct stop_if_morethan_min
		{
			size_t min_size;
			size_t size_tobe_stopped;
			bool operator()(thread_t* thread)
			{
				if (size_tobe_stopped>0)
				{
					thread->stop_next();
					size_tobe_stopped--;
				}
				return size_tobe_stopped>0;
			}
		};

		void do_set_max_min(size_t max_size,size_t min_size)
		{
			if (max_size!=__thread_count_max_default)
				_m_thread_max_count = max_size;
			if (min_size>max_size) min_size = _m_thread_max_count;
			_m_thread_min_count = min_size;
			size_t size = _m_multi_thread.unsafe_size();
			if (size <= min_size) return;
			stop_if_morethan_min stopit;
			stopit.min_size = min_size;
			stopit.size_tobe_stopped = size-min_size;
			typedef multi_thread_t::action_d action_d;
			_m_multi_thread.foreach_do(action_d(&stopit));
		}

		/// run in control thread
		void on_unsafe_terminate_before_start()
		{
			_m_idle.clear();
			typedef queue_t::object_ptr object_ptr;
			while (1)
			{
				object_ptr objptr = _m_queue.get();
				if (objptr.is_empty())
					break;
				objptr()->destroy();
			}
		}

		void on_thread_started(thread_t::base* thread)
		{
			if (_m_on_started.is_empty()) return;
			_m_on_started(static_cast<thread_t*>(thread));
		}

		void add_to_queue(thread_task_t* task,bool is_high)
		{
			is_high?_m_queue_high.add(task):_m_queue.add(task);
		}
		queue_t::object_ptr get_from_queue(bool& is_high)
		{
			is_high = true;
			queue_t::object_ptr ptr = _m_queue_high.get();
			if (ptr.is_empty()) ptr = _m_queue.get(),is_high=false;
			return ptr;
		}
		bool can_normal_task_run() const
		{
			if (_m_on_can_normal_task_run.is_empty()) return true;
			size_t idle_count = _m_idle.size()+1;
			size_t all_count = _m_multi_thread.unsafe_size();
			return _m_on_can_normal_task_run(idle_count,all_count);
		}


		started_d _m_on_started;
		can_normal_task_run_d _m_on_can_normal_task_run;
		size_t _m_thread_min_count;
		size_t _m_thread_max_count;
		multi_thread_t _m_multi_thread;
		queue_t _m_queue;
		queue_t _m_queue_high;
		numlist_t _m_idle;
	};

} ///end of namespace ox
