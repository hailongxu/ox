
#include "task_decl.h"
#include "win_queue_thread_interface.h"
#include "winop.h"
#include <assert.h>


#pragma once



namespace abc
{
	//struct thread_user
	//{
	//	typedef win_queue_thread_interface thread_interface;
	//	typedef thread_interface::exit_d exit_d;

	//	thread_user(thread_interface* interface)
	//	{
	//		attach(interface);
	//	}
	//	void attach(thread_interface* interface)
	//	{
	//		_m_thread_interface = interface;
	//	}
	//	void detach()
	//	{
	//		_m_thread_interface = 0;
	//	}

	//	void start()
	//	{
	//		_m_thread_interface->start();
	//	}
	//	void add(thread_task_t* task)
	//	{
	//		_m_thread_interface->add(task);
	//	}
	//	void stop_next()
	//	{
	//		_m_thread_interface->stop_next();
	//	}
	//	void set_on_exit(exit_d const& on_exited)
	//	{
	//		_m_thread_interface->set_on_exit(on_exited);
	//	}
	//	thread_interface* interface()
	//	{
	//		return _m_thread_interface;
	//	}
	//private:
	//	thread_interface* _m_thread_interface;
	//};

	struct win_queue_thread_dll
	{
		typedef abc::win_queue_thread_interface thread_interface;
		typedef abc::win_queue_thread_interface::exit_d exit_d;
		typedef thread_interface* (__stdcall create_instance_fun)(void);
		typedef void (__stdcall destroy_instance_fun)(thread_interface*);
		
		thread_interface* create()
		{
			assert(_m_create_thread);
			if (!_m_create_thread) return 0;
			return _m_create_thread();
		}
		void destrory(thread_interface* thread)
		{
			assert(_m_destroy_thread);
			if (!_m_destroy_thread) return;
			_m_destroy_thread(thread);
		}

		bool load_dll(wchar_t const* communication_dll_path)
		{
			bool b = _m_dll.init(communication_dll_path);
			if (!b) return false;
			_m_create_thread = _m_dll.function<create_instance_fun>("create_win_queue_thread");
			_m_destroy_thread = _m_dll.function<destroy_instance_fun>("destroy_win_queue_thread");
			return _m_create_thread && _m_destroy_thread;
		}

		sys::dll _m_dll;
	private:
		create_instance_fun* _m_create_thread;
		destroy_instance_fun* _m_destroy_thread;
	};

} /// end of 