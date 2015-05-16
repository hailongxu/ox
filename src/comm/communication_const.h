

#include "../ox/nsab.h"
#include "error_const.h"


#pragma once


___namespace2_begin(ox,comm)

struct communication_const : error_const
{
	enum actionid_enum
	{
		__file_read='r',
		__file_write='w',
		__file_exit=1,
		__file_error,
		__file_task,
		__file_accept,
		__file_connect,
	};
	enum protocol_enum
	{
		__protocol_tcp,
		__protocal_udp,
		__protocal_http,
		__protocol_ipc,
	};
	enum file_dispatchable_enum
	{
		__undispatchable,
		__dispatchable,
	};
	enum file_life_enum
	{
		__file_unknown,
		__file_creating,
		__file_created,
		__file_invalid,
	};
	enum file_used_enum
	{
		__file_idle,
		__file_busy,
	};
	enum sender_step_enum
	{
		__step_prepare,
		__step_in_cache,
		__step_before_write,
		__step_write,
		__step_read,
	};
	enum response_result_enum
	{
		__resp_unfinished,
		__resp_finished_and_keepalive,
		__resp_finished_and_close,
		__resp_later,
		__resp_read_new,
		//__resp_finished_and_keepalive_and_nodisp=1<<16|__resp_finished_and_keepalive,
		//__resp_finished_and_close_and_nodisp=1<<16|__resp_finished_and_close,
	};
	enum communcation_state_enum
	{
		__state_null,
		__state_running,
		__state_stop_starting,
	};
	static size_t const __send_receive_buffer_size_default = 1024;
};


___namespace2_end()