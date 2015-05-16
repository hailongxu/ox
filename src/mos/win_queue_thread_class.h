
#include "win_queue_thread_interface.h"
#include "win_queue_thread.h"



#pragma once



namespace abc
{
	struct win_queue_thread_class : win_queue_thread_interface
	{
		typedef win_queue_thread_class self;
		virtual void start()
		{
			_m_win_queue_thread.start();
		}
		virtual void start_here()
		{
			_m_win_queue_thread.start_here();
		}
		virtual void add(thread_task_t* task)
		{
			_m_win_queue_thread.add(task);
		}
		virtual void add2(thread_task_t* task,size_t)
		{
			_m_win_queue_thread.add2(task,0);
		}
		virtual void stop_next(exit_d const& on_exited)
		{
			_m_win_queue_thread.stop_next();
		}
		virtual void stop_next()
		{
			_m_win_queue_thread.stop_next();
		}
		virtual void set_on_exit(exit_d const& on_exited)
		{
			_m_interface_on_exit = on_exited;
			_m_win_queue_thread.on_exit().assign(this,&self::on_internal_exit);
		}

	private:
		void on_internal_exit(win_queue_thread* th)
		{
			_m_interface_on_exit(this);
		}

		exit_d _m_interface_on_exit;
		win_queue_thread _m_win_queue_thread;
	};
} /// end of namespace abc