


#ifdef _DEBUG
#include "../ox/io/file.h"
#endif
#include "delegate.h"
#include "client_helper.h"
#include "ipc_define.h"
#include "ipc_helper_internal.h"



#pragma once




namespace abc
{
	struct ipc_client_helper_internal : ipc_define
	{
		typedef client_helper helper;
		typedef communication_const constant;
		typedef communication_define::data_t data_t;
		typedef communication_define::str_t str_t;
		typedef communication_define::response_struct data_response_struct;
		typedef constant::response_result_enum response_result_enum;
		typedef communication_define::post_task_d post_task_d;

		typedef ipc_helper_internal::ipc_protocol_header ipc_protocol_header;
		typedef ipc_helper_internal::readwrite_response_handle readwrite_response_handle;

		struct sender_struct : client_helper::sender_struct
		{
			typedef client_helper::sender_struct tcp_struct;
			typedef readwrite_response_handle extra_t;
			sender_struct()
			{
				tcp_struct::protocol = constant::__protocol_ipc;
				tcp_struct::extra_construct.assign(this,&sender_struct::do_extra_construct);
				tcp_struct::extra_destruct.assign(&do_extra_destruct);
				tcp_struct::to_binary.assign(this,&sender_struct::do_to_binary);
				tcp_struct::extra_size = sizeof(extra_t);
				tcp_struct::extra_param = 0;
				tcp_struct::is_data_null.assign(&do_is_data_null);
				tcp_struct::completed.assign(this,&sender_struct::on_ipc_message_error);
			}
			virtual void copy_construct(client_helper::sender_struct& ss) const
			{
				sender_struct& obj = * (new (&ss) sender_struct);
				/// set like the member function set
				obj._m_ipc_sender = _m_ipc_sender;
				obj.tcp_struct::buffer_size_for_sendrecving = buffer_size_for_sendrecving;
				obj.tcp_struct::data = data;
				obj.tcp_struct::outofband = outofband;
			}
			void do_to_binary(std::string& out)
			{
				out.clear();
				encode(out,data.begin,data.size);
			}
			static bool do_is_data_null()
			{
				return false;
			}
			void do_extra_construct(char* pobj,char* param)
			{
				extra_t* resp = new (pobj) extra_t;
				resp->on_ipc_feedback = _m_ipc_sender.on_ipc_feedback;
				//resp->_m_ipc_response_struct.post_task = _m_ipc_sender._m_post_task;
				client_helper::sender_struct::completed.assign(resp,&extra_t::on_ipc_message);
			}
			static void do_extra_destruct(char* buf)
			{
				((extra_t*)(buf))->~extra_t();
			}
			void set(ipc_user_sender_struct const& user_param)
			{
				_m_ipc_sender = user_param;
				tcp_struct::buffer_size_for_sendrecving = user_param._m_recv_buffer_size;
				tcp_struct::data = user_param._m_data;
				tcp_struct::outofband = user_param._m_binddata;
				//tcp_struct::outofband2 = http_user_param._m_binddata2;
				//tcp_struct::file_number = http_user_param._m_file_number;
			}

			ipc_sender_struct _m_ipc_sender;
		private:
			void encode(std::string& out,char const* data,size_t size)
			{
				ipc_protocol_header header;
				header.size = size+8;
				header.ver = 1;
				out.append((char const*)&header,sizeof(header));
				out.append(data,size);
			}
			response_result_enum on_ipc_message_error(data_response_struct eventdata)
			{
				assert(eventdata.step==constant::__step_prepare);
				ipc_define::response_struct ipc_resp(&eventdata);
				ipc_resp.user_data = 0;
				ipc_resp.user_data_size = 0;
				//ipc_resp.on_http_outer_feedback = &_m_http_sender.on_http_outer_feedback;
				response_result_enum ret = _m_ipc_sender.on_ipc_feedback(ipc_resp);
				return ret;
			}
		};
	}; /// end of class ipc helper internal
}
