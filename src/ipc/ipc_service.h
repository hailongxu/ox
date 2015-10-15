
#include "delegate.h"
#include "../ox/utl/data_t.h"
#include "ipcnode_define.h"

#pragma once


namespace abc
{
	namespace ipc
	{
		typedef ox::utl::data_t data_t;
		typedef abc::communication_define::post_task_d post_task_d;
		typedef communication_const constant;
		typedef abc::ipcnode_define::callback_struct callback_struct;
		typedef abc::ipcnode_define::header header;
		typedef abc::ipcnode_define::ret_t ret_t;
		typedef abc::ipcnode_define::stop_event_d stop_event_d;
		typedef abc::ipcnode_define::server_listener_d server_listener_d;

		struct data_interface
		{
			char const* append(data_t data) = 0;
			data_t get() = 0;
		};

		struct ipc_client_interface
		{
			size_t set_post(post_task_d const& post) = 0;
			/// join the group
			size_t start(char const* group_name) = 0;
			size_t stop() = 0;
			size_t post(
				size_t cmdid, /// ����id
				data_t data, /// ���͵�����
				callback_struct const& callback,
				size_t timeout_ms,
				bool bsession_ignored = false /// �Ƿ�͸��ͬ��session
				) = 0;
			size_t send(
				size_t cmdid, /// ����id
				data_t data, /// ���͵�����
				data_interface* data_out, /// ���ص�����
				size_t timeout_ms,
				bool bsession_ignored = false /// �Ƿ�͸��ͬ��session
				) = 0;
			/// �������Է�����������
			size_t add_listener(size_t cmdid,callback_struct const& listener) = 0;
			size_t remove_listener(size_t cmdid,callback_struct const& listener) = 0;
		}; /// end of client interface

		struct ipc_server_interface
		{
			/// ��ʼ�������������������Ͻ���������
			size_t init(char group_name) = 0;
			size_t uninit() = 0;
			size_t start() = 0;
			size_t stop(stop_event_d const& stop_event) = 0;
			size_t broadcast(
				size_t cmdid, /// ����id
				data_t data, /// ���͵�����
				bool bsession_ignored = false /// �Ƿ�͸��ͬ��session
				) = 0;
			/// �������Կͻ��˵�����
			size_t add_listener(size_t cmdid,server_listener_d listener) = 0;
			size_t remove_listener(size_t cmdid,server_listener_d listener) = 0;
		}; /// end of ipc server interface
	} /// end of ipc namespace
}