

#include "server_pipe_accept.h"
#include "file_pipe_operation.h"
#include "client_helper.h"
#include "communication_client.h"


#pragma once



namespace abc
{
	struct server_ipc
		: communication_client<client_helper>
	{
		typedef communication_client<client_helper> server_t;
		typedef server_pipe_accept<accept_pipe_trait> accept_t;
		typedef file_pipe_operation file_operation;

		void init()
		{
			server_t::init_param_set_server_accept(_m_accept.as_server_accept());
			server_t::init_param_set_handle_operation(file_operation::as_file_handle_operation());
			server_t::init();
		}

	private:
		accept_t _m_accept;
	};

} /// end of namespace abc