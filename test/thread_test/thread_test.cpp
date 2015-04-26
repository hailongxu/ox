// thread_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>

#include "../../src/thread/task_feedback.h"
#include "../../src/thread/win_queue_thread.h"
#include "../../src/thread/win_queue_multi_thread.h"
#include "../../src/thread/win_queue_pool_thread.h"
#include "../../src/thread/win_ui_thread.h"





typedef ox::win_queue_thread thread_t;


namespace event
{
	void f2(int i)
	{
		//Sleep(5);
		printf ("[%u %d] f2 f2 f2 f2 f2 f2 \n",GetCurrentThreadId(),i);
	}
	void f1()
	{
		//Sleep(500);
		printf ("[%u] f1 f1 f1 f1 f1 \n",GetCurrentThreadId());
	}
	void f3()
	{
		//Sleep(500);
		static int i = 0;
		char buf[64];
		sprintf (buf,"[%u,%d] tttttttttt\n",GetCurrentThreadId(),++i);
		OutputDebugStringA(buf);
	}
	void on_idle(thread_t* t)
	{
		printf ("%d is idle\n",t->threadid());
	}
	void on_busy(thread_t* t)
	{
		printf ("%d is busy\n",t->threadid());
	}
	void on_exited()
	{
		printf ("exit all\n");
	}
	int run()
	{
		printf ("thread is running\n");
		Sleep(5*1000);
		printf ("thread is running out\n");
		return 0;
	}
	void recall(ox::win_queue_thread* th)
	{
		th->add(ox::task_single<void>::make(&event::f1));
	}
}

namespace uithread_test
{
	ox::win_ui_thread ui;
	ox::win_queue_thread ui_task;
	bool timer_event()
	{
		static int i = 0;
		printf("timer #%i event is coming\n",++i);
		return i<5;
	}
	void post_task()
	{
		while(getch())
		{
			printf("post task\n");
			ui.add(ox::task_single<void>::make(&event::f3));
		}
	}
	void test()
	{
		ui.start();
		ui.add_timer(ox::timer_task_helper::make(timer_event),2*1000);
		ui_task.start();
		ui_task.add(ox::task_single<void>::make(&post_task));
		::MessageBoxA(0,0,0,0);
		//ui.stop();
	}
}

namespace thread_test
{
	typedef ox::win_thread<unsigned()> thread_t;
	thread_t thread;
	void test()
	{
		thread.on_run().assign(event::run);
		thread.start();
		printf ("join\n");
		thread.join();
		printf ("join finished\n");
	}
}

namespace win_queue_thread_test
{
	ox::win_queue_thread thread;
	void test()
	{
		thread.add(ox::task_single<void>::make(&event::recall,&thread));
		thread.start();
		Sleep(10);
		thread.stop();
	}
}

namespace multi_queue_thread_test
{
	typedef ox::win_queue_multi_threads mth;
	mth m(2,"multi_queue_thread_test");
	void test()
	{
		ox::win_queue_thread control;
		control.start();
		m.bind(&control);
		m.on_idle().assign(&event::on_idle);
		m.on_busy().assign(&event::on_busy);
		thread_t* t = m.create_thread();
		thread_t* t2 = m.create_thread();
		t->add(ox::task_single<void>::make(&event::f1));
		t2->add(ox::task_single<void>::make(&event::f1));
		m.stop();
		printf("sub stoped \n");
		control.stop();
		printf("control stoped \n");
	}
}


#include "../../src/thread/timer.h"
namespace thread_performace_test
{
	//ox::win_high_timer<ox::win_waitable_timer_engine> high;
	ox::win_queue_thread thread;
	void f() {}
	void test()
	{
		thread.start();
		//ox::time_elapsed counter;
		//counter.begin();
		for (int i=0;i<5/*600*1000*/;i++)
		{
			thread.add(ox::task_single<void>::make(f));
			//thread.add(ox::thread_task_helper::make(event::f2,0));
			//thread.add(ox::thread_task_helper::make(event::f2,0));
			//thread.add_high(ox::thread_task_helper::make(event::f1));
		}
		//counter.end();
		//printf ("%lf seconds\n",counter.double_elapsed_seconds());
	}
}



#include "../../src/thread/win_multi_thread.h"
namespace multi_thread_test
{
	typedef ox::win_thread<unsigned()> thread_t;
	typedef ox::win_queue_thread control_thread_t;
	typedef ox::win_multi_threads<thread_t> mth;
	mth m(1,"multi_thread_test");
	unsigned f() {return 0;}
	void test()
	{
		control_thread_t thread;
		thread.start();
		m.bind(&thread);
		//m.on_idle().assign(&event::on_idle);
		//m.on_busy().assign(&event::on_busy);
		thread_t* t = m.create_thread(&f);
		thread_t* t2 = m.create_thread(&f);
		m.astop();
		m.stop();
		thread.stop();
		//t->add(abc::task_single<int>::make(&event::f1));
		//t2->add(abc::task_single<int>::make(&event::f1));
	}
}

#include "../../src/thread/win_queue_thread_with_timer.h"
namespace thread_timer_test
{
	ox::win_queue_thread_with_timer thread;
	bool on_timer()
	{
		printf("on timer\n");
		return true;
	}
	void test()
	{
		thread.start();
		thread.add_timer_second(ox::timer_task_helper::make(on_timer),4);
	}
}


#include "../../src/thread/win_queue_pool_thread.h"
namespace thread_pool_test
{
	ox::win_queue_pool_thread pool;
	ox::win_queue_thread control;
	bool on_timer()
	{
		printf("on timer\n");
		return true;
	}
	void high_task()
	{
		Sleep(20);
		printf("high task\n");
	}
	void normal_task()
	{
		Sleep(10);
		printf("normal task\n");
	}
	bool is_normal_allowed(size_t i,size_t all)
	{
		printf("%d,%d\n",i,all);
		return i>1;
	}
	void test()
	{
		control.start();
		pool.bind(&control);
		pool.set_max_min(3,3);
		pool.on_exited().add(event::on_exited);
		pool.on_can_normal_task_run().assign(is_normal_allowed);
		for (int i=0;i<10;i++)
		{
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(normal_task));
			pool.add(ox::thread_task_helper::make(high_task));
		}
		Sleep(1500);
		//pool.astop();
		//pool.wait_subs();
		pool.stop();
		control.stop();
	}
}


//#include <vld.h>

int _tmain(int argc, _TCHAR* argv[])
{
	do {
		thread_pool_test::test();break;
		thread_performace_test::test();break;
		uithread_test::test(); break;
		thread_timer_test::test(); break;
        multi_thread_test::test();break;
		thread_test::test();break;
		win_queue_thread_test::test();break;
		multi_queue_thread_test::test();break;
	} while(false);
	//Sleep(-1);
	printf ("wait ... \n");
	thread_t mt;
	mt.start_here();
	printf ("get char and exit ... \n");
	//getchar();
	return 0;
}

