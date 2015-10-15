

#include "communication_ipc.h"
#include "ipc_id_list.h"


#pragma once


___namespace2_begin(ox,ipc)

using namespace ox::comm;

struct communication_ipc_with_session
{
	typedef communication_ipc_with_session self;
	typedef communication_ipc base;
	typedef base::cdata_t cdata_t;
	typedef base::response_struct response_struct;
	typedef base::group_response_struct group_response_struct;
	typedef base::response_d response_d;
	typedef base::all_exited_d all_exited_d;
	typedef base::ipc_listener_d ipc_listener_d;
	typedef base::ipc_service_d ipc_service_d;
	typedef base::service_struct service_struct;
	typedef base::listener_struct listener_struct;
	typedef base::bind_struct bind_struct;
	typedef base::post_task_d post_task_d;
	typedef base::task_single task_single;
	typedef client_connect_t::connection_info connection_info;
	typedef ipcnode_define::channelid channelid;
	typedef ipcnode_define::on_started_d on_started_d;

	~communication_ipc_with_session()
	{
		log_proxy::instance().printf("ipc.~communication_ipc_with_session\r\n");
	}
	self(): _m_ipcid_mutex(true)
	{}
	size_t init_param_set_ipc_node_name(char const* node_name)
	{
		cdata_t name = ox::utl::to_data(node_name);
		bool is_valid = ipc_list_make_helper::is_node_name_valid(name);
		if (!is_valid)
		{
			assert (false && "name is not valid format");
			return communication_const::__error_invalid_node_name;
		}
		_m_ipc.init_param_set_ipc_node_name(node_name);
		_m_ipc.init_param_set_to_ipc_pipe_name(ipc_id_list::ipc_pipe_name);
		_m_ipcid_mutex.init(ipc_id_list::ipc_mutex_name(name).c_str());
		return 0;
	}

	void init()
	{
		log_proxy::instance().printf("ipc +++ node [%s] init called\r\n",_m_ipc.node_name().begin);
		_m_ipc.init();
		_m_id_list.init(_m_ipc.node_name().begin);
		_m_ipc.on_connect_error_list().assign(this,&self::on_connection_error);
	}

	size_t start()
	{
		return _m_ipc.start();
	}
	void stop()
	{
		log_proxy::instance().printf("ipc --- node [%s] stop called\r\n",_m_ipc.node_name().begin);
		_m_ipc.stop();
		_m_id_list.remove_from_all_groups();
		_m_ipcid_mutex.uninit();
	}
	bool is_started() const
	{
		return _m_ipc.is_started();
	}
	on_started_d& on_server_started()
	{
		return _m_ipc.on_server_started();
	}
	all_exited_d& on_exited()
	{
		return _m_ipc.on_exited();
	}
	channelid create_channel(cdata_t const& destid,bool self_included=false,bool is_session_cross=false,size_t* errorid=0)
	{
		log_proxy::instance().printf("ipc: create_channel to[%s]\r\n",ox::utl::to_string(destid).c_str());
		size_t error_code = 0;
		channelid channel_item;
		do {
			std::string idlist;
			size_t pid_count=0;
			error_code = get_list(idlist,pid_count,destid,is_session_cross,self_included);
			if (error_code!=0 || pid_count==0)
			{
				log_proxy::instance().printf("ipc.crete_channel failed errid:[%u]\r\n",error_code);
				break;
			}
			if (pid_count!=1)
			{
				channelid_list::keep_top(idlist);
				pid_count = 1;
			}
			memmove(channel_item.name,idlist.data(),idlist.size());
			channel_item.seq = channelid_list::seq();
			channelid_list::make_index(channel_item);
			log_proxy::instance().printf("ipc.create_channel success :[seq:%u,%s]\r\n",channel_item.seq,channel_item.name);
			_m_ipc.set_index_filemax(ox::utl::to_data(channel_item.name),1);
			_m_ipc.post_task(task_single::make(this,&self::do_update_channel,channel_item.seq));
		} while(0);
		if (errorid) *errorid = error_code;
		return channel_item;
	}
	void close_channel(channelid const& channel)
	{
		log_proxy::instance().printf("ipc.close_channel:[%u,%s]\r\n",channel.seq,channel.name);
		if (channel.is_empty()) return;
		_m_ipc.close_channel(ox::utl::to_data(channel.name));
		_m_ipc.post_task(task_single::make(this,&self::do_erase_channel,channel.seq));
	}
	void enter(char const* group_name)
	{
		_m_id_list.add_into_group(group_name);
	}
	void leave(char const* group_name)
	{
		_m_id_list.remove_from_group(group_name);
	}
	size_t get_list(std::string& idlist,size_t& count,cdata_t const& singleid,bool self_included,bool is_session_cross_okay)
	{
		ipc_list_make_helper get_list_helper(_m_id_list);
		return get_list_helper.get_id_list(idlist,count,singleid,self_included,is_session_cross_okay);
	}

	void add_listener(size_t cmdid,listener_struct const& listener)
	{
		_m_ipc.add_listener(cmdid,listener);
	}
	void add_service(size_t cmdid,service_struct const& service)
	{
		_m_ipc.add_service(cmdid,service);
	}
	void add_listener(size_t cmdid,ipc_listener_d const& listener,bind_struct const& bind=bind_struct())
	{
		listener_struct ls = {listener,bind};
		_m_ipc.add_listener(cmdid,ls);
	}
	void add_service(size_t cmdid,ipc_service_d const& service,bind_struct const& bind=bind_struct())
	{
		service_struct ss = {service,bind};
		_m_ipc.add_service(cmdid,ss);
	}
	void remove_listener(size_t cmdid,ipc_listener_d const& listener)
	{
		_m_ipc.remove_listener(cmdid,listener);
	}
	void remove_service(size_t cmdid)
	{
		_m_ipc.remove_service(cmdid);
	}
	void set_post_task(size_t id,post_task_d const& post_task)
	{
		_m_ipc.set_post_task(id,post_task);
	}

	//size_t try_connect(channelid const& chid)
	//{
	//	_m_ipc.post_task(task_single::make(this,&self::do_try_connect,chid));
	//}

	size_t post_channel(size_t* reqid,channelid const& channel,size_t cmdid,cdata_t const& data,
		response_struct const& response,size_t timeout_ms=-1)
	{
		cdata_t destid = ox::utl::to_data(channel.name);
		if (channel.is_empty())
		{
			ipcnode_define::header head;
			head.commandid = cmdid;
			head.source.assign("",0);
			head.errorid = communication_const::__error_ipcnode_list_is_null;
			head.seq = 0; /// ???? the accurate value of seq if the node is null
			head.step = communication_const::__step_prepare;
			head.bind = response.bind.bind;
			head.bind_data_begin = response.bind.bind_data_begin;
			head.bind_data_size = response.bind.bind_data_size;
			head.data.assign("",0);
			_m_ipc.post_event(response,head);
			return communication_const::__error_ipcnode_list_is_null;
		}
		_m_ipc.post(reqid,destid,cmdid,data,response,timeout_ms);
		return 0;
	}

	size_t post(size_t* reqid,cdata_t const& destid,size_t cmdid,cdata_t const& data,
		group_response_struct const& response,
		size_t timeout_ms = -1,bool self_included=false,bool is_session_cross_okay = false)
	{
		return internal_post(reqid,destid,_m_ipc.node_name(),cmdid,data,response,
			timeout_ms,self_included,is_session_cross_okay);
	}
	size_t post(size_t* reqid,char const* destid,size_t cmdid,cdata_t const& data,
		group_response_struct const& response,
		size_t timeout_ms = -1,bool self_included=false,bool is_session_cross_okay = false)
	{
		return internal_post(reqid,ox::utl::to_data(destid),_m_ipc.node_name(),cmdid,data,response,
			timeout_ms,self_included,is_session_cross_okay);
	}
	size_t post(size_t* reqid,cdata_t const& destid,size_t cmdid,cdata_t const& data,
		size_t timeout_ms = -1,bool self_included=false,bool is_session_cross_okay = false)
	{
		return internal_post(reqid,destid,_m_ipc.node_name(),cmdid,data,group_response_struct(),
			timeout_ms,self_included,is_session_cross_okay);
	}

	size_t internal_post(
		size_t* reqid,
		cdata_t const& singleid,
		cdata_t const& srcid,
		size_t cmdid,
		cdata_t const& data,
		group_response_struct const& response,
		size_t timeout_ms /*= -1*/,
		bool self_included = false,
		bool is_session_cross_okay = false /*= false*/
		)
	{
		/// get list
		std::string idlist;
		size_t count = 0;
		get_list(idlist,count,singleid,self_included,is_session_cross_okay);
		log_proxy::instance().printf("ipc.internal_post(dest:[%s],cmdid:%u,to:%u)\r\n",idlist.c_str(),cmdid,timeout_ms);
		if (count==0)
			return communication_const::__error_ipcnode_list_is_null;
		cdata_t destids = ox::utl::to_data(idlist);
		return _m_ipc.post_many(reqid,destids,cmdid,data,response,timeout_ms);
	}

private:
	//void do_try_connect(channelid const& id)
	//{
	//	channelid_list::item_t* item = _m_channel_list.find(id.id);
	//	if (!item) return;
	//	_m_ipc.try_connect();
	//}
	void do_erase_channel(size_t seq)
	{
		_m_channel_list.erase(seq);
	}
	void do_update_channel(size_t channel_seq)
	{
		_m_channel_list.update(channel_seq);
	}
	void on_connection_error(connection_info const* conns,size_t size)
	{
		/// we should update the name share, {share} -= {name}
		typedef ox::str::astring_kit strkit;
		for (size_t i=0;i<size;++i)
		{
			connection_info const& conn = conns[i];
			if (conn.errorid!=2) continue; /// file does not exist, so we erase pid from node file
			cdata_t nidpid = ipc_all_list::get_nidpid(conn.name);
			ipc_list_make_helper(_m_id_list).remove_nidpid(nidpid);
		}
	}

private:
	communication_ipc _m_ipc;
	safe_ipc_all_list _m_id_list;
	channelid_list _m_channel_list;
	ox::mos::win_mutex _m_ipcid_mutex;
};

___namespace2_end()