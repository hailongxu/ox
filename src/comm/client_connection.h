

#include <string>
#include "communication_define.h"
#include "client_helper.h"



#pragma once


___namespace2_begin(ox,comm)


template <typename communication>
struct client_connection
{
	typedef communication_define::reqid_t reqid_t;
	typedef client_helper::sender_struct sender_struct;

	client_connection(size_t errorid = 0)
		: _m_communication(0)
		, _m_errorid(errorid)
		, _m_file_number(0)
	{}
	client_connection(std::string const& name, size_t file_number, communication* comm)
		: _m_communication(comm)
		, _m_name(name)
		, _m_file_number(file_number)
		, _m_errorid(0)
	{}
	bool is_null() const
	{
		return _m_communication == 0;
	}
	reqid_t send(sender_struct& sdata)
	{
		assert(_m_communication);
		_m_name = sdata._m_name;
		sdata.file_number = _m_file_number;
		return _m_communication->send(sdata);
	}
	void close()
	{
		assert(_m_communication);
		_m_communication->close(_m_name, _m_file_number);
	}
private:
	std::string _m_name;
	size_t _m_file_number;
	communication* _m_communication;
	size_t _m_errorid;
};


___namespace2_end()