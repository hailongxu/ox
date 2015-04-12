
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
		typedef thread_t::wait_enum wait_enum;
		static const int __messageid_task = WM_USER + 1;
		typedef safe_double_queue_tt<task_t*> task_queue;
		typedef task_queue control_queue;
		typedef task_queue::object_ptr task_pptr;

	public:
		~win_ui_thread() {}
		self(self const&);
		self& operator=(self const&);
		self()
		{
			thread_t::on_exit().assign(this,&self::on_internal_exit);
			thread_t::on_run().assign(this,&self::run);
			//init_window();
			_m_is_quit_set = false;
		}
		self(size_t your_id,char const* your_name)
			: thread_t(your_id,your_name)
		{
			thread_t::on_exit().assign(this,&self::on_internal_exit);
			thread_t::on_run().assign(this,&self::run);
			//init_window();
			_m_is_quit_set = false;
		}
		bool is_exiting() const
		{
			return 0!=_m_exit_enabled.value();
		}

		int add(task_t* task)
		{
			printf("add\n");
			if (!_m_normal_queue.is_add_enabled() || is_exiting()) return -1;
			assert (!is_exiting());
			_m_normal_queue.add(task);
			schedule_task();
			return 0;
		}
		int add2(task_t* task,size_t/*threadid, compatible with multi-thread*/)
		{
			return add(task);
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
			_m_normal_queue.add(task_single<int>::make(this,&self::do_set_quit_flag));
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
			// IF this was just a simple PeekMessage() loop (servicing all possible work
			// queues), then Windows would try to achieve the following order according
			// to MSDN documentation about PeekMessage with no filter):
			//    * Sent messages
			//    * Posted messages
			//    * Sent messages (again)
			//    * WM_PAINT messages
			//    * WM_TIMER messages
			//
			// Summary: none of the above classes is starved, and sent messages has twice
			// the chance of being processed (i.e., reduced service time).
			init_window();
			
			//for(MSG msg;;)
			//{ 
			//	GetMessage(&msg,0,0,0);
			//	TranslateMessage(&msg); 
			//	DispatchMessage(&msg); 
			//}
			//return 0;

			for (int has_more_work=0;;) 
			{
				// If we do any work, we may create more messages etc., and more work may
				// possibly be waiting in another task group.  When we (for example)
				// process_next_message(), there is a good chance there are still more
				// messages waiting.  On the other hand, when any of these methods return
				// having done no work, then it is pretty unlikely that calling them again
				// quickly will find any work to do.  Finally, if they all say they had no
				// work, then it is a good time to consider sleeping (waiting) for more
				// work.

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

				//{
				//	is_more_work_plausible |=
				//		m_pState->m_pDelegate->DoDelayedWork(&m_DelayedWorkTime);
				//}

				// If we did not process any delayed work, then we can assume that our
				// existing WM_TIMER if any will fire when delayed work should run.  We
				// don't want to disturb that timer if it is already in flight.  However,
				// if we did do all remaining delayed work, then lets kill the WM_TIMER.
				//if (is_more_work_plausible && m_DelayedWorkTime.IsNull())
				//	KillTimer(m_hMessageWnd, reinterpret_cast<UINT_PTR>(this));

				//if (m_pState->m_bShouldQuit)
				//	break;

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
			// If there are sent messages in the queue then PeekMessage internally
			// dispatches the message and returns false. We return true in this
			// case to ensure that the message loop peeks again instead of calling
			// MsgWaitForMultipleObjectsEx again.
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
			// When we encounter a kMsgHaveWork message, this method is called to peek
			// and process a replacement message, such as a WM_PAINT or WM_TIMER.  The
			// goal is to make the kMsgHaveWork as non-intrusive as possible, even though
			// a continuous stream of such messages are posted.  This method carefully
			// peeks a message while there is no chance for a kMsgHaveWork to be pending,
			// then resets the have_work_ flag (allowing a replacement kMsgHaveWork to
			// possibly be posted), and finally dispatches that peeked replacement.  Note
			// that the re-post of kMsgHaveWork may be asynchronous to this thread!!

			MSG msg;
			bool has_message = (0 != PeekMessage(&msg,0,0,0,PM_REMOVE));

			// Since we discarded a kMsgHaveWork message, we must update the flag.
			_m_has_task_slice = 0;

			// We don't need a special time slice if we didn't have_message to process.
			if (!has_message)
				return FALSE;

			// Guarantee we'll get another time slice in the case where we go into native
			// windows code.   This schedule_task() may hurt performance a tiny bit when
			// tasks appear very infrequently, but when the event queue is busy, the
			// kMsgHaveWork events get (percentage wise) rarer and rarer.
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
				// A WM_* message is available.
				// If a parent child relationship exists between windows across threads
				// then their thread inputs are implicitly attached.
				// This causes the MsgWaitForMultipleObjectsEx API to return indicating
				// that messages are ready for processing (specifically mouse messages
				// intended for the child window. Occurs if the child window has capture)
				// The subsequent PeekMessages call fails to return any messages thus
				// causing us to enter a tight loop at times.
				// The WaitMessage call below is a workaround to give the child window
				// sometime to process its input messages.
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
			_m_has_task_slice = 1;
			// Make sure the MessagePump does some work for us.
			post_task_message();
		}
		int pop_one_task_and_run()
		{
			OutputDebugStringA("pop_one_task_and_run enter\n");
			int has_task = 0;
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
					_m_is_busy = 1;
					runing()->run();
					_m_is_busy = 0;
					has_task = 1;
				}
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
				reinterpret_cast<self*>(wparam)->HandleWorkMessage();
				break;
			case WM_TIMER:
				//reinterpret_cast<self*>(wparam)->HandleTimerMessage();
				break;
			}

			return DefWindowProc(hwnd, message, wparam, lparam);
		}
		void HandleWorkMessage()
		{
			OutputDebugStringA("HandleWorkMessage\n");
			// If we are being called outside of the context of Run, then don't try to do
			// any work.  This could correspond to a MessageBox call or something of that
			// sort.
			//if (!m_pState)
			//{
			//	// Since we handled a kMsgHaveWork message, we must still update this flag.
			//	InterlockedExchange(&m_nHaveWork, 0);
			//	return;
			//}
			_m_has_task_slice = 0;

			// Let whatever would have run had we not been putting messages in the queue
			// run now.  This is an attempt to make our dummy message not starve other
			// messages that may be in the Windows message queue.
			ignore_task_message_and_process_next();

			// Now give the delegate a chance to do some work.  He'll let us know if he
			// needs to do more work.
			if (pop_one_task_and_run())
				schedule_task();
		}
		void HandleTimerMessage()
		{
			KillTimer(_m_message_wnd_handle, reinterpret_cast<UINT_PTR>(this));

			// If we are being called outside of the context of Run, then don't do
			// anything.  This could correspond to a MessageBox call or something of
			// that sort.
			//if (!m_pState)
			//	return;

			//m_pState->m_pDelegate->DoDelayedWork(&m_DelayedWorkTime);
			//if (!m_DelayedWorkTime.IsNull()) 
			//{
			//	// A bit gratuitous to set m_DelayedWorkTime again, but oh well.
			//	ScheduleDelayedWork(m_DelayedWorkTime);
			//}
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
		}
		void lize_window()
		{
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

	private:public:
		bool _m_is_quit_set;
		atomic_long _m_exit_enabled;
		atomic_long _m_is_busy;
		task_queue _m_normal_queue;
		exit_d _m_on_exit;
		HWND _m_message_wnd_handle;
		atomic_long _m_has_task_slice;
		static char const* wnd_class_name()
		{
			static char const* static_wnd_class_name = "Browser_MessagePumpWindow";
			return static_wnd_class_name;
		}

	}; /// end of win ui thread

} ///end of namespace ox
