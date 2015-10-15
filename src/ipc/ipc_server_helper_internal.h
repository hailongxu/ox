


#ifdef _DEBUG
#include "../ox/io/file.h"
#endif
#include "delegate.h"
#include "communication_define.h"
#include "ipc_define.h"
#include "ipc_helper_internal.h"
#include "client_helper.h"



#pragma once




namespace abc
{
	struct ipc_server_helper_internal : ipc_define
	{
		typedef communication_const constant;
		typedef communication_define::data_t data_t;
		typedef communication_define::str_t str_t;
		typedef communication_define::response_struct base_response_struct;
		typedef constant::response_result_enum response_result_enum;
		//typedef http_define::buffer_append_d buffer_append_d;
		typedef communication_define::post_task_d post_task_d;
		//typedef client_helper::response_d response_d;
		//typedef client_helper::receiver_struct receiver_struct;
		//typedef client_helper::sender_struct sender_struct;

		typedef ipc_helper_internal::ipc_protocol_header ipc_protocol_header;
		typedef ipc_helper_internal::ipc_sender_struct ipc_sender_struct;
		typedef ipc_helper_internal::ipc_user_sender_struct ipc_user_sender_struct;
		typedef ipc_helper_internal::ipc_event_d ipc_event_d;



		struct sender_struct : client_helper::sender_struct
		{
			typedef sender_struct self;
			typedef client_helper::sender_struct base_struct;
			typedef readwrite_response_handle extra_t;
			self()
			{
				base_struct::protocol = constant::__protocol_ipc;
				base_struct::extra_construct.assign(this,&self::do_extra_construct);
				base_struct::extra_destruct.assign(&do_extra_destruct);
				base_struct::to_binary.assign(this,&self::do_to_binary);
				base_struct::extra_size = sizeof(extra_t);
				base_struct::extra_param = 0;
				base_struct::is_data_null.assign(&do_is_data_null);
				base_struct::completed.assign(this,&self::on_message_error);
			}
			ipc_event_d& on_ipc_feedback()
			{
				return _m_ipc_sender.on_ipc_feedback;
			}

			virtual void copy_construct(client_helper::sender_struct& ss) const
			{
				self& obj = * (new (&ss) self);
				/// set like the member function set
				obj._m_ipc_sender = _m_ipc_sender;
				obj.base_struct::buffer_size_for_sendrecving = buffer_size_for_sendrecving;
				obj.base_struct::data = data;
				obj.base_struct::outofband = outofband;
				obj.buffer_size_for_sendrecving = buffer_size_for_sendrecving;
				obj.file_number = file_number;
				obj.protocol = protocol;
				obj.completed = completed;
				obj.data = data;
				obj._m_flag = _m_flag;
				memmove(obj._m_name,_m_name,sizeof(_m_name));
			}
			void do_to_binary(std::string& out)
			{
				out.clear();
				ipc_protocol_header header;
				header.size = data.size+ipc_protocol_header::head_size();
				header.ver = 1;
				out.append((char const*)&header,sizeof(header));
				out.append(data.begin,data.size);
			}
			static bool do_is_data_null()
			{
				return false;
			}
			void do_extra_construct(char* pobj,char* param)
			{
				extra_t* resp = new (pobj) extra_t;
				resp->on_ipc_feedback = _m_ipc_sender.on_ipc_feedback;
				client_helper::sender_struct::completed.assign(resp,&extra_t::on_ipc_message);
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
				base_struct::outofband = ipc_user_param._m_binddata;
			}

			ipc_sender_struct _m_ipc_sender;
		private:
			response_result_enum on_message_error(base_response_struct eventdata)
			{
				assert(eventdata.step==constant::__step_prepare);
				ipc_define::response_struct ipc_resp(&eventdata);
				ipc_resp.user_data = 0;
				ipc_resp.user_data_size = 0;
				response_result_enum ret = _m_ipc_sender.on_ipc_feedback(ipc_resp);
				return ret;
			}
		};

	}; /// end of class ipc server helper internal
}
