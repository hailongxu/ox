

#include "../ox/nsab.h"
#include "client_net.h"
//#include "timer_service.h"
#include "win_queue_thread_with_timer.h"


#pragma once


___namespace2_begin(ox,comm)


struct communication_service
{
	typedef communication_service self;
	typedef communication_define::all_exited_d all_exited_d;

	self()
	{
		_m_client_net.init();
	}

	void start_client_net()
	{
		_m_client_net.start();
		_m_timer.start();
	}
	void stop_client_net(all_exited_d const& _on_exited)
	{
		_m_client_net.stop(_on_exited);
		_m_timer.stop_next();
	}
	void wait_client_net()
	{
		_m_client_net.wait();
		_m_timer.join();
	}
	client_net _m_client_net;
	win_queue_thread_with_timer _m_timer;
};


___namespace2_end()