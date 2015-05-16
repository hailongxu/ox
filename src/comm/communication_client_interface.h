

#include "../ox/nsab.h"
#include "communication_define.h"
#include "http_define.h"
#include "task_decl.h"
//#include "multi_delegate.h"


#pragma once


___namespace2_begin(ox,comm)


struct communication_client_interface
{
	typedef communication_public::post_task_d post_task_d;
	typedef communication_define::reqid_t reqid_t;
	typedef http_define::http_sender_struct http_sender_struct;
	typedef http_define::http_user_sender_struct http_user_sender_struct;
	//typedef http_define::buffer_append_d buffer_append_d;

	virtual void init() = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void lize() = 0;
	//virtual void set_default_threadid(size_t) = 0;
	//virtual void set_default_post_task(post_task_d const&) = 0;
	virtual reqid_t http_send(http_user_sender_struct const&) = 0;
	virtual reqid_t http_send2(http_user_sender_struct const&) = 0;

	//virtual reqid_t http_send(http_sender_struct const&,buffer_append_d const&) = 0;
	//virtual reqid_t http_send_and_recv_all(http_sender_struct const&) = 0;
	virtual reqid_t tcp_send() = 0;
};


___namespace2_end()