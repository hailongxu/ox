// thread_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/thread/task_feedback.h"
#include "../../src/thread/win_queue_thread.h"
#include "../../src/thread/win_queue_multi_thread.h"
#include "../../src/thread/win_queue_pool_thread.h"
#include <conio.h>


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
		printf ("thread is running");
		return 0;
	}
	void recall(ox::win_queue_thread* th)
	{
		th->add(ox::task_single<int>::make(&event::f1));
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
	}
}

namespace win_queue_thread_test
{
	ox::win_queue_thread thread;
	void test()
	{
		thread.add(ox::task_single<int>::make(&event::recall,&thread));
		thread.start();
	}
}

#include "../../src/thread/win_multi_thread.h"
namespace multi_thread_test
{
	typedef ox::win_thread<unsigned()> thread_t;
	typedef ox::win_multi_threads<thread_t> mth;
	mth m;
	unsigned f() {return 0;}
	void test()
	{
		//m.on_idle().assign(&event::on_idle);
		//m.on_busy().assign(&event::on_busy);
		thread_t* t = m.create_thread(&f);
		thread_t* t2 = m.create_thread(&f);
		m.stop();
		//t->add(ox::task_single<int>::make(&event::f1));
		//t2->add(ox::task_single<int>::make(&event::f1));
	}
}


namespace multi_queue_thread_test
{
	typedef ox::win_queue_multi_threads mth;
	mth m;
	void test()
	{
		m.on_idle().assign(&event::on_idle);
		m.on_busy().assign(&event::on_busy);
		thread_t* t = m.create_thread();
		thread_t* t2 = m.create_thread();
		t->add(ox::task_single<int>::make(&event::f1));
		t2->add(ox::task_single<int>::make(&event::f1));
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
		pool.set_max_min(1,1);
		pool.on_exited().add(event::on_exited);
		for (int i=0;i<1;++i)
		{
			//::_sleep(10);
			printf ("%d++++++++\n",i);
			pool.add(ox::task_single<int>::make(&event::f2,i));
		}
		int i = 9;
		Sleep(3000);
		pool.stop();
		pool.wait_subs();
		thread.stop();
		//pool.unsafe_terminate();
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

#include "../../src/thread/scope_raii.h"
namespace mutex_test
{
	win_mutex mutex("aa");
	void f1()
	{
		scope_mutex sync(mutex);
		::Sleep(1000);
		static int i = 0;
		printf ("t:%u  i%d\n",::GetCurrentThreadId(),++i);
	}
	void f2()
	{
		scope_mutex sync(mutex);
		static int i = 0;
		printf ("t:%u  i%d\n",::GetCurrentThreadId(),++i);
	}
	thread_t t1,t2;
	void test()
	{
		t1.start();
		t2.start();
		while (1)
		{
			t1.add(ox::task_single<int>::make(f1));
			t2.add(ox::task_single<int>::make(f2));
		}
	}
}



#include "../../src/cxx/delegate.h"
namespace delegate_test
{
	void f() {}
	void f2() {}
	void test()
	{
		delegate<void(void)> a(f),b(f2);
		bool c = a==b;
	}
}

namespace string_pass_test
{
	struct inf
	{
		inf()
		{
			seq = next();
			printf ("inf(%ld)\n",seq);
		}
		inf(inf const& o)
		{
			str = o.str;
			seq = next();
			printf ("inf copy(%ld)\n",seq);
		}
		~inf()
		{
			printf ("inf~~(%ld)\n",seq);
		}
		std::string str;
		long seq;
		static long next()
		{
			static long sseq=0;
			InterlockedIncrement(&sseq);
			return sseq;
		}
	};
	void f(inf& i)
	{
		i.str = "234";
	}
	thread_t t1;
	static void test()
	{
		t1.start();
		while(1)
		{
			getchar();
			inf i;
			t1.add(ox::task_single<int>::make(f,i));
		}
	}
}


//#include <vld.h>

int _tmain(int argc, _TCHAR* argv[])
{
	do {
		win_queue_thread_test::test();break;
		thread_performace_test::test();break;
		thread_test::test();break;
		string_pass_test::test();break;
		pool_thread_test::test();break;
		delegate_test::test();break;
		multi_queue_thread_test::test();break;
		multi_thread_test::test();break;
		mutex_test::test();break;
	} while(false);
	//Sleep(-1);
	//thread_t mt;
	//mt.start_here();
	getch();
	return 0;
}

