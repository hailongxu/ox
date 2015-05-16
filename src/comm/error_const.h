

#include "../ox/nsab.h"
#include <WinError.h>


#pragma once


___namespace2_begin(ox,comm)


struct error_const
{
	enum error_selfdefine_enum
	{
		__error_null = 0,
		__error_socket_changes_unknownlly = 0x20000001,
		__error_socket_read_nothing,
		__error_start_to_exit_normally,
		__error_protocol_afint_not_supported,
		__error_map_insert_error,
		__error_http_head_format_invalid,
		__error_http_body_format_invalid,
		__error_http_abnormal_data,
		__error_data_sent_is_zero,
		__error_connection_prepared_error,
		__error_invalid_file_number,
		__error_write_length_longer_than_raw,
		__error_pointer_is_null,
		__error_invalid_parameter,
		__error_no_enough_memory,
		__error_canceled_by_user,
		__error_channelid_not_found,
		__error_invalid_node_name,
		__error_invalid_group_name,
		__error_connection_deleted,
		__error_commandid_not_found,
		__error_no_timer,
		__error_timeout,
		__error_ipcnode_format_error,
		__error_ipcnode_list_is_null,
		__error_pool_allocated_out,
		__error_start_not_allowed_at_running,
		__error_has_been_started,
		__error_name_has_been_used,
		__error_file_is_not_that_long,
		__error_proxy_socks_error_general_failure,
		__error_proxy_socks_error_authentication_error,
		__error_proxy_socks_error_no_ident,
		__error_proxy_socks_error_identd_error,
		__error_proxy_socks_error_unsupported_version,
		__error_proxy_socks_error_command_not_supported,
		__error_proxy_socks_error_username_required,
		__error_proxy_socks_error_unsupported_authentication_method,
		__error_proxy_socks_error_unsupported_authentication_version,
		__error_proxy_socks_error_invalid_destination_host,

		__error_system_socks_error_wsaeacces = WSAEACCES,
		__error_system_socks_error_wsaepfnosupport = WSAEPFNOSUPPORT,
		__error_system_socks_error_wsaenetunreach = WSAENETUNREACH,
		__error_system_socks_error_wsaetimedout = WSAETIMEDOUT,
		__error_system_socks_error_wsaeconnrefused = WSAECONNREFUSED,
		__error_system_socks_error_wsaehostunreach = WSAEHOSTUNREACH,
		__error_system_file_not_found = ERROR_FILE_NOT_FOUND,
	};

};


___namespace2_end()