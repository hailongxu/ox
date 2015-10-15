
#include "../utl/data_t.h"
#include "../cxx/delegate.h"
#include "../comm/communication_const.h"
#include "../comm/communication_public.h"
#include "ipc_sender_back.h"


#pragma once


___namespace2_begin(ox,ipc)


	struct ipcnode_define
	{
		enum group_handle_enum
		{
			__wait_one_by_one,
			__wait_first,
			__wait_first_successful,
			__wait_all,
		};
		typedef ox::comm::communication_const::sender_step_enum sender_step_enum;
		typedef ox::utl::cdata_t cdata_t;
		typedef ox::comm::communication_public::post_task_d post_task_d;
		typedef delegate<void(cdata_t /*name*/)> on_started_d;
		struct channelid
		{
			channelid():seq(0) {clear();}
			size_t seq;
			char name[64];
			bool is_empty() const {return seq==0 || name[0]==0;}
			void clear() {seq=0;memset(name,0,sizeof(name));}
		};
		struct header
		{
			size_t errorid;
			size_t seq;
			sender_step_enum step;
			cdata_t source;
			size_t commandid;
			void const* bind;
			cdata_t* bind_data_begin;
			size_t bind_data_size;
			cdata_t data;
		};
		struct group_header
		{
			size_t seq;
			size_t commandid;
			void const* bind;
			cdata_t* bind_data_begin;
			size_t bind_data_size;
			header* heads;
			size_t head_count;
		};
		typedef delegate<void(header head)> response_d;
		typedef delegate<void(group_header head)> group_response_d;
		struct bind_struct
		{
			bind_struct(): bind(0),threadid(-1),postid(-1),bind_data_begin(0),bind_data_size(0) {}
			void const* bind;
			size_t threadid; /// -1: ºöÂÔthreadid²ÎÊý
			size_t postid;
			cdata_t* bind_data_begin;
			size_t bind_data_size;
			void clear() {bind=0,threadid=-1,postid=-1,bind_data_begin=0,bind_data_size=0;}
		};
		struct response_struct
		{
			response_d event;
			bind_struct bind;
		};
		struct group_response_struct
		{
			group_response_struct():mode(__wait_all) {}
			group_response_d event;
			bind_struct bind;
			group_handle_enum mode; /// __wait_all
		};
		struct req_t
		{
			req_t():seq(0),errorid(0){}
			req_t(size_t _seq,size_t eid):seq(_seq),errorid(eid) {}
			size_t seq;
			size_t errorid;
		};
		typedef delegate<void(header,ipc_sender_back const& write)> ipc_service_d;
		typedef response_d ipc_listener_d;
		typedef response_struct listener_struct;
		struct service_struct
		{
			ipc_service_d event;
			bind_struct bind;
		};
	};


___namespace2_end()