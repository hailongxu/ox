


#ifdef _DEBUG
#include "../io/file.h"
#endif
#include <string>
#include "../pkg/string_simple_kit.h"
#include "../utl/vecbuff.h"
#include "../mos/scope_raii.h"
#include "../cxx/delegate.h"
#include "../comm/communication_define.h"
#include "../comm/client_helper.h" /// for temporary using
#include "ipc_define.h"
#include "ipc_sender_back.h"
#include "ipc_protocol.h"



#pragma once




___namespace2_begin(ox,ipc)


using namespace ox::comm;

struct ipc_helper_internal
{
	static ox::character::is_case_sensitive_c const casive = ox::character::__case_sensitive;
	static ox::character::is_case_sensitive_c const incasive = ox::character::__case_insensitive;
	typedef ox::str::astring_kit strkit;
	typedef ox::utl::vecbuff_t vecbuff_t;
	typedef ox::utl::vecbuff_helper vbuff_helper;

	typedef ox::comm::communication_const constant;
	typedef ox::comm::communication_define::cdata_t cdata_t;
	typedef ox::comm::communication_define::cstr_t cstr_t;
	typedef ox::comm::communication_define::response_struct base_response_struct;
	typedef constant::response_result_enum response_result_enum;
	typedef ox::comm::communication_define::send_d send_d;
	typedef ipc_define::ipc_event_d ipc_event_d;
	typedef ipc_protocol::ipc_protocol_header ipc_protocol_header;
	typedef ipc_protocol::ipc_protocol_buff ipc_protocol_buff;
	
	struct readwrite_response_handle
	{
		readwrite_response_handle()
			: _m_presize_reserved(0)
		{}
		response_result_enum on_base_message(base_response_struct basedata)
		{
			size_t size_read = basedata.size;
			size_t error_code = basedata.errorid;
			_m_ipc_response_struct.base = &basedata;
			if (basedata.step!=constant::__step_read)
			{
				ipc_sender_back send_back;
				ipc_protocol_buff* protocol = (ipc_protocol_buff*)basedata.data;
				_m_ipc_response_struct.protocol = protocol;
				response_result_enum ret = on_ipc_feedback(_m_ipc_response_struct,send_back);
				return ret;
			}

			buffer_append(basedata.data,basedata.size);
			//_m_received.append(basedata.data,basedata.size);

			label_reparse_again:
			if (buffer_size()<ipc_protocol_header::head_size())
				return constant::__resp_unfinished;
			ipc_protocol_buff* ipc_content = (ipc_protocol_buff*)buffer_begin();
			if (buffer_size()>=ipc_content->size())
			{
				ipc_sender_back send_back;
				send_back.set(basedata.write_back,&_m_ipc_response_struct);
				_m_ipc_response_struct.protocol = ipc_content;
				response_result_enum ret = on_ipc_feedback(_m_ipc_response_struct,send_back);
				if (!basedata.is_message_more())
				{
					assert (buffer_size()==ipc_content->size());
					buffer_clear();
					return ret;
				}
				buffer_remove_front(ipc_content->size());
				if (ret!=constant::__resp_finished_and_close)
					if (buffer_size()>=ipc_protocol_header::head_size())
						goto label_reparse_again;
				return ret;
			}

			return constant::__resp_unfinished;
		}

		void buffer_append(char const* begin,size_t size)
		{
			assert(_m_received.size()>=_m_presize_reserved);
			_m_received.append(begin,size);
		}
		char const* buffer_begin() const
		{
			assert(_m_received.size()>=_m_presize_reserved);
			return _m_received.data()+_m_presize_reserved;
		}
		size_t buffer_size() const
		{
			assert(_m_received.size()>=_m_presize_reserved);
			return _m_received.size()-_m_presize_reserved;
		}
		void buffer_remove_front(size_t size)
		{
			assert(_m_received.size()>=_m_presize_reserved);
			_m_received.erase(_m_presize_reserved,size);
		}
		void buffer_clear()
		{
			assert(_m_received.size()>=_m_presize_reserved);
			_m_received.erase(_m_presize_reserved);
		}

		ipc_define::response_struct _m_ipc_response_struct;
		ipc_event_d on_ipc_feedback;
		std::string _m_received;
		size_t _m_presize_reserved;
	};

	struct sender_struct : ox::comm::communication_define::sender_struct
	{
		typedef sender_struct self;
		typedef ox::comm::communication_define::sender_struct base_struct;
		typedef readwrite_response_handle extra_t;
		typedef ipc_protocol::ipc_protocol_buff_helper ipc_protocol_buff_helper;
		typedef ipc_define::ipc_user_sender_struct ipc_user_sender_struct;

		self(size_t bind_size) : _m_presize_reserved(0)
		{
			base_struct::protocol = constant::__protocol_ipc;
			base_struct::extra_construct.assign(this,&self::do_extra_construct);
			base_struct::extra_destruct.assign(&do_extra_destruct);
			base_struct::to_binary.assign(this,&self::do_to_binary);
			base_struct::extra_size = sizeof(extra_t)+bind_size;
			base_struct::extra_param = 0;
			base_struct::is_data_null.assign(&do_is_data_null);
			base_struct::completed.assign(this,&self::on_message_error);
		}
		ipc_event_d& on_ipc_feedback()
		{
			return _m_ipc_sender.on_ipc_feedback;
		}

		virtual void copy_construct(ox::comm::communication_define::sender_struct& ss) const
		{
			self& obj = * (new (&ss) self (base_struct::extra_size-sizeof(extra_t)));
			/// set like the member function set
			obj.base_struct::buffer_size_for_sendrecving = buffer_size_for_sendrecving;
			obj.base_struct::data = data;
			obj.base_struct::outofband = outofband;
			obj._m_ipc_sender = _m_ipc_sender;
			obj.buffer_size_for_sendrecving = buffer_size_for_sendrecving;
			obj.file_number = file_number;
			obj.protocol = protocol;
			obj.completed = completed;
			obj.data = base_struct::data;
			obj._m_flag = _m_flag;
			memmove(obj._m_name,_m_name,sizeof(_m_name));
			obj._m_presize_reserved = _m_presize_reserved;
		}
		void do_to_binary(std::string& out)
		{
			ipc_protocol_buff_helper(out).init().add(
				_m_saddr,
				_m_ipc_sender._m_faddr,
				_m_ipc_sender._m_daddrs,
				_m_ipc_sender._m_cmdid,
				base_struct::data);
		}
		static bool do_is_data_null()
		{
			return false;
		}
		void do_extra_construct(char* pobj,size_t size,char* param)
		{
			assert (size >= sizeof(extra_t));
			extra_t* resp = new (pobj) extra_t;
			resp->on_ipc_feedback = _m_ipc_sender.on_ipc_feedback;
			communication_define::sender_struct::completed.assign(resp,&extra_t::on_base_message);
			/// user cares
			resp->_m_ipc_response_struct._m_user_response_struct = _m_ipc_sender._m_user_response_struct;
			resp->_m_ipc_response_struct.timeout._m_period = _m_ipc_sender._m_timeout_ms;
			/// bind buffer
			if (_m_ipc_sender._m_binddata_begin && _m_ipc_sender._m_binddata_size)
			{
				cdata_t binddata ((char*)(resp+1),size-sizeof(extra_t));
				size_t binddata_size = vecbuff_t::size_needed_of_index_and_data(_m_ipc_sender._m_binddata_begin,_m_ipc_sender._m_binddata_size);
				binddata_size += vecbuff_t::size_of_meta();
				assert ((int)binddata.size>0 && binddata.size>=binddata_size);
				vecbuff_t* vecbind = vecbuff_t::unsafe_as_vecbuff(binddata);
				vecbind->init_rel();
				vecbind->unsafe_add_index_and_data(_m_ipc_sender._m_binddata_begin,_m_ipc_sender._m_binddata_size);
			}
			/// presize, we can move into a receiver struct, but I am still not  firm the conception
			resp->_m_presize_reserved = _m_presize_reserved;
			resp->_m_received.resize(_m_presize_reserved,0);
		}
		static void do_extra_destruct(char* buf)
		{
			((extra_t*)(buf))->~extra_t();
		}
		void set(ipc_user_sender_struct const& ipc_user_param)
		{
			_m_ipc_sender = ipc_user_param;
			base_struct::buffer_size_for_sendrecving = ipc_user_param._m_recv_buffer_size;
			base_struct::data = ipc_user_param._m_data;
			base_struct::outofband = ipc_user_param._m_bind;
		}

		cdata_t _m_saddr;
		size_t _m_presize_reserved;
		ipc_define::ipc_sender_struct _m_ipc_sender;
	private:
		response_result_enum on_message_error(base_response_struct eventdata)
		{
			assert(eventdata.step==constant::__step_prepare);
			ipc_define::response_struct ipc_resp(&eventdata);
			response_result_enum ret = _m_ipc_sender.on_ipc_feedback(ipc_resp,ipc_sender_back());
			return ret;
		}
	};


	/// temporary using
	static ipc_protocol_buff const* get_ipc_protocol(void const* internal_object_handle)
	{
		client_helper::rw_iodata_t const* iodata = (client_helper::rw_iodata_t const*)internal_object_handle;
		readwrite_response_handle const* handle = (readwrite_response_handle const*)(iodata->extra_data());
		ipc_protocol_buff const* ipc_protocol = (ipc_protocol_buff*)(handle->_m_received.data());
		return ipc_protocol;
	}
	/// temporary using
	static ipcnode_define::response_struct const* get_ipcnode_response(void const* internal_object_handle)
	{
		client_helper::rw_iodata_t const* iodata = (client_helper::rw_iodata_t const*)internal_object_handle;
		readwrite_response_handle const* handle = (readwrite_response_handle const*)iodata->extra_data();
		ipcnode_define::response_struct const* ipc_response = (ipcnode_define::response_struct*)(&handle->_m_ipc_response_struct._m_user_response_struct);
		return ipc_response;
	}
	/// temporary using
	static ipc_define::timer_struct const* get_ipc_timeout(void const* internal_object_handle)
	{
		client_helper::rw_iodata_t const* iodata = (client_helper::rw_iodata_t const*)internal_object_handle;
		readwrite_response_handle const* handle = (readwrite_response_handle const*)iodata->extra_data();
		ipc_define::timer_struct const* ipc_timeout = &handle->_m_ipc_response_struct.timeout;
		return ipc_timeout;
	}
	/// temporary using
	static ipc_define::response_struct const* get_ipc_response(void const* internal_object_handle)
	{
		client_helper::rw_iodata_t const* iodata = (client_helper::rw_iodata_t const*)internal_object_handle;
		readwrite_response_handle const* handle = (readwrite_response_handle const*)iodata->extra_data();
		ipc_define::response_struct const* ipc_response = &handle->_m_ipc_response_struct;
		return ipc_response;
	}
	/// temporary using
	static size_t get_ipc_response_presize_reserved(void const* internal_object_handle)
	{
		client_helper::rw_iodata_t const* iodata = (client_helper::rw_iodata_t const*)internal_object_handle;
		readwrite_response_handle const* handle = (readwrite_response_handle const*)iodata->extra_data();
		return handle->_m_presize_reserved;
	}
	/// temporary using
	static cdata_t get_ipc_binddata(void const* internal_object_handle)
	{
		cdata_t data;
		client_helper::rw_iodata_t const* iodata = (client_helper::rw_iodata_t const*)internal_object_handle;
		readwrite_response_handle const* handle = (readwrite_response_handle const*)iodata->extra_data();
		data.begin = (char*)(handle+1);
		data.size = iodata->_m_extra_size-sizeof(readwrite_response_handle);
		return data;
	}

}; /// end of class ipc helper internal


inline ox::utl::cdata_t ipc_define::response_struct::bind_data() const
{
	typedef ipc_helper_internal::readwrite_response_handle response_handle;
	cdata_t data;
	data.begin = (char*)((response_handle*)(this)+1);
	data.size = this->base->response.extra_size()-sizeof(response_handle);
	return data;
}


___namespace2_end()