
#include "../utl/data_t.h"
#include "../cxx/delegate.h"
#include "ipcnode_define.h"

#pragma once


namespace ox
{
	namespace ipc
	{
		using namespace ox::comm;
		/// public data type
		typedef ox::utl::cdata_t cdata_t;
		typedef ox::utl::data_t data_t;
		typedef communication_public::post_task_d post_task_d;
		typedef communication_const constant;
		typedef communication_public::logf_d logf_d;

		/// p2p mode
		typedef ipcnode_define::channelid channelid;
		typedef ipcnode_define::header header;
		typedef ipcnode_define::response_d response_d;
		typedef ipcnode_define::response_struct response_struct;
		typedef ipcnode_define::bind_struct bind_struct;
		/// cast mode
		typedef ipcnode_define::group_header group_header;
		typedef ipcnode_define::group_response_d group_response_d;
		typedef ipcnode_define::group_response_struct group_response_struct;
		/// service mode
		typedef ipcnode_define::on_started_d on_started_d;
		/// service handler
		typedef ipcnode_define::ipc_listener_d ipc_listener_d;
		typedef ipcnode_define::ipc_service_d ipc_service_d;

		/// data interface
		struct data_interface
		{
			virtual cdata_t data() const = 0;
			virtual void append(cdata_t const& data) = 0;
			virtual void append(char const* data,size_t size) = 0;
			/// the same as the string::clear()
			virtual void clear() = 0;
		};

		/// the ipc node interface
		struct ipcnode_interface
		{
			virtual void set_post(size_t id,post_task_d const& post) = 0;
			virtual void set_on_started(on_started_d const& _on_started) = 0;
			/// node name must be started with "t1"
			virtual size_t start(/*char const* node_name*/) = 0;
			/// join the group
			/// group name must be started with "t2"
			virtual void enter_group(char const* group_name) = 0;
			virtual void leave_group(char const* group_name) = 0;
			virtual void get_list(data_t list,char const* name) = 0;
			virtual void stop() = 0;
			virtual channelid create_channel(
				char const* destid, /// the node id, not group id
				bool bself_included = false, /// 是否包含自己
				bool bsession_ignored = false /// 是否穿透不同的session
				) = 0;
			virtual void close_channel(channelid const& channel) = 0;
			/// sequence sending
			virtual size_t post_channel(
				channelid const& channel, /// channel id
				size_t cmdid, /// 命令id
				cdata_t data, /// 发送的数据
				response_struct const& response,
				size_t timeout_ms,
				size_t* reqid = 0 /// 返回本次请求id，如果需要传入
				) = 0;
			/// not sequence
			virtual size_t post(
				char const* destid, /// node id, or group id
				size_t cmdid, /// 命令id
				cdata_t data, /// 发送的数据
				group_response_struct const& response,
				size_t timeout_ms,
				bool self_included = false, /// 是否包含自己
				bool bsession_ignored = false, /// 是否穿透不同的session
				size_t* reqid = 0 /// 返回本次请求id，如果需要传入
				) = 0;
			/// synchronized
			virtual size_t send_channel(
				channelid const& channel, /// channel id
				size_t cmdid, /// 命令id
				cdata_t data, /// 发送的数据
				response_struct const& response,
				size_t timeout_ms ///
				) = 0;
			/// cast, not sequence synchronized
			virtual size_t send(
				char const* destid, /// node id, or group id
				size_t cmdid, /// 命令id
				cdata_t data, /// 发送的数据
				group_response_struct const& response,
				size_t timeout_ms,
				bool self_included = false, /// 是否包含自己
				bool bsession_ignored = false /// 是否穿透不同的session
				) = 0;
			/// the ipc service
			virtual void add_listener(size_t cmdid,ipc_listener_d const& listener,bind_struct const& bind) = 0;
			virtual void add_service(size_t cmdid,ipc_service_d const& listener,bind_struct const& bind) = 0;
			virtual void remove_listener(size_t cmdid,ipc_listener_d const& listener) = 0;
			virtual void remove_service(size_t cmdid) = 0;
		}; /// end of ipcnode interface

#if 0
		ipcnode_interface* create_ipcnode();
		void destroy_ipcnode(ipcnode_interface*);
		void destroy_services();
		void set_logf(logf_d const&);
#endif
	} /// end of ipc namespace
}