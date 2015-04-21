
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */


#include <Windows.h>
#include "win_thread.h"
#include "atomic_number.h"
#include "safe_double_queue.h"
#include "timer.h"



#pragma once




/*
1. on_shedule_gap()
2. transfer queue, that is, promote the efficiency of swapping queue, when there is
thousands of thread being prepare to add message to the wait or next queue
3. is-idle(), show that the thread must be at idle, but not-idle is not accurate //absolutely
4. condition queue
5. threads: thread pool, all threads will trigger pool when it starts or stops
6. register server or function
7. clear(), will clear all the queue
8. notify self, post self
9. the listener can sniff the certain event, that is, we have two options, one is for
architecture to design the flow, the other is for the programmer to do at their will
10. return suspend, exit, go on next, instead of true/false, on the run function
11. thread suspend, kill from system, or from self
12. the atomic unit of schedule is the whole procedure of run()
*/


namespace ox
{
	struct win_ui_thread : win_thread<unsigned()>
	{
		typedef win_ui_thread self;
		typedef win_thread<unsigned()> thread_t;
		typedef delegate<void(self*)> idle_d;
		typedef delegate<void(self*)> busy_d;
		typedef delegate<void(self*)> exit_d;
		typedef thread_task_t task_t;
		typedef timer_task_t timer_task_t;
		typedef thread_t::wait_enum wait_enum;
        struct ui_task_t
        {
            task_t* task;
			bool is_timer;
            size_t after_milli;
        };
		typedef safe_double_queue_tt<ui_task_t> task_queue;
		typedef task_queue control_queue;
		typedef task_queue::object_ptr task_pptr;
        static const int __messageid_task = WM_USER + 1;

	private:
		typedef win_timer_list timer_list_t;
		struct timer_engine_t
		{
			timer_engine_t()
				: _m_hwnd(0), _m_timerid(0)
			{}
			void init(HWND hwnd,UINT_PTR timerid)
			{
				_m_hwnd = hwnd;
				_m_timerid = timerid;
			}
			void set_after_milli(size_t milli_seconds)
			{
				assert(_m_hwnd!=0);
				assert(_m_timerid!=0);
				::SetTimer(_m_hwnd,_m_timerid,milli_seconds,0);
			}
			void kill()
			{
				::KillTimer(_m_hwnd,_m_timerid);
			}
			void uninit()
			{
				kill();
			}
			HWND _m_hwnd;
			UINT_PTR _m_timerid;
		};
	public:
		~win_ui_thread() {}
		self(self const&);
		self& operator=(self const&);
		self()
		{
			thread_t::on_exit().assign(this,&self::on_internal_exit);
			thread_t::on_run().assign(this,&self::run);
			_m_is_quit_set = false;
		}
		self(size_t your_id,char const* your_name)
			: thread_t(your_id,your_name)
		{
			thread_t::on_exit().assign(this,&self::on_internal_exit);
			thread_t::on_run().assign(this,&self::run);
			_m_is_quit_set = false;
		}
		bool is_exiting() const
		{
			return 0!=_m_exit_enabled.value();
		}

		int add(task_t* task)
		{
			if (!_m_normal_queue.is_add_enabled() || is_exiting()) return -1;
			assert (!is_exiting());
			ui_task_t ui_task;
			ui_task.is_timer = false;
			ui_task.task = task;
			_m_normal_queue.add(ui_task);
			schedule_task();
			return 0;
		}
		int add2(task_t* task,size_t/*threadid, compatible with multi-thread*/)
		{
			return add(task);
		}
		size_t add_timer(timer_task_t* timer_task,LONGLONG period_milli_seconds)
		{
			if (!_m_normal_queue.is_add_enabled() || is_exiting()) return -1;
			assert (!is_exiting());
			ui_task_t ui_task;
			ui_task.is_timer = true;
			ui_task.task = (task_t*)timer_task;
			ui_task.after_milli = period_milli_seconds;
			_m_normal_queue.add(ui_task);
			schedule_task();
			return 0;
		}

		bool clear()
		{
			_m_normal_queue.clear_outer();
		}

		bool start() {return thread_t::start();}
		bool start_here() {return thread_t::start_here();}
		bool stop_next()
		{
			OutputDebugStringA("stop_next\n");
			if (!_m_normal_queue.is_add_enabled()) return false;
			if (get_is_exiting_or_set()) return false;
			ui_task_t ui_task;
			ui_task.task = task_single<void>::make(this,&self::do_set_quit_flag);
			ui_task.is_timer = false;
			_m_normal_queue.add(ui_task);
			schedule_task();
			OutputDebugStringA("stop_next return\n");
		}

		wait_enum stop()
		{
			bool b = is_stop_allowed(/*handle*/);
			if (!b) return thread_t::__notallowed;
			stop_next();
			return thread_t::join_thread_id(thread_t::threadid());
		}
		exit_d& on_exit() {return _m_on_exit;}
		exit_d const& on_exit() const {return _m_on_exit;}

	private: /// the message process's main procedure
		unsigned run()
		{
			init_window(); /// should init in its thread
			
			for (int has_more_work=0;;) 
			{
				OutputDebugStringA("0\n");

				has_more_work = process_next_message();
				if (_m_is_quit_set) break;
				OutputDebugStringA("1\n");
				has_more_work |= pop_one_task_and_run();
				if (_m_is_quit_set)
				{
					OutputDebugStringA("_m_is_quit_set\n");
					break;
				}
				OutputDebugStringA("2\n");

				has_more_work |= pop_one_timer_task_and_run();
				if (has_more_work && _m_timer_list.is_empty())
					_m_timer_engine.kill();

				if (_m_is_quit_set)
				{
					OutputDebugStringA("_m_is_quit_set 2\n");
					break;
				}

				if (has_more_work) continue;
				OutputDebugStringA("3\n");
				has_more_work = run_one_idle_task();
				if (_m_is_quit_set) break;
				if (has_more_work) continue;
				OutputDebugStringA("4\n");

				wait_message_arrived();
			}

			lize_window();
			OutputDebugStringA("loop quit\n");
			return 0;
		}
		int process_next_message()
		{
			/// ??? is peek message handled all the sent message?????
			OutputDebugStringA("process_next_message\n");
			MSG msg;
			int is_sent_message_in_queue = 0;
			DWORD queue_status = GetQueueStatus(QS_SENDMESSAGE);
			if (HIWORD(queue_status) & QS_SENDMESSAGE)
				is_sent_message_in_queue = 1;
			if (PeekMessage(&msg,0,0,0,PM_REMOVE))
				return process_any_message(msg);
			return is_sent_message_in_queue;
		}
		bool process_any_message(const MSG& msg) 
		{
			char buf[32];sprintf(buf,"----msgid:%u\n",msg.message);
			OutputDebugStringA(buf);
			if (is_quit_message(msg)) 
			{
				_m_is_quit_set = true;
				PostQuitMessage(static_cast<int>(msg.wParam));
				return false;
			}

			// While running our main message pump, we discard kMsgHaveWork messages.
			if (is_task_message(msg))
				return ignore_task_message_and_process_next();

			//if (CallMsgFilter(const_cast<MSG*>(&msg), kMessageFilterCode))
			//	return TRUE;

			//if (!PreTranslateMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			return true;
		}
		bool ignore_task_message_and_process_next() 
		{
			MSG msg;
			bool has_message = (0 != PeekMessage(&msg,0,0,0,PM_REMOVE));
			task_lose_time_slice();
			if (!has_message) return false;
			schedule_task();

			return process_any_message(msg);
		}
		void wait_message_arrived() 
		{
			// Wait until a message is available, up to the time needed by the timer
			// manager to fire the next set of timers.
			INT nDelay = -1;//GetCurrentDelay();
			if (nDelay < 0)  // Negative value means no timers waiting.
				nDelay = INFINITE;

			OutputDebugStringA("MsgWaitForMultipleObjectsEx 1\n");
			DWORD dwResult = MsgWaitForMultipleObjectsEx(
				0,0,static_cast<DWORD>(nDelay),QS_ALLINPUT,//QS_ALLEVENTS,
				MWMO_INPUTAVAILABLE|MWMO_ALERTABLE);

			OutputDebugStringA("MsgWaitForMultipleObjectsEx 2\n");
			if (WAIT_OBJECT_0 == dwResult) 
			{
				MSG msg = {0};
				DWORD dwQueueStatus = GetQueueStatus(QS_MOUSE);
				if (HIWORD(dwQueueStatus) & QS_MOUSE &&
					!PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE)) 
				{
					WaitMessage();
				}
				return;
			}
		}
	private:
		void schedule_task() 
		{
			OutputDebugStringA("schedule_task\n");
			if (is_task_got_time_slice()) return;
			task_get_time_slice();
			// Make sure the MessagePump does some work for us.
			post_task_message();
		}
		void schedule_timer_task()
		{
			OutputDebugStringA("schedule_timer_task\n");
			LONGLONG relative_100nano = 0;
			_m_timer_list.calculate_next_timer(&relative_100nano);
			_m_timer_engine.set_after_milli(-relative_100nano/10/1000);
			//printf ("time %d milli seconds",-relative_100nano/10/1000);
		}
		int pop_one_task_and_run()
		{
			OutputDebugStringA("pop_one_task_and_run enter\n");
			int has_task = 0;
			while(1)
			{
				task_pptr runing = _m_normal_queue.get();
				if (runing.is_empty())
				{
					_m_normal_queue.swap();
					runing = _m_normal_queue.get();
				}
				if (!runing.is_empty())
				{
					//if (!on_busy().is_empty()) thread_t::_m_on_busy(this);
					//is_busying = true;
					_m_normal_queue.runing->pop();
					//_m_is_busy = 1;
					ui_task_t& ui_task = runing();
					if (ui_task.is_timer)
					{
						add_to_timer_list(ui_task);
						continue;
					}
					ui_task.task->run();
					ui_task.task->destroy();
					//_m_is_busy = 0;
					has_task = 1;
				}
				break;
			}
			OutputDebugStringA("pop_one_task_and_run return\n");
			return has_task;
		}
		int run_one_idle_task()
		{
			return 0; /// no idle work
		}
		static LRESULT CALLBACK message_wnd_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
		{
			OutputDebugStringA("message_wnd_proc  aaaaaaaaaaa\n");
			switch (message) 
			{
			case __messageid_task:
				OutputDebugStringA("message_wnd_proc  ssssssss\n");
				//if (get_is_disable_pump()) return 0;
				reinterpret_cast<self*>(wparam)->handle_task_message();
				break;
			case WM_TIMER:
				reinterpret_cast<self*>(wparam)->handle_timer_message();
				break;
			}

			return DefWindowProc(hwnd, message, wparam, lparam);
		}
		void handle_task_message()
		{
			OutputDebugStringA("HandleWorkMessage\n");
			task_lose_time_slice();
			ignore_task_message_and_process_next();
			if (pop_one_task_and_run())
				schedule_task();
		}
		void handle_timer_message()
		{
			//printf("pop_one_timer_task_and_run 222222222222\n");
			_m_timer_engine.kill();
			pop_one_timer_task_and_run();
			schedule_timer_task();
		}
		bool is_quit_message(MSG const& msg) const
		{
			return msg.message==WM_QUIT;
		}
		bool is_task_message(MSG const& msg) const
		{
			return msg.message==__messageid_task && msg.hwnd==_m_message_wnd_handle;
		}
		void post_task_message()
		{
			OutputDebugStringA("post_task_message\n");
			//assert(IsWindow(_m_message_wnd_handle));
			//OutputDebugStringA("post_task_message 2\n");
			BOOL b = PostMessage(_m_message_wnd_handle, __messageid_task, reinterpret_cast<WPARAM>(this), 0);
#ifdef _DEBUG
			if (b==FALSE)
			{
				DWORD err = GetLastError();
			}
#endif
		}
		static bool get_is_disable_pump()
		{
			char flag_name[MAX_PATH];
			sprintf_s(flag_name, MAX_PATH, "FLAG_DISABLE_PUMP-%d", GetCurrentProcessId());
			ATOM flag = FindAtomA(flag_name);
			return flag!=0;
		}

		void do_set_quit_flag()
		{
			OutputDebugStringA("do_set_quit_flag\n");
			_m_is_quit_set = true;
		}
		void init_window()
		{
			register_class(wnd_class_name(),message_wnd_proc);
			_m_message_wnd_handle =	create_message_window(wnd_class_name());
			_m_timer_engine.init(_m_message_wnd_handle,(UINT_PTR)this);
		}
		void lize_window()
		{
			_m_timer_engine.kill();
			_m_timer_engine.uninit();
			DestroyWindow(_m_message_wnd_handle);
			UnregisterClassA(wnd_class_name(), GetModuleHandle(0));
		}
		void on_internal_exit(thread_t* thread)
		{
			assert (thread==this);
			clear_resource();
			if (_m_on_exit.is_empty()) return;
			_m_on_exit(this);
		}
		static void register_class(char const* class_name,WNDPROC window_proc)
		{
			assert(class_name);
			HINSTANCE exe_handle = GetModuleHandle(0);
			WNDCLASSEXA wc = {0};
			wc.cbSize = sizeof(wc);
			wc.lpfnWndProc = window_proc;
			wc.hInstance = exe_handle;
			wc.lpszClassName = class_name;
			RegisterClassExA(&wc);
		}
		static HWND create_message_window(char const* class_name)
		{
			assert(class_name);
			HINSTANCE exe_handle = GetModuleHandle(0);
			return CreateWindowA(class_name,0,0,0,0,0,0,HWND_MESSAGE,0,exe_handle,0);
		}
		void do_clear_runing()
		{
			if (GetCurrentThreadId()!=_m_threadid)
			{
				assert(false && "current thread is not the thread class!");
				return;
			}
			_m_normal_queue.clear_runing();
		}
		bool get_is_exiting_or_set()
		{
			return _m_exit_enabled.assign_and_return_old(1);
		}
		bool is_stop_allowed(/*HANDLE& h*/)
		{
			bool is_allowed = (GetCurrentThreadId()!=_m_threadid);
			assert (is_allowed);
			return is_allowed;
		}
		void clear_resource()
		{
			_m_normal_queue.uninit();
			_m_exit_enabled=1;
		}
        static char const* wnd_class_name()
        {
            static char const* static_wnd_class_name = "win_ui_thread_message_window";
            return static_wnd_class_name;
        }
		void add_to_timer_list(ui_task_t const& task)
		{
			_m_timer_list.add_timer((timer_task_t*)task.task,task.after_milli*1000,(size_t)(this),0);
			schedule_timer_task();
		}
		int pop_one_timer_task_and_run()
		{
			if (_m_timer_list.is_empty()) return 0;
			if (!_m_timer_list.is_first_coming()) return 0;
			_m_timer_list.run_first_and_remove();
			schedule_timer_task();
			return 1;
		}
		bool is_task_got_time_slice() const {return _m_has_task_slice.value();}
		void task_get_time_slice() {_m_has_task_slice = 1;}
		void task_lose_time_slice() {_m_has_task_slice = 0;}

	private:
		bool _m_is_quit_set;
		atomic_long _m_exit_enabled;
		task_queue _m_normal_queue;
		exit_d _m_on_exit;
		HWND _m_message_wnd_handle;
		atomic_long _m_has_task_slice;
		timer_engine_t _m_timer_engine;
		timer_list_t _m_timer_list;
	}; /// end of win queue thread

} ///end of namespace ox
