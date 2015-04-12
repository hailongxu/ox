// thread_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/thread/task_feedback.h"
#include "../../src/thread/win_queue_thread.h"
#include "../../src/thread/win_queue_multi_thread.h"
#include "../../src/thread/win_queue_pool_thread.h"
#include <conio.h>
#include "../../src/thread/win_ui_thread.h"





typedef ox::win_queue_thread thread_t;


namespace event
{
	void f2(int i)
	{
		Sleep(5);
		printf ("[%u %d] ooooooo\n",GetCurrentThreadId(),i);
	}
	void f1()
	{
		//Sleep(500);
		printf ("[%u] tttttttttt\n",GetCurrentThreadId());
	}
	void f3()
	{
		//Sleep(500);
		char buf[64];
		sprintf (buf,"[%u] tttttttttt\n",GetCurrentThreadId());
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
		printf ("exit all");
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
		th->add(ox::task_single<int>::make(&event::f1));
	}
}

namespace chthread_test
{
	ox::win_ui_thread ui;
	ox::win_queue_thread ui_task;
	static const int __messageid_task = WM_USER + 1;
	HWND _m_message_wnd_handle;
	void post_task()
	{
		while(getchar())
		{
			printf("post task\n");
			ui.add(ox::task_single<int>::make(&event::f3));
		}
	}
	void test()
	{
		ui.start();
		ui_task.start();
		ui_task.add(ox::task_single<int>::make(&post_task));
		ui.stop();
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
		thread.add(ox::task_single<int>::make(&event::recall,&thread));
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
		t->add(ox::task_single<int>::make(&event::f1));
		t2->add(ox::task_single<int>::make(&event::f1));
		m.stop();
		printf("sub stoped \n");
		control.stop();
		printf("control stoped \n");
	}
}

typedef ox::win_queue_pool_thread pth;
pth pool;
namespace pool_thread_test
{
	ox::win_queue_thread thread;
	void test()
	{
		thread.start();
		pool.bind(&thread);
		pool.set_max_min(2,1);
		pool.on_exited().add(event::on_exited);
		for (int i=0;i<10;++i)
		{
			//::_sleep(10);
			printf ("%d++++++++\n",i);
			pool.add(ox::task_single<int>::make(&event::f2,i));
		}
		int i = 9;
		Sleep(3000);
		pool.astop();
		pool.wait_subs();
		thread.stop();
	}
}

#include "../../src/thread/timer.h"
namespace thread_performace_test
{
	ox::win_queue_thread thread;
	void f() {}
	void test()
	{
		thread.start();
		ox::time_elapsed counter;
		counter.begin();
		for (int i=0;i<600*1000;i++)
		{
			thread.add(ox::task_single<int>::make(f));
		}
		counter.end();
		printf ("%lf seconds\n",counter.double_elapsed_seconds());
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


//#include <vld.h>

int _tmain(int argc, _TCHAR* argv[])
{

	do {
        chthread_test::test(); break;
        //multi_thread_test::test();break;
		//thread_test::test();break;
		//win_queue_thread_test::test();break;
		//pool_thread_test::test();break;
		//thread_performace_test::test();break;
		//delegate_test::test();break;
		//multi_queue_thread_test::test();break;
		//mutex_test::test();break;
		//string_pass_test::test();break;
	} while(false);
	//Sleep(-1);
	printf ("wait ... \n");
	thread_t mt;
	mt.start_here();
	printf ("get char and exit ... \n");
	//getchar();
	return 0;
}

