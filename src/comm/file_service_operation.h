
#include "../ox/nsab.h"
#include "communication_const.h"
#include "communication_define.h"


#pragma once


___namespace2_begin(ox,comm)


struct server_accept_t
{
	typedef communication_define::file_handle_t file_handle_t;
	typedef delegate<void(void*)> exited_d;
	typedef delegate<void(size_t/*errorid*/,file_handle_t,char const*,void const* bind)> file_accepted_d;
	typedef delegate<void(ox::utl::cdata_t/*name*/)> started_d;

	typedef delegate<size_t(ox::utl::cdata_t)> start_d;
	typedef delegate<void()> stop_d;
	typedef delegate<void(size_t)> wait_d;
	typedef delegate<file_accepted_d&(void const* bind)> on_connected_d;
	typedef delegate<exited_d&()> on_exited_d;
	typedef delegate<started_d&()> on_started_d;
	typedef delegate<char const*()> name_d;

	start_d start;
	stop_d stop;
	wait_d wait;
	on_connected_d on_connected;
	on_started_d on_started;
	on_exited_d on_exit;
	name_d name;
};

struct client_connect_t
{
	typedef communication_define::file_handle_t file_handle_t;
	typedef communication_define::cdata_t cdata_t;
	typedef delegate<void(void*)> exited_d;
	struct connection_info
	{
		connection_info();
		size_t errorid;
		file_handle_t file_handle;
		ox::utl::cdata_t name;
		void* bind;
	};
	typedef delegate<void(connection_info const&)> file_connected_d;
	typedef file_connected_d file_before_connected_d;
	typedef delegate<void(connection_info const*,size_t)> connect_error_list_d;


	typedef delegate<void()> start_d;
	typedef delegate<void(int)> stop_d;
	typedef delegate<void(size_t)> wait_d;
	typedef delegate<exited_d&()> on_exited_d;
	typedef delegate<void(ox::utl::cdata_t,file_connected_d const&,void*)> connect_d;
	typedef delegate<void(file_before_connected_d const&)> set_on_before_connect_d;

	start_d start;
	stop_d stop;
	wait_d wait;
	connect_d connect;
	on_exited_d on_exit;
	set_on_before_connect_d set_on_before_connect;
};

struct file_handle_operation
{
	typedef communication_define::file_handle_t file_handle_t;
	typedef communication_define::cdata_t cdata_t;

	static file_handle_t invalid_file_handle()
	{
		return (file_handle_t)(~0);
	}
	static bool is_handle_invalid(file_handle_t handle)
	{
		return handle == invalid_file_handle();
	}

	typedef delegate<int()> init_d;
	typedef delegate<void()> uninit_d;
	typedef delegate<size_t(file_handle_t)> close_d;
	typedef delegate<int(file_handle_t,cdata_t,char*,OVERLAPPED*)> asend_d;
	typedef delegate<int(file_handle_t,cdata_t,char*,OVERLAPPED*)> arecv_d;
	typedef delegate<size_t()> last_error_d;

	struct wsabuf_t
	{
		unsigned long a;char* buf;
	};
	static size_t const __wsa_buf_size = sizeof(wsabuf_t);
	init_d init;
	uninit_d uninit;
	close_d close;
	asend_d asend;
	arecv_d arecv;
	last_error_d last_error;
};
inline client_connect_t::connection_info::connection_info()
	: errorid(0)
	, file_handle (file_handle_operation::invalid_file_handle())
	, bind(0)
{}


___namespace2_end()