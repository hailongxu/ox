

#include "../ox/nsab.h"
#include "communication_client_interface.h"
#include "client_net.h"
#include "http_helper_internal.h"
#include "http_memory_helper_internal.h"


#pragma once


___namespace2_begin(ox,comm)


struct communication_client_service : communication_client_interface
{
	typedef abc::client_net client_t;
	typedef communication_client_interface base;
	typedef client_t::reqid_t reqid_t;
	typedef client_t::sender_struct sender_struct;
	typedef client_t::option_filemax option_filemax;
	typedef client_t::connection connection;
	typedef base::http_sender_struct http_sender_struct;
	typedef base::http_user_sender_struct http_user_sender_struct;

	virtual void init()
	{
		_m_client.init();
	}
	virtual void start()
	{
		_m_client.start();
	}
	virtual void stop()
	{
		_m_client.stop();
	}
	virtual void lize()
	{
		_m_client.lize();
	}
	//virtual void set_default_threadid(size_t default_threadid)
	//{
	//	_m_error_threadid_default = default_threadid;
	//}
	//virtual void set_default_post_task(post_task_d const& post_task)
	//{
	//	_m_post_task_default = post_task;
	//}

	virtual reqid_t http_send(http_user_sender_struct const& hss)
	{
		abc::http_helper_internal::sender_struct_default ss;
		ss.set(hss);
		reqid_t r2 = _m_client.send(ss);
		return r2;
	}
	virtual reqid_t http_send2(http_user_sender_struct const& hss)
	{
		abc::http_memory_helper_internal::sender_struct ss;
		ss.set(hss);
		reqid_t r2 = _m_client.send(ss);
		return r2;
	}
	//virtual reqid_t http_send_and_recv_all(http_sender_struct const&)
	//{
	//	return reqid_t();
	//}

	virtual reqid_t tcp_send()
	{
		return reqid_t();
	}
	//size_t _m_error_threadid_default;
	//post_task_d _m_post_task_default;
	client_t _m_client;
};


___namespace2_end()