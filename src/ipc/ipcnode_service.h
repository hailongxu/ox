
#include "ipcnode_interface.h"
#include "communication_ipc_with_session.h"
#include "ipc_protocol.h"


#pragma once


namespace ox
{
namespace ipc
{



/// the ipc node interface
struct ipcnode_service : ipcnode_interface
{
	/// public data type
	typedef ox::utl::cdata_t cdata_t;
	typedef communication_public::post_task_d post_task_d;
	typedef communication_const constant;


	typedef communication_ipc_with_session::cdata_t cdata_t;
	typedef ipcnode_define::response_struct response_struct;
	typedef ipcnode_define::channelid channelid;

	size_t init(char const* node_name)
	{
		if (_m_ipc.is_started()) return constant::__error_has_been_started;
		_m_ipc.init_param_set_ipc_node_name(node_name);
		_m_ipc.init();
		return constant::__error_null;
	}
	void set_post(size_t id,post_task_d const& post)
	{
		_m_ipc.set_post_task(id,post);
	}
	void set_on_started(on_started_d const& _on_started)
	{
		_m_ipc.on_server_started() = _on_started;
	}
	size_t start(/*char const* node_name*/)
	{
		if (_m_ipc.is_started()) return constant::__error_has_been_started;
		//_m_ipc.init_param_set_ipc_node_name(node_name);
		//_m_ipc.init();
		return _m_ipc.start();
	}
	/// join the group
	void enter_group(char const* group_name)
	{
		_m_ipc.enter(group_name);
	}
	void leave_group(char const* group_name)
	{
		_m_ipc.leave(group_name);
	}
	void get_list(data_t idlist,char const* name)
	{
		std::string strlist;
		size_t list_count = 0;
		_m_ipc.get_list(strlist,list_count,ox::utl::to_data(name),true,true);
		if (idlist.is_empty()) return;
		strncpy_s(idlist.begin,idlist.size,strlist.data(),strlist.size());
	}

	void stop()
	{
		_m_ipc.stop();
	}
	/// destid: the node id, not group id
	channelid create_channel(char const* destid,bool self_included=false,bool bsession_ignored=false)
	{
		return _m_ipc.create_channel(ox::utl::to_data(destid),self_included,bsession_ignored);
	}
	void close_channel(channelid const& channel)
	{
		_m_ipc.close_channel(channel);
	}
	/// sequence
	size_t post_channel(
		channelid const& channel, /// channel id
		size_t cmdid, /// 命令id
		cdata_t data, /// 发送的数据
		response_struct const& response,
		size_t timeout_ms,
		size_t* reqid
		)
	{
		return _m_ipc.post_channel(reqid,channel,cmdid,data,response,timeout_ms);
	}
	/// not sequence
	size_t post(
		char const* destid, /// node id, or group id
		size_t cmdid, /// 命令id
		cdata_t data, /// 发送的数据
		group_response_struct const& response,
		size_t timeout_ms,
		bool self_included = false, /// 是否包含自己
		bool bsession_ignored = false, /// 是否穿透不同的session
		size_t* reqid = 0
		)
	{
		return _m_ipc.post(reqid,destid,cmdid,data,response,timeout_ms,self_included);
	}
	/// synchronized
	size_t send_channel(
		channelid const& channel, /// channel id
		size_t cmdid, /// 命令id
		cdata_t data, /// 发送的数据
		response_struct const& response,
		size_t timeout_ms
		)
	{
		ox::mos::win_auto_event auto_event(false);
		HANDLE wait_handle = auto_event.handle();
		response_struct send_response;
		response_struct const* response_pointer = &response;
		cdata_t handle_data ((char const*)&wait_handle,sizeof(wait_handle));
		cdata_t response_data ((char const*)&response_pointer,sizeof(response_pointer));
		cdata_t binddata[2] = {handle_data,response_data};
		send_response.bind.bind_data_begin = binddata;
		send_response.bind.bind_data_size = 2;
		send_response.event.assign(on_send_channel_response_byfunc);
		size_t ret = _m_ipc.post_channel(0,channel,cmdid,data,send_response,timeout_ms);
		if (ret==communication_const::__error_ipcnode_list_is_null) ox::mos::win_auto_event::set_signaled(wait_handle);
		if (ret==0) {ox::mos::scope_auto_event lock(auto_event);}
		return ret;
	}
	/// cast, not sequence synchronized
	size_t send(
		char const* destid, /// node id, or group id
		size_t cmdid, /// 命令id
		cdata_t data, /// 发送的数据
		group_response_struct const& response,
		size_t timeout_ms,
		bool self_included = false, /// 是否包含自己
		bool bsession_ignored = false /// 是否穿透不同的session
		)
	{
		ox::mos::win_auto_event auto_event(false);
		HANDLE wait_handle = auto_event.handle();
		group_response_struct send_response;
		group_response_struct const* response_pointer = &response;
		cdata_t handle_data ((char const*)&wait_handle,sizeof(wait_handle));
		cdata_t response_data ((char const*)&response_pointer,sizeof(response_pointer));
		cdata_t binddata[2] = {handle_data,response_data};
		send_response.bind.bind_data_begin = binddata;
		send_response.bind.bind_data_size = 2;
		send_response.event.assign(on_send_group_response_byfunc);
		size_t ret = _m_ipc.post(0,destid,cmdid,data,send_response,timeout_ms,self_included);
		if (ret==communication_const::__error_ipcnode_list_is_null) ox::mos::win_auto_event::set_signaled(wait_handle);
		if (ret==0) {ox::mos::scope_auto_event lock(auto_event);}
		return ret;
	}
	/// the ipc service
	void add_listener(size_t cmdid,ipc_listener_d const& listener,bind_struct const& bind)
	{
		_m_ipc.add_listener(cmdid,listener,bind);
	}
	void add_service(size_t cmdid,ipc_service_d const& listener,bind_struct const& bind)
	{
		_m_ipc.add_service(cmdid,listener,bind);
	}
	void remove_listener(size_t cmdid,ipc_listener_d const& listener)
	{
		_m_ipc.remove_listener(cmdid,listener);
	}
	void remove_service(size_t cmdid)
	{
		_m_ipc.remove_service(cmdid);
	}
	static void on_send_channel_response_byfunc(ipcnode_define::header head)
	{
		assert (head.bind_data_size>=2 &&
			head.bind_data_begin[0].size==sizeof(HANDLE) &&
			head.bind_data_begin[1].size==sizeof(response_struct*));
		/// get the parameter data address
		HANDLE wait_handle = *(HANDLE*)head.bind_data_begin[0].begin;
		response_struct* rsp = *(response_struct**)head.bind_data_begin[1].begin;
		if (rsp && !rsp->event.is_empty())
		{
			head.bind = rsp->bind.bind;
			head.bind_data_begin = rsp->bind.bind_data_begin;
			head.bind_data_size = rsp->bind.bind_data_size;
			rsp->event(head);
		}
		ox::mos::win_auto_event::set_signaled(wait_handle);
	}
	static void on_send_group_response_byfunc(ipcnode_define::group_header head)
	{
		assert (head.bind_data_size>=2 &&
			head.bind_data_begin[0].size==sizeof(HANDLE) &&
			head.bind_data_begin[1].size==sizeof(group_response_struct*));
		/// get the parameter data address
		HANDLE wait_handle = *(HANDLE*)head.bind_data_begin[0].begin;
		group_response_struct* rsp = *(group_response_struct**)head.bind_data_begin[1].begin;
		if (rsp && !rsp->event.is_empty()) rsp->event(head);
		ox::mos::win_auto_event::set_signaled(wait_handle);
	}

	communication_ipc_with_session _m_ipc;
}; /// end of class ipcnode_service

struct data_service : data_interface
{
	cdata_t data() const
	{
		return cdata_t(_m_data.data(),_m_data.size());
	}
	void append(cdata_t const& data)
	{
		_m_data.append(data.begin,data.size);
	}
	void append(char const* data,size_t size)
	{
		_m_data.append(data,size);
	}
	void clear()
	{
		_m_data.clear();
	}
	std::string _m_data;
};
//struct group_protocol_service : group_protocol_interface
//{
//	typedef abc::ipc_protocol::ipc_protocol_multi_buff ipc_group;
//	size_t get_count(cdata_t const& data) const
//	{
//		if (data.is_empty()) return 0;
//		ipc_group* group = ipc_group::as_ipc_protocol_multi(data);
//		return group->get_count();
//	}
//	header* get_head(cdata_t const& data,size_t i) const
//	{
//		if (data.is_empty()) return 0;
//		ipc_group* group = ipc_group::as_ipc_protocol_multi(data);
//		return (header*)(group->get_head(i).begin);
//	}
//	cdata_t get_data(cdata_t const& data,size_t i) const
//	{
//		if (data.is_empty()) return cdata_t();
//		ipc_group* group = ipc_group::as_ipc_protocol_multi(data);
//		return group->get_data(i);
//	}
//};
//
struct factory_ipc
{
	enum id_enum
	{
		__id_ipc=1,
		__id_data,
	};
	struct classid
	{
		id_enum id;
		void* object;
		bool operator==(classid const& o) const
		{
			return id==o.id && object==o.object;
		}
		void stop()
		{
			switch (id==__id_ipc)
			{
			case __id_ipc:
				{
					ipcnode_service* obj = reinterpret_cast<ipcnode_service*>(object);
					obj->stop();
				}
				break;
			case __id_data:
				break;
			default:
				assert (false);
			}
		}
		void destroy()
		{
			switch (id==__id_ipc)
			{
			case __id_ipc:
				{
					ipcnode_service* obj = reinterpret_cast<ipcnode_service*>(object);
					delete obj;
				}
				break;
			case __id_data:
				{
					data_service* obj = reinterpret_cast<data_service*>(object);
					delete obj;
				}
				break;
			default:
				assert (false);
			}
		}
	};
	struct ipcnode_id : classid
	{
		ipcnode_id(ipcnode_service* ipc) {id=__id_ipc;object=ipc;}
	};
	struct data_id : classid
	{
		data_id(data_interface* data) {id=__id_data;object=data;}
	};
	typedef std::vector<classid> all_interface;
	data_interface* create_data()
	{
		data_service* data = new (std::nothrow) data_service;
		if (!data) return 0;
		data_id objid (data);
		ox::mos::scope_cs lock(_m_cs);
		_m_all.push_back(objid);
		return data;
	}
	ipcnode_interface* create_ipcnode(char const* node_name)
	{
		ipcnode_service* ipc = new (std::nothrow) ipcnode_service;
		if (!ipc) return 0;
		ipcnode_id ipcnode (ipc);
		ipc->init(node_name);
		ox::mos::scope_cs lock(_m_cs);
		_m_all.push_back(ipcnode);
		return ipc;
	}
	//group_protocol_interface& get_group_protocol()
	//{
	//	return _m_group_protocol_service;
	//}
	void destroy_ipcnode(ipcnode_interface* obj)
	{
		destroy_obj(obj,__id_ipc);
	}
	//void destroy_data(data_interface* obj)
	//{
	//	destroy_obj(obj,__id_data);
	//}
	void destroy_all()
	{
		typedef all_interface::iterator I;
		ox::mos::scope_cs lock(_m_cs);
		for (I i=_m_all.begin();i!=_m_all.end();)
		{
			classid id = *i;
			id.stop();
			id.destroy();
			i = _m_all.erase(i);
		}
	}
	void destroy_obj(void* obj,id_enum _id)
	{
		typedef all_interface::iterator I;
		classid id = {_id,obj};
		ox::mos::scope_cs lock(_m_cs);
		I i = find(id);
		if (i==_m_all.end())
		{
			assert (false);
			return;
		}
		id.stop();
		id.destroy();
		_m_all.erase(i);
	}
	all_interface::iterator find(classid id)
	{
		typedef all_interface::iterator I;
		for (I i=_m_all.begin();i!=_m_all.end();++i)
			if (*i==id)
				return i;
		return _m_all.end();
	}

	ox::mos::critical_section _m_cs;
	all_interface _m_all;
	//group_protocol_service _m_group_protocol_service;
};



} /// end of ipc namespace
}