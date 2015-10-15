

#include "../utl/data_t.h"



#pragma once



___namespace2_begin(ox,ipc)

	struct buffer_interface
	{
		typedef ox::utl::data_t data_t;
		virtual	buffer_interface const* append(/*data_t data*/) const = 0;
		virtual	size_t size() const = 0;
	};

	struct vector_interface
	{
		typedef ox::utl::data_t data_t;
		virtual	size_t get_size() const = 0;
		virtual	data_t get_item() const = 0;
	};

	struct ipc_protocol_interface
	{
		typedef ox::utl::data_t data_t;
		virtual	size_t get_size() const = 0;
		virtual	size_t get_ver() const = 0;
		virtual	size_t get_seq() const = 0;
		virtual	size_t get_errorid() const = 0;
		virtual	data_t get_source_address() const = 0;
		virtual	data_t get_response_address() const = 0;
		virtual	vector_interface const* get_destination_addresses() const = 0;
		virtual	size_t get_commandid() const = 0;
		virtual	vector_interface const* get_data() const = 0;
	};


___namespace2_end()