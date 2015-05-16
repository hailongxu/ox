
#include "../ox/nsab.h"
#include "../mos/scope_raii.h"
#include "file_service_operation.h"
#include "pointer_check.h"
#include "communication_const.h"



#pragma once




___namespace2_begin(ox,comm)


struct iodata_iocp_check_t
{
	typedef pointer_check<void> iodata_check_t;
	iodata_check_t _m_iodata_check;
	ox::mos::critical_section _m_cs;
	
	void add(void* iodata)
	{
		_m_iodata_check.uncheck_add(iodata);
	}
	bool safe_has_and_remove(void* iodata)
	{
		ox::mos::scope_cs lock(_m_cs);
		return _m_iodata_check.has_and_remove(iodata);
	}
	static iodata_iocp_check_t& instance()
	{
		static iodata_iocp_check_t _static_iodata_check;
		return _static_iodata_check;
	}
};

template <typename iodata_tn,typename rw_iodata_tn>
struct sender_iodata_tt
{
	typedef iodata_tn iodata_t;
	typedef rw_iodata_tn rw_iodata_t;
	typedef file_handle_operation::file_handle_t file_handle_t;
	typedef communication_const constant;
	typedef ox::utl::cdata_t cdata_t;


	sender_iodata_tt(file_handle_operation& handle_operation)
		: operation(handle_operation)
	{}
	file_handle_operation& operation;
	int operator()(file_handle_t file_handle,iodata_t* _iodata)
	{
		int ret = -1;
		if (_iodata->actionid()==constant::__file_write)
		{
			rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
			assert(iodata->_m_bytes_willbe_transfered >= iodata->_m_bytes_hasbeen_transfered);
			size_t bytes_left = iodata->_m_bytes_willbe_transfered - iodata->_m_bytes_hasbeen_transfered;
			if (int(bytes_left)<0)
			{
				assert (false && "size written is longer than to be written");
				ret = constant::__error_write_length_longer_than_raw;
				return ret;
			}
			//cdata_t data (iodata->_m_buffer.data(),iodata->_m_buffer.size());
			cdata_t data;
			data.begin = iodata->_m_buffer.data()+iodata->_m_bytes_hasbeen_transfered;
			data.size = iodata->_m_buffersize_for_sendrecving;
			if (data.size>bytes_left) data.size = bytes_left;
			char* wsabuf = iodata->_m_wsabuf;
			OVERLAPPED& ol = iodata->overlapped();
			ox::mos::scope_cs lock(iodata_iocp_check_t::instance()._m_cs);
			ret = operation.asend(file_handle,data,wsabuf,&ol);
			iodata->_m_errorid = ret;
			if (ret!=0) operation.close(file_handle),
			iodata->_m_file_handle = operation.invalid_file_handle();
			if (ret==0) iodata_iocp_check_t::instance().add(_iodata);
		}
		else if (_iodata->actionid()==constant::__file_read)
		{
			rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
			char* wsabuf = iodata->_m_wsabuf;
			cdata_t data (iodata->_m_buffer.data(),iodata->_m_buffer.size());
			OVERLAPPED& ol = iodata->overlapped();
			ox::mos::scope_cs lock(iodata_iocp_check_t::instance()._m_cs);
			ret = operation.arecv(file_handle,data,wsabuf,&ol);
			iodata->_m_errorid = ret;
			if (ret!=0) operation.close(file_handle),
			iodata->_m_file_handle = operation.invalid_file_handle();
			if (ret==0) iodata_iocp_check_t::instance().add(_iodata);
		}
		//else if (_iodata->actionid()==constant::__file_connect)
		//{
		//	assert(false);
		//	conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);
		//	//locationid const& lid = iodata->_m_lid;
		//	OVERLAPPED& ol = iodata->overlapped();
		//	//ret = file_handle_operation::acconect(file_handle,lid,&ol);
		//}
		else
		{
			assert("invalid action type");
		}
		return ret;
	}
};


___namespace2_end()