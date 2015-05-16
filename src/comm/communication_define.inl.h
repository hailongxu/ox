
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */


//#include <string.h>
//#include <string>
//#include <assert.h>
//#include "../ox/utl/data_t.h"
//#include "communication_const.h"
//#include "delegate.h"
//#include "task_decl.h"
#include "../ox/nsab.h"
#include "client_helper.h"
#include "communication_define.h"



#pragma once



___namespace2_begin(ox,comm)


inline communication_define::response_object::response_object(void const* _obj) : _m_object(_obj)
{
}
inline size_t communication_define::response_object::seq() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	return iodata->_m_seq;
}
inline communication_const::sender_step_enum communication_define::response_object::step() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	return iodata->_m_step;
}
inline int communication_define::response_object::errorid() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	return iodata->_m_errorid;
}
inline char const* communication_define::response_object::name() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	return iodata->_m_name.c_str();
}
inline size_t communication_define::response_object::file_number() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	return iodata->_m_file_number;
}
inline void const* communication_define::response_object::outofbind() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	return iodata->_m_outofband;
}
inline ox::utl::cdata_t communication_define::response_object::data() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	cdata_t cdata;
	cdata.begin = iodata->_m_buffer.data();
	cdata.size = iodata->_m_bytes_transfered;
	return cdata;
}
inline void communication_define::response_object::write_back() const
{
	assert (false);
	return;
	//client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	//iodata->_m_fileinfo?
	//	&iodata->_m_fileinfo->group->_m_send_back : 0;
	//send_back_d const& send_back = *iodata->_m_fileinfo->group->_m_send_back;
	//return send_back()
}
inline size_t communication_define::response_object::extra_size() const
{
	client_helper::rw_iodata_t const* iodata = static_cast<client_helper::rw_iodata_t const*>(_m_object);
	return iodata->_m_extra_size;
}
//inline 


___namespace2_end()