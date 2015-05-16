
#include "task_decl.h"
#include "delegate.h"


#pragma once



namespace abc
{
	struct win_queue_thread_interface
	{
		typedef delegate<void(win_queue_thread_interface*)> exit_d;
		virtual void start() = 0;
		virtual void start_here() = 0;
		virtual void add(thread_task_t* task) = 0;
		virtual void add2(thread_task_t* task,size_t) = 0;
		virtual void stop_next() = 0;
		virtual void set_on_exit(exit_d const& on_exited) = 0;
	};
}