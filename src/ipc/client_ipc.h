

#include "client_pipe_connect.h"
#include "file_pipe_operation.h"
#include "client_helper.h"
#include "communication_client.h"


#pragma once



namespace abc
{
	struct client_ipc
		: communication_client<client_helper>
	{
		typedef communication_client<client_helper> client_t;
		typedef client_pipe_connect<connect_pipe_trait> connect_t;
		typedef file_pipe_operation file_operation;
		typedef client_t base;

		void init()
		{
			client_t::init_param_set_client_connect(_m_connect.as_client_connect());
			client_t::init_param_set_handle_operation(file_operation::as_file_handle_operation());
			client_t::init();
		}

	private:
		connect_t _m_connect;
	};

} /// end of namespace abc