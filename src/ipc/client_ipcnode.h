

#include "../ox/utl/data_t.h"
#include "client_ipc.h"
#include "ipc_define.h"
#include "ipc_helper_internal.h"
#include "ipcnode_define.h"

#pragma once


namespace abc
{
	struct client_ipcnode
	{
		typedef client_ipcnode self;
		typedef client_ipc::base::connection connection;
		typedef ox::utl::data_t data_t;
		typedef communication_define::post_task_d post_task_d;
		typedef ipc_define::ipc_user_sender_struct ipc_user_sender_struct;
		typedef ipc_define::ipc_event_d ipc_event_d;
		typedef client_ipc client_ipc;
		typedef communication_const constant;

		typedef ipcnode_define::callback_struct callback_struct;
		typedef ipcnode_define::header header;
		typedef ipcnode_define::ret_t ret_t;
		typedef ipcnode_define::stop_event_d stop_event_d;


		size_t init()
		{
	
		}
		size_t unint()
		{
			//
		}
		size_t stop(stop_event_d const& on_stop,void* param)
		{
			return 0;
		}

		size_t set_post(post_task_d const& post)
		{
			_m_post_task = post;
		}
		size_t set_group(char const* group_name)
		{
			assert (_m_client_ipc);
			/// prepare send connection
			_m_client_ipc->create_package("ipc_send");
			std::string conn_name = "ipc_send/";
			_m_send_connection = _m_client_ipc->create_connection(conn_name.append(group_name));
			post(_m_send_connection,__cmdid_join_group,data_t(),ipc_event_d(self::ipc_package_handle),0);

			/// prepare recv connection
			_m_client_ipc->create_stream("ipc_recv");
			ipc_user_sender_struct param;

			param.on_ipc_feedback.assign(this,&self::on_stream_received);
			_m_client_ipc->set_recv_to_stream("ipc_recv",0);
			conn_name = "ipc_recv/";
			_m_recv_connection = _m_client_ipc->create_connection(conn_name.append(group_name));
			post(_m_recv_connection,__cmdid_receive_file,data_t(),ipc_event_d(self::on_stream_received),0);
		}
		static ret_t post(
			connection& conn,
			size_t cmdid, /// 命令id
			data_t data, /// 发送的数据
			callback_struct const& callback,
			size_t timeout_ms = 30,
			bool bsession_ignored = false /// 是否穿透不同的session
			)
		{
			assert (conn._m_errorid==0 && conn._m_file_number!=0);
			std::string dest_buf;
			ox::utl::vecbuff_t& db = ox::utl::vecbuff_helper(dest_buf)
				.init().add(_m_group_name.c_str()).as_vecbuff();
			abc::ipc_define::ipc_user_sender_struct iss;
			iss._m_saddr.assign("A");
			iss._m_daddrs.assign(dest_buf.data(),dest_buf.size());
			iss.on_ipc_feedback.assign(self::ipc_package_handle);
			iss.data().assign(data);
			iss._m_post_task.assign(_m_post_task);
			abc::ipc_helper_internal::sender_struct ss2;
			ss2.set(iss);
			//ss2.name().set(_m_send_name.c_str());
			communication_define::reqid_t r = _m_send_connection.send(ss2);
			return ret_t(r.seq,r.errorid);
		}
		//size_t send(
		//	size_t cmdid, /// 命令id
		//	data_t data, /// 发送的数据
		//	data_t data_out,
		//	bool bsession_ignored = false, /// 是否穿透不同的session
		//	size_t timeout_ms = 30
		//	)
		//{
		//	HANDLE hevent = 0; /// is okay
		//	post(cmdid,data,data_out,0);
		//	wait_event(hevent,timeout_ms);
		//	return 0;
		//}
		//size_t broad_cast(
		//	size_t cmdid, /// 命令id
		//	char const* data,size_t size, /// 发送的数据
		//	response_d const& response,
		//	size_t response_threadid = -1, /// 需要响应的线程，
		//	bool bwait_all = false, /// 是否等到所有的节点返回，才触发事件
		//	size_t timeout_ms = 30,
		//	bool bsession_ignored = false /// 是否穿透不同的session
		//	) = 0;
		size_t add_listener(size_t cmdid,callback_struct const& listener)
		{
			/// create a stream
			/// send a command, to read a data, until the data arrived
			/// on the data arrived event to all the listener
		}
		size_t remove_listener(size_t cmdid,callback_struct const& listener)
		{
			/// remove from the map listener container
		}

	private:
		bool wait_event(HANDLE handle,size_t timeout_ms)
		{
			return true;
		}
	private:
		static constant::response_result_enum on_stream_received(abc::ipc_define::response_struct resp,abc::ipc_define::sender const& send)
		{
			return constant::__resp_unfinished;
		}
		static constant::response_result_enum ipc_package_handle(abc::ipc_define::response_struct resp,abc::ipc_define::sender const& send)
		{
			constant::response_result_enum ret = constant::__resp_unfinished;
			do {
				/// if error happens, close the connection
				if (resp.base->errorid!=0)
					break;
				if (resp.base->step!=constant::__step_read)
				{
					printf("\n[%d] ",resp.base->file_number);
					data_t data = resp.user_data();
					std::string str(data.begin,data.size);
					printf("%u,%u,%s",resp.protocol->size(),data.size,str.c_str());
					return constant::__resp_unfinished;
				}
				/// read action
				if (resp.base->step==constant::__step_read)
					printf("\n\t[%d] ",resp.base->file_number);
				//if (resp.base->step==constant::__step_read)
				//	return constant::__resp_finished_and_keepalive;
				//return constant::__resp_unfinished;
			} while (0);
			/// notify user
			header head;
			head.errorid = resp.base->errorid;
			head.seq = resp.base->seq;
			head.source = resp.protocol->get_response_address();
			data_t data = resp.protocol->get_data();
			if (resp.post_task.is_empty())
				resp.callback.event(head,data);
			else
				resp.post_task(task_single<int>::make(resp.callback,head,data),resp.callback.threadid);
			return constant::__resp_finished_and_close;
			return ret;
		}
	private:
		client_ipc* _m_client_ipc;
		connection _m_send_connection;
		connection _m_recv_connection;
		std::string _m_group_name;
		std::string _m_send_name;
		post_task_d _m_post_task;
	}; /// end of class client ipc node
}