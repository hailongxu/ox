


#include "../utl/vecbuff.h"
#include "../cxx/delegate.h"
#include "../mos/timer.h"
//#include "socket_define.h"
#include "../comm/communication_const.h"
#include "ipc_interface_define.h"
#include "ipc_protocol.h"
#include "ipcnode_define.h"



#pragma once




___namespace2_begin(ox,ipc)


	//struct ipc_helper_internal::readwrite_response_handle;
	struct ipc_define
	{
		struct response_struct;
		typedef ox::utl::cdata_t cdata_t;
		typedef ox::mos::win_timer_list::timer_position timer_position;
		typedef ox::comm::communication_const constant;
		typedef ipc_sender_back ipc_sender_back;
		typedef ipc_protocol::ipc_protocol_buff ipc_protocol_buff;
		typedef delegate<constant::response_result_enum(response_struct&,ipc_sender_back const&)> ipc_event_d;
		typedef delegate<std::string(cdata_t const&)> to_ipc_pipe_name_d;

		struct timer_struct
		{
			timer_struct()
			{
				_m_flag = 0;
				_m_period = -1;
				_m_commandid = -1;
			}
			~timer_struct()
			{
				char const* p = _m_source.begin;
				_m_source.clear();
				_m_binddata.clear();
				delete [] p;
			}
			ox::mos::atomic_long _m_flag; /// the count of access
			timer_position _m_pos;
			size_t _m_period;
			size_t _m_commandid;
			cdata_t _m_source;
			cdata_t _m_binddata;

			void set_source_and_binddata(cdata_t const& source,cdata_t const& binddata)
			{
				char* p = new (std::nothrow) char [source.size+1+binddata.size];
				assert (p);
				if (!p) return;
				memmove(p,source.begin,source.size);
				memmove(p+source.size,binddata.begin,binddata.size);
				p[source.size] = 0;
				_m_source.begin = p;
				_m_source.size = source.size;
				_m_binddata.begin = p+_m_source.size+1;
				_m_binddata.size = binddata.size;
			}

			bool is_definitely_timeout() const
			{
				return _m_flag.value() >= 3;
			}
			bool is_timeout_before_finished() const
			{
				assert (_m_flag.value() <= 2);
				return _m_flag.value() >= 2;
			}
			bool is_started() const
			{
				return _m_flag.value() > 0 ;
			}
			bool set_started_return_is_null()
			{
				long old = _m_flag++;
				return old==0;
			}
			bool set_timeout_return_is_started()
			{
				long old = _m_flag++;
				return old==1;
			}
			bool set_finished_return_is_started()
			{
				long old = _m_flag++;
				return old==1;
			}
		};

		struct response_struct
		{
			response_struct(ox::comm::communication_define::response_struct const* _base=0)
				: base(_base)
				, protocol(0)
			{}
			ox::comm::communication_define::response_struct const* base;
			ipc_protocol_buff* protocol;
			cdata_t user_data() {return protocol->get_data();};
			cdata_t bind_data() const;
			ipcnode_define::response_struct _m_user_response_struct; /// user event
			timer_struct timeout;
		};

		struct ipc_sender_struct
		{
			ipc_sender_struct() : _m_cmdid(0),_m_timeout_ms(-1),_m_binddata_begin(0),_m_binddata_size(0) {}
			cmdid_helper cmdid() {return cmdid_helper(_m_cmdid);}
			size_t _m_cmdid;
			cdata_t _m_faddr;
			cdata_t _m_daddrs; // ox::utl::vecbuff_t const*
			ipc_event_d on_ipc_feedback;
			ipcnode_define::response_struct _m_user_response_struct;
			size_t _m_timeout_ms;
			cdata_t* _m_binddata_begin;
			size_t _m_binddata_size;
		};

		struct ipc_user_sender_struct : ipc_sender_struct
		{
			ipc_user_sender_struct()
				: _m_recv_buffer_size(1024)
				, _m_bind(0)
			{}
			cdata_t& data() {return _m_data;}
			size_t _m_recv_buffer_size;
			ox::utl::cdata_t _m_data;
			void const* _m_bind;
		};
	}; /// end of class ipc helper


___namespace2_end()