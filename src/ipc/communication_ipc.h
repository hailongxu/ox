
#include "../utl/data_t.h"
#include "../utl/vecbuff.h"
#include "../utl/path_kit.h"
#include "../utl/data_helper.h"
#include "../comm/client_helper.h"
#include "../comm/communication_client.h"
#include "post_task_proxy.h"
#include "client_pipe_connect.h"
#include "server_pipe_accept.h"
#include "file_pipe_operation.h"
#include "ipc_define.h"
#include "ipcnode_define.h"
#include "ipc_helper_internal.h"
#include "ipc_one_process_node_list.h"


#pragma once



___namespace2_begin(ox,ipc)

using namespace ox::comm;

	struct communication_ipc
	{
		friend class communication_ipc_with_session;
		typedef communication_ipc self;
		typedef communication_client<client_helper> base;
		typedef ox::utl::cdata_t cdata_t;
		typedef communication_const constant;
		typedef communication_public::post_task_d post_task_d;
		typedef communication_define::connection_id connection_id;
		typedef ipc_define::ipc_event_d ipc_event_d;
		typedef ipc_define::to_ipc_pipe_name_d to_ipc_pipe_name_d;
		typedef ipcnode_define::response_d response_d;
		typedef ipcnode_define::ipc_service_d ipc_service_d;
		typedef ipcnode_define::ipc_listener_d ipc_listener_d;
		typedef ipcnode_define::response_struct response_struct;
		typedef ipcnode_define::group_response_struct group_response_struct;
		typedef ipcnode_define::service_struct service_struct;
		typedef ipcnode_define::listener_struct listener_struct;
		typedef ipcnode_define::bind_struct bind_struct;
		typedef ipcnode_define::req_t req_t;
		typedef ipcnode_define::on_started_d on_started_d;
		typedef client_connect_t::connect_error_list_d connect_error_list_d;
		typedef base::all_exited_d all_exited_d;
		typedef base::helper::task_single task_single;

	private:
		typedef client_pipe_connect connect_t;
		typedef server_pipe_aaccept<accept_pipe_trait> accept_t;
		typedef file_pipe_operation file_operation;
		typedef ipc_define::timer_struct timer_struct;
		typedef base::timer_t timer_t;
		typedef communication_define::cstr_t cstr_t;
		typedef ox::utl::vecbuff_t vecbuff_t;
		struct listener2_struct : listener_struct
		{
			post_task_d post_task;
		};
		typedef std::vector<listener2_struct> ipc_multi_listener_d;

		struct listener_t;
		struct service_t;
		struct classid
		{
			enum classid_enum {__singleid,__returnid,};
			classid_enum id;
			classid(classid_enum _id): id(_id) {}
			classid() : id(__singleid) {}
			bool is_single() const {return id==__singleid;}
			bool is_return() const {return id==__returnid;}
			void destroy_self()
			{
				switch (id)
				{
				case __singleid: delete static_cast<listener_t*>(this); break;
				case __returnid: delete static_cast<service_t*>(this); break;
				default : assert(false);
				}
			}
		};
		struct service_t :classid
		{
			service_t():classid(classid::__returnid) {}
			service_struct handle;
			post_task_d post_task;
			response_struct back_response;
		};
		struct listener_t : classid
		{
			listener_t():classid(classid::__singleid) {}
			ipc_multi_listener_d handles;
		};
		typedef std::map<size_t,classid*> ipc_handle_map;

		struct result_group
		{
			static size_t seq_next()
			{
				static ox::mos::atomic_number<long> _static_seq(0);
				return ++_static_seq;
			}

			void init_magic()
			{
				for (int i=0;i<8;++i) _m_magic_seq[i]='a'+i;
			}
			bool is_init_magic() const
			{
				for (int i=0;i<8;++i)
					if (_m_magic_seq[i]!='a'+i) return false;
				return true;
			}
			void clear_magic() const
			{
				memset((char*)_m_magic_seq,1,sizeof(_m_magic_seq));
			}
			char _m_magic_seq[8];
			ipcnode_define::group_header _m_head;
			group_response_struct _m_response;
			std::vector<ipcnode_define::header> _m_contents;
			std::vector<std::string> _m_contents_data;
			std::string _m_destid_list;
			std::string _m_binddata;
			size_t _m_destid_count;
			ox::mos::atomic_long _m_recv_count;
			ipcnode_define::group_handle_enum _m_mode; /// wait first, wait all, one by one
		};
		template <typename value_tn,typename bind_tn>
		struct size_value_map
		{
			typedef value_tn value_t;
			typedef std::map<size_t,value_t*> values_t;
			typedef client_helper::allocator_t allocator_t;
			typedef size_t key_t;

			size_value_map(allocator_t& allocator)
				: _m_allocator(allocator)
			{}

			~size_value_map()
			{
				clear();
			}
			value_t* create(key_t const& name)
			{
				value_t* g = _m_allocator._new<value_t>();
				return g;
			}
			value_t* add(key_t const& name)
			{
				typedef values_t::iterator I;
				I i = _m_groups.find(name);
				if (i!=_m_groups.end())
					return i->second;
				value_t* g = _m_allocator._new<value_t>();
				if (!g) return 0;
				_m_groups[name] = g;
				return g;
			}
			value_t* find(key_t const& name)
			{
				typedef values_t::iterator I;
				value_t* group = 0;
				I i = _m_groups.find(name);
				if (i==_m_groups.end())
					return 0;
				return i->second;
			}
			void remove(key_t const& name)
			{
				_m_groups.erase(name);
			}
			void destroy(value_t* value)
			{
				_m_allocator._delete(value);
			}
			void clear()
			{
				typedef values_t::iterator I;
				for (I i=_m_groups.begin();i!=_m_groups.end();++i)
					_m_allocator._delete(i->second);
				_m_groups.clear();
			}
			values_t _m_groups;
			allocator_t& _m_allocator;
			bind_tn _m_bind;
		};
		typedef size_value_map<result_group,int> multi_result_group_t;
		typedef size_value_map<post_task_d,int> multi_post_t;

	public:
		base _m_base;
		~communication_ipc()
		{
			log_proxy::instance().printf("ipc.~communication_ipc()\r\n");
			clear_all_commands();
		}
		self()
			: _m_ipcc("ipcc")
			, _m_ipcs("ipcs")
			, _m_ipcs_slash("ipcs/")
			, _m_group_result(_m_base.allocator())
			, _m_multi_post_task(_m_base.allocator())
		{}
		void init_param_set_ipc_node_name(char const* ipc_node_name)
		{
			if (ipc_node_name) _m_ipc_name=ipc_node_name;
		}
		void init_param_set_to_ipc_pipe_name(to_ipc_pipe_name_d const& to_pipe_name)
		{
			_m_to_ipc_pipe_name = to_pipe_name;
		}
		void init()
		{
			log_proxy::instance().printf("ipc.communication_ipc.init()\r\n");
			_m_base.init_param_set_client_connect(_m_connect.as_client_connect());
			_m_base.init_param_set_server_accept(_m_accept.as_server_accept(),_m_ipcs_slash);
			_m_base.init_param_set_handle_operation(file_operation::as_file_handle_operation());
			_m_base.init();

			/// create client group
			_m_base.create_package(_m_ipcc);
			/// create server group
			ipc_helper_internal::sender_struct ss(0);
			ss.on_ipc_feedback().assign(this,&self::ipc_server_handle);
			ss._m_ipc_sender._m_user_response_struct.bind.clear();
			ss._m_ipc_sender._m_user_response_struct.event.clear();
			ss.name().set(_m_ipcs_slash);
			_m_base.create_stream(_m_ipcs);
			_m_base.set_recv_to_stream(_m_ipcs,ss,0);
			_m_base.set_send_to_stream(_m_ipcs,ss,0);
			_m_base.set_send_back(_m_ipcs,communication_define::send_back_d(this,&self::send_back));

			/// clear all registered command
			clear_all_commands();
		}

		size_t start()
		{
			log_proxy::instance().printf("ipc.communication_ipc.start()\r\n");
			std::string ipc_pipe_name = _m_ipc_name;
			if (!_m_to_ipc_pipe_name.is_empty())
				ipc_pipe_name = _m_to_ipc_pipe_name(ox::utl::to_data(_m_ipc_name));
			bool okay = this_process_ipc_list().merge(ox::utl::to_data(ipc_pipe_name));
			assert (okay);
			if (!okay) return constant::__error_name_has_been_used;
			if (!_m_on_started.is_empty())
				_m_accept.on_started().assign(this,&self::on_accept_started_notified);
			return _m_base.start(ox::utl::to_data(ipc_pipe_name));
		}
		void stop()
		{
			log_proxy::instance().printf("ipc.communication_ipc.stop()\r\n");
			_m_base.stop();
		}

		bool is_started() const
		{
			return _m_base.is_started();
		}

		void post_task(ox::mos::thread_task_t* task)
		{
			_m_base.try_post_task(task);
		}

		cdata_t node_name() const
		{
			return ox::utl::to_data(_m_ipc_name);
		}
		on_started_d& on_server_started()
		{
			return _m_on_started;//_m_accept.on_started();
		}
		all_exited_d& on_exited()
		{
			return _m_base.on_exited();
		}
		connect_error_list_d& on_connect_error_list()
		{
			return _m_connect.on_error();
		}

		size_t post_many(size_t* reqid,char const* destids,size_t cmdid,cdata_t data,group_response_struct const& response,size_t timeout_ms=-1)
		{
			cdata_t zero;
			cdata_t destid = ox::utl::to_data(destids);
			return internal_group_send(reqid,destid,zero,cmdid,data,response,timeout_ms);
		}
		size_t post_many(size_t* reqid,cdata_t const& destids,size_t cmdid,cdata_t data,group_response_struct const& response,size_t timeout_ms=-1)
		{
			cdata_t zero;
			return internal_group_send(reqid,destids,zero,cmdid,data,response,timeout_ms);
		}
		/// scatter
		size_t post(size_t* reqid,char const* destid,size_t cmdid,cdata_t data,response_struct const& response,size_t timeout_ms=-1)
		{
			char internal_name[128] = {0};
			cdata_t zero;
			cdata_t dest = ox::utl::to_data(destid);
			make_send_indexname(internal_name,dest);
			return internal_send(reqid,internal_name,zero,zero,dest,0,cmdid,zero,data,response,timeout_ms);
		}
		size_t post(size_t* reqid,cdata_t const& destid,size_t cmdid,cdata_t data,response_struct const& response,size_t timeout_ms=-1)
		{
			char internal_name[128] = {0};
			cdata_t zero;
			make_send_indexname(internal_name,destid);
			return internal_send(reqid,internal_name,zero,zero,destid,0,cmdid,zero,data,response,timeout_ms);
		}
		size_t post(size_t* reqid,char const* destid,size_t cmdid,cdata_t data,size_t timeout_ms=-1)
		{
			char internal_name[128] = {0};
			cdata_t zero;
			cdata_t dest = ox::utl::to_data(destid);
			make_send_indexname(internal_name,dest);
			return internal_send(reqid,internal_name,zero,zero,dest,0,cmdid,zero,data,response_struct(),timeout_ms);
		}
		/// sequence
		size_t post(size_t* reqid,connection_id const& conn,size_t cmdid,cdata_t data)
		{
			char const* destid = ox::utl::localpath_kit<char>::get_last(conn.name);
			char internal_name[128] = {0};
			cdata_t zero;
			cdata_t dest = ox::utl::to_data(destid);
			make_send_indexname(internal_name,dest);
			return internal_send(reqid,internal_name,zero,zero,dest,conn.file_number,cmdid,zero,data,response_struct(),-1);
		}
		size_t post(size_t* reqid,connection_id const& conn,size_t cmdid,cdata_t data,response_struct const& response,size_t timeout_ms=-1)
		{
			char const* destid = ox::utl::localpath_kit<char>::get_last(conn.name);
			char internal_name[128] = {0};
			cdata_t zero;
			cdata_t dest = ox::utl::to_data(destid);
			make_send_indexname(internal_name,dest);
			return internal_send(reqid,internal_name,zero,zero,dest,conn.file_number,cmdid,zero,data,response,timeout_ms);
		}

		connection_id create_connection(char const* destid)
		{
			typedef base::connection connection;
			assert(destid && *destid);
			std::string name = _m_ipcc;
			name.append("/").append(destid);
			connection conn = _m_base.create_connection(name);
			connection_id conn_id;
			conn_id.file_number = conn.file_number();
			conn_id.name = conn.name();
			return conn_id;
		}
		void close_channel(cdata_t const& name)
		{
			_m_base.close_index(ox::utl::to_string(name));
		}
		void set_index_filemax(cdata_t const& name,size_t filemax)
		{
			base::option_filemax odata;
			make_send_indexname(odata.name,name);
			odata.line_max = filemax;
			_m_base.set_option(odata);
		}

		void add_service(size_t cmdid,service_struct const& service,response_struct const& back_response)
		{
			assert (!cmdid_helper(cmdid).is_not_back());
			_m_base.try_post_task(task_single::make(this,&self::do_add_service,cmdid,service,back_response));
		}
		void add_service(size_t cmdid,service_struct const& service)
		{
			assert (!cmdid_helper(cmdid).is_not_back());
			_m_base.try_post_task(task_single::make(this,&self::do_add_service,cmdid,service,response_struct()));
		}
		void add_service(size_t cmdid,ipc_service_d const& service,bind_struct const& bind=bind_struct())
		{
			assert (!cmdid_helper(cmdid).is_not_back());
			service_struct ss = {service,bind};
			_m_base.try_post_task(task_single::make(this,&self::do_add_service,cmdid,ss,response_struct()));
		}
		void add_listener(size_t cmdid,ipc_listener_d const& listener,bind_struct const& bind=bind_struct())
		{
			assert (!cmdid_helper(cmdid).is_not_back());
			listener_struct ls = {listener,bind};
			_m_base.try_post_task(task_single::make(this,&self::do_add_listener,cmdid,ls));
		}
		void add_listener(size_t cmdid,listener_struct const& listener)
		{
			assert (!cmdid_helper(cmdid).is_not_back());
			_m_base.try_post_task(task_single::make(this,&self::do_add_listener,cmdid,listener));
		}
		void remove_service(size_t cmdid)
		{
			_m_base.try_post_task(task_single::make(this,&self::do_remove_service,cmdid));
		}
		void remove_listener(size_t cmdid,ipc_listener_d const& listener)
		{
			_m_base.try_post_task(task_single::make(this,&self::do_remove_listener,cmdid,listener));
		}
		void set_post_task(size_t id,post_task_d const& post_task)
		{
			post_task_d* pvalue = _m_multi_post_task.add(id);
			assert (pvalue);
			*pvalue = post_task;
		}

	private:
		std::string _m_ipc_name;
		connect_t _m_connect;
		accept_t _m_accept;
		on_started_d _m_on_started;
		to_ipc_pipe_name_d _m_to_ipc_pipe_name;
		multi_result_group_t _m_group_result;
		ipc_handle_map _m_ipc_handles;
		multi_post_t _m_multi_post_task;
		char const* _m_ipcc;
		char const* _m_ipcs;
		char const* _m_ipcs_slash;

	private:
		void on_accept_started_notified(cdata_t name)
		{
			if (_m_on_started.is_empty()) return;
			post_task(task_single::make(this,&self::do_on_accept_started,ox::utl::to_string(name)));
		}
		void do_on_accept_started(std::string const& name)
		{
			_m_on_started(ox::utl::to_data(name));
		}
		size_t internal_group_send(
			size_t* reqid,
			cdata_t const& destids, /// group name
			cdata_t const& srcid,
			size_t cmdid,
			cdata_t const& data,
			group_response_struct const& response,
			size_t timeout_ms=-1)
		{
			typedef ox::str::astring_kit strkit;
			bool is_single_destid = 0==strkit::strch<true>(destids.begin,destids.size,',');
			if (is_single_destid)
			{
				ox::utl::good_buffer<char[8+8*4]> temp_buffer;
				cdata_t temp = temp_buffer.allocate(ox::utl::vecbuff_t::minsize(response.bind.bind_data_size+2));
				vecbuff_t* dataindex = vecbuff_t::unsafe_as_vecbuff(temp);
				dataindex->init_abs();
				dataindex->unsafe_add_index_by_data((char const*)&response.event,sizeof(response.event));
				dataindex->unsafe_add_index_by_data((char const*)&response.bind.threadid,sizeof(response.bind.threadid));
				dataindex->unsafe_add_index(response.bind.bind_data_begin,response.bind.bind_data_size);

				response_struct resp_single;
				resp_single.event.assign(this,&self::on_group_single_return);
				resp_single.bind = response.bind;
				resp_single.bind.threadid = -1;
				resp_single.bind.bind_data_begin = (cdata_t*)dataindex->head_begin();
				resp_single.bind.bind_data_size = dataindex->item_count();
				/// send single
				char internal_name[128] = {0};
				cdata_t zero;
				cdata_t dest = destids;
				make_send_indexname(internal_name,dest);
				size_t presize_reserved = vecbuff_t::minsize(1)+sizeof(ipcnode_define::header);/// outer.meta+/*inter.meta.size+*/data0(head).size
				return internal_send(reqid,internal_name,zero,zero,dest,0,cmdid,zero,data,resp_single,timeout_ms,presize_reserved);
			}

			static char const* sep [] = {","};
			strkit::get_line<ox::character::__case_sensitive> get(destids.begin,destids.begin+destids.size);
			get.find.set_seps(sep);
			
			size_t group_seq = result_group::seq_next();
			result_group* item = _m_group_result.create(group_seq);
			item->init_magic();
			item->_m_recv_count = 0;
			item->_m_head.seq = group_seq;
			item->_m_head.commandid = cmdid;
			item->_m_head.bind = response.bind.bind;
			item->_m_mode = response.mode;
			ox::utl::vecbuff_helper(item->_m_binddata).init().add(response.bind.bind_data_begin,response.bind.bind_data_size);
			ox::utl::vecbuff_t& vecbind = ox::utl::vecbuff_helper(item->_m_binddata,0).as_vecbuff();
			item->_m_head.bind_data_begin = vecbind.head_begin();
			item->_m_head.bind_data_size = vecbind.item_count();
			item->_m_response = response;
			item->_m_destid_count = 0;
			item->_m_destid_list.assign(destids.begin,destids.size);

			/// prepare the call
			size_t errid = 0;
			response_struct resp_single;
			resp_single.event.assign(this,&self::on_group_each_return);
			resp_single.bind.threadid = -1;
			resp_single.bind.bind = item;
			char const* line_end = 0;
			char const* line = get(&line_end);
			/// get all destinations
			ox::utl::good_buffer<char[8*sizeof(cdata_t)]> data_buff;
			while (line)
			{
				item->_m_destid_count++;
				cdata_t dest(line,line_end-line);
				if (data_buff.data().size>item->_m_destid_count)
					data_buff.allocate(item->_m_destid_count*sizeof(cdata_t)*2);
				cdata_t* data_begin = reinterpret_cast<cdata_t*>((char*)(data_buff.data().begin));
				*(data_begin+item->_m_destid_count-1) = dest;
				line = get(&line_end);
			}
			item->_m_contents.reserve(item->_m_destid_count);
			item->_m_contents.resize(item->_m_destid_count);
			item->_m_contents_data.reserve(item->_m_destid_count);
			item->_m_contents_data.resize(item->_m_destid_count);
			/// post respectively
			cdata_t* data_begin = reinterpret_cast<cdata_t*>((char*)(data_buff.data().begin));
			for (int i=0;i<item->_m_destid_count;++i)
			{
				int err = post(0,data_begin[i],cmdid,data,resp_single,timeout_ms);
				err?errid=err:0;
			}
			if (reqid) *reqid = _m_base.seq();
			return errid;
		}
		void on_group_single_return(ipcnode_define::header head)
		{
			assert (head.bind_data_size>=2);
			assert (head.bind_data_begin[0].size>=sizeof(ipcnode_define::group_response_d));
			assert (head.bind_data_begin[1].size>=sizeof(size_t));
			group_response_struct response;
			response.event = *(ipcnode_define::group_response_d*)head.bind_data_begin[0].begin;
			response.bind.threadid = *(size_t*)head.bind_data_begin[1].begin;
			response.bind.bind = head.bind;
			response.mode = ipcnode_define::__wait_all;
			ipcnode_define::group_header ghead;
			ghead.bind = head.bind;
			ghead.bind_data_begin = head.bind_data_begin+2;
			ghead.bind_data_size = head.bind_data_size-2;;
			ghead.commandid = head.commandid;
			ghead.seq = 0;
			/// event
			post_group_event(response,ghead,&head,1);
			//post_group_event(response,ghead,vecbuff_outer->data());
		}
		void on_group_each_return(ipcnode_define::header head)
		{
			/// verify is valid or not
			result_group* pitem = (result_group*)(head.bind);
			if (!pitem) return;
			result_group& item = *pitem;
			assert (item.is_init_magic() && item._m_contents.size()==item._m_contents.capacity());
			if (!item.is_init_magic()) return;
			size_t i = item._m_recv_count++;
			if (item._m_recv_count.value()>item._m_recv_count) return;
			if (i>item._m_contents.size()) return;
			size_t item_count = 0;

			/// do according to the policy
			/// one by one, wait first, wait first successful, wait all
			/// serialize the current result
			switch (item._m_mode)
			{
			case ipcnode_define::__wait_all:
				{
					std::string& this_data = item._m_contents_data[i];
					ipcnode_define::header& this_head = item._m_contents[i];
					this_data.assign(head.data.begin,head.data.size);
					this_head = head;
					this_head.data.assign(this_data.data(),this_data.size());
					if (i+1<item._m_destid_count) return;
					item_count = i+1;
				}
				break;
			case ipcnode_define::__wait_first:
				{
					if (i>0) return;
					/// reserve the result
					std::string& this_data = item._m_contents_data[0];
					ipcnode_define::header& this_head = item._m_contents[0];
					this_data.assign(head.data.begin,head.data.size);
					this_head = head;
					this_head.data.assign(this_data.data(),this_data.size());
					item_count = 1;
				}
				break;
			case ipcnode_define::__wait_first_successful:
				{
					std::string& this_data = item._m_contents_data[0];
					ipcnode_define::header& this_head = item._m_contents[0];
					this_data.assign(head.data.begin,head.data.size);
					this_head = head;
					this_head.data.assign(this_data.data(),this_data.size());
					if (head.errorid!=0 && i+1<item._m_destid_count) return;
					item_count = 1;
				}
			case ipcnode_define::__wait_one_by_one:
				{
					item_count = 0;
					assert(false && "not supported at present");
					return;
				}
			default: assert(false); return;
			}
			// get content
			//cdata_t content = content_helper.as_vecbuff().data();
			/// yes, return user
			post_group_event(item._m_response,item._m_head,&item._m_contents[0],item_count);
			/// free the data
			if (i+1>=item._m_destid_count) _m_group_result.destroy(pitem);
		}
		size_t internal_send(
			size_t* reqid,
			char const* index_group_name,
			cdata_t const& srcid,
			cdata_t const& respid,
			cdata_t const& destid,
			size_t channel_number,
			size_t cmdid,
			cdata_t const& data_pre,
			cdata_t const& data,
			response_struct const& response,
			size_t timeout_ms=-1,
			size_t presize_reserved=0,
			bool is_not_back=false)
		{
			assert (index_group_name && !destid.is_empty());
			typedef communication_define::reqid_t reqid_t;
			std::string dest_buf;
			ox::utl::vecbuff_t& db = ox::utl::vecbuff_helper(dest_buf).init().add(destid).as_vecbuff();
			ipc_define::ipc_user_sender_struct iss;
			iss._m_faddr = respid;
			iss._m_daddrs = ox::utl::to_data(dest_buf);
			iss.on_ipc_feedback.assign(this,&self::ipc_send_back_handle);
			iss._m_user_response_struct = response; /// the user event
			iss.cmdid().set_cmdid(cmdid,is_not_back);
			iss.data().assign(data);
			iss._m_binddata_begin = response.bind.bind_data_begin;
			iss._m_binddata_size = response.bind.bind_data_size;

			size_t bind_size = vecbuff_t::size_needed_of_index_and_data(response.bind.bind_data_begin,response.bind.bind_data_size);
			if (bind_size) bind_size += vecbuff_t::size_of_meta();
			ipc_helper_internal::sender_struct ss2(bind_size);
			ss2._m_saddr = (srcid.begin==0) ? ox::utl::to_data(_m_ipc_name) : srcid;
			ss2._m_presize_reserved = presize_reserved;
			ss2.base_struct::is_answer_needed().set(!is_not_back);
			ss2.set(iss);
			ss2.name().set(index_group_name);
			ss2.file_number = channel_number;
			if (timeout_ms!=-1)
			{
				ss2._m_ipc_sender._m_timeout_ms = timeout_ms;
				ss2.response_flag().prepare_event_enabled(true);
				ss2.event_before_write_enabled().set(true);
			}
			reqid_t r = _m_base.send(ss2);
			if (reqid) *reqid = r.seq;
			return r.errorid;
		}

		void send_back(void const* ipc_define_response_struct,cdata_t data)
		{
			assert (ipc_define_response_struct);
			ipc_define::response_struct const* resp = static_cast<ipc_define::response_struct const*>(ipc_define_response_struct);
			cdata_t srcid = resp->protocol->get_source_address();
			cdata_t respid = ox::utl::to_data(_m_ipc_name);
			cdata_t destid = resp->protocol->get_destination_address(0);
			size_t channel_number = resp->base->file_number;
			size_t cmdid = resp->protocol->get_commandid();
			response_struct response;
			size_t timeout = -1;
			cdata_t zero;
			ox::comm::log_proxy::instance().printf("ipc.send_back(cmdid:%u,f:%u)\n",cmdid,channel_number);
			internal_send(0,_m_ipcs_slash,srcid,respid,destid,channel_number,cmdid,zero,data,response,timeout,0,true);
		}

		/// run in dispatch
		bool on_timeout(size_t id,LONGLONG period_micro_second,void const* bind)
		{
			/// the task has not happened
			assert (bind);
			timer_struct* ts = const_cast<timer_struct*>(ipc_helper_internal::get_ipc_timeout(bind));
			communication_define::response_object response_object(bind);
			ipcnode_define::response_struct const* ipcnode_response = ipc_helper_internal::get_ipcnode_response(bind);

			bool is_done = !(ts->set_timeout_return_is_started());
			if (is_done) return false;

			ipcnode_define::header head;
			head.commandid = ts->_m_commandid;
			head.source = ts->_m_source;
			head.errorid = response_object.errorid();
			head.seq = response_object.seq();
			head.step = response_object.step();
			head.bind = response_object.outofbind();
			cdata_t bind_data = ipc_helper_internal::get_ipc_binddata(bind);
			std::string buffer;
			buffer.append(head.source.begin,head.source.size);
			buffer.push_back('\0');
			buffer.append(bind_data.begin,bind_data.size);
			if (head.errorid==0) head.errorid = constant::__error_timeout;
			_m_base.post_task_to_iocp(task_single::make(this,&self::do_show_timeout,head,buffer,*ipcnode_response));
			return false;
		}

		/// run in io thread
		void do_show_timeout(ipcnode_define::header& head,std::string const& buffer,ipcnode_define::response_struct const& response)
		{
			head.source.begin = buffer.data();
			head.bind_data_begin = 0;
			head.bind_data_size = 0;
			cdata_t bind_data(buffer.data()+head.source.size+1,buffer.size()-head.source.size-1);
			if (!bind_data.is_empty())
			{
				vecbuff_t* vecbind = vecbuff_t::as_vecbuff(bind_data);
				assert (vecbind->is_relative_mode());
				head.bind_data_begin = vecbind->head_begin();
				head.bind_data_size = vecbind->item_count();
				vecbind->convert_to_absolute();
			}
			head.data.assign("",0);
			post_event(response,head);
		}

		/// run in dispatch
		void on_no_timeout(timer_struct* timeout)
		{
			if (_m_base.dispatch_thread().threadid()==GetCurrentThreadId())
				do_on_no_timeout(timeout);
			else
				_m_base.dispatch_thread().add(task_single::make(this,&self::do_on_no_timeout,timeout));
		}

		void do_on_no_timeout(timer_struct* timeout)
		{
			assert (_m_base.dispatch_thread().threadid()==GetCurrentThreadId());
			if (timeout->is_definitely_timeout())
				return;
			_m_base.dispatch_thread().unsafe_erase_timer(timeout->_m_pos);
		}

		constant::response_result_enum ipc_send_back_handle(ipc_define::response_struct& resp,ipc_sender_back const&)
		{
			constant::response_result_enum ret = constant::__resp_unfinished;
			bool show_user = false;

			if (resp.base->errorid!=0)
			{
				show_user = true;
				/// if error occurs, erase the timer
				if (resp.timeout._m_period!=-1 && resp.base->step > constant::__step_prepare)
				{
					bool is_started = resp.timeout.set_finished_return_is_started();
					if (is_started) /// before time out
					{
						/// show user error
						//show_user = true;
						on_no_timeout(&resp.timeout);
					}
					else show_user = false;
				}
				ret = constant::__resp_finished_and_close;
			}
			else switch (resp.base->step)
			{
			case constant::__step_prepare:
				{
					/// if the data is ok, if period is infinite, do not
					if (resp.timeout._m_period==-1) break;
					/// or else the period is valid, start
					_m_base.dispatch_thread().add_timer_milli(
						ox::mos::win_timer_list::timer_event_d(this,&self::on_timeout),
						resp.timeout._m_period,
						resp.base->response._m_object,
						-1,
						&resp.timeout._m_pos);
					bool is_null = resp.timeout.set_started_return_is_null();
					assert (is_null);
					cdata_t source = resp.protocol->get_source_address();
					cdata_t bindata = resp.bind_data();
					resp.timeout.set_source_and_binddata(source,bindata);
					resp.timeout._m_commandid = resp.protocol->get_commandid();
					//Sleep(3*1010);
				}
				break;
			case constant::__step_before_write:
				{
					ret = constant::__resp_later; 
					if (resp.timeout._m_period==-1) break;
					if (resp.timeout.is_timeout_before_finished())
						/// the time out event is on the way,
						/// so we do not call on timeout event again
						/// and ignore this action, do result directly
						ret = constant::__resp_finished_and_keepalive;
				}
				break;
			case constant::__step_write:
				{
					/// just for debugging
#ifdef _DEBUG
					cdata_t data = resp.protocol->get_data();
					std::string str(data.begin,data.size);
					log_proxy::instance().printf("ipc.send_back step:write, f:[%d] cmd:[%u] size:%u,data size%u,data:[%s]\r\n",
						resp.base->file_number,
						resp.protocol->get_commandid(),
						resp.protocol->size(),
						data.size,
						str.c_str());
#endif
				}
				break;
			case constant::__step_read:
				{
					show_user = true;
					if (resp.timeout._m_period!=-1) /// has timer
					{
						bool is_started = resp.timeout.set_finished_return_is_started();
						if (is_started)  /// before time out
						{
							/// delete the timer
							on_no_timeout(&resp.timeout);
							/// do the normal handle, default is yes
						}
						else /// after time out, timer has been removed
						{
							show_user = false;
							/// set the error = time out
							/// do not show data to user
						}
					}
					ret = constant::__resp_finished_and_keepalive;
				}
				break;
			}
			if (show_user)
			{
				/// run the user event
				ipcnode_define::header head;
				get_event_param(head,resp);
				post_event(resp._m_user_response_struct,head);
			}
			return ret;
		}
		constant::response_result_enum ipc_server_handle(ipc_define::response_struct& resp,ipc_sender_back const& send)
		{
			if (resp.base->errorid!=0)
			{
				//assert (false);
				printf ("error happens[%u] and close the connection\n",resp.base->errorid);
				return constant::__resp_finished_and_close;
			}
			if (resp.base->step == constant::__step_write)
				return constant::__resp_unfinished;
			if (resp.base->step == constant::__step_write)
			{
				/// for testing
				printf("\twrite: %d bytes\n",resp.base->size);
			}
			else if (resp.base->step == constant::__step_read)
			{
				handle_command(resp,send);
			}
			return constant::__resp_unfinished;
		}
		template <size_t size_tc>
		void make_send_indexname(char (&out)[size_tc],cdata_t const& destid)
		{
			typedef ox::str::astring_kit strkit;
			char* p = out;
			p = strkit::append(p,_m_ipcc);
			p = strkit::append(p,"/");
			p = strkit::append(p,destid.begin,destid.size);
		}
		void make_send_indexname(std::string& out,cdata_t const& destid)
		{
			out.append(_m_ipcc);
			out.append("/");
			out.append(destid.begin,destid.size);
		}
		void do_add_service(size_t cmdid,service_struct const& ipc_service,response_struct const& back_response)
		{
			service_t* pservice = new service_t;
			assert (pservice);
			if (!pservice) return;
			service_t& service = *pservice;
			service.handle = ipc_service;
			service.back_response = back_response;

			typedef ipc_handle_map::iterator I;
			I i = _m_ipc_handles.find(cmdid);
			if (i!=_m_ipc_handles.end())
			{
				classid* idservice = i->second;
				assert (idservice);
				idservice->destroy_self();
			}

			_m_ipc_handles[cmdid] = pservice;
		}
		void do_remove_service(size_t cmdid)
		{
			_m_ipc_handles.erase(cmdid);
		}
		void do_add_listener(size_t cmdid,listener_struct const& listener)
		{
			ipc_multi_listener_d* multi = update_multi_listener(cmdid);
			post_task_d* post_task_finded = _m_multi_post_task.find(listener.bind.postid);
			add_listener(*multi,listener,post_task_finded?*post_task_finded:post_task_d());
		}
		void do_remove_listener(size_t cmdid,ipc_listener_d const& listener)
		{
			typedef ipc_handle_map::iterator I;
			I i = _m_ipc_handles.find(cmdid);
			if (i!=_m_ipc_handles.end()) return;
			listener_t* listener_service = static_cast<listener_t*>(i->second);
			assert (listener_service && listener_service->is_single());
			ipc_multi_listener_d& multi_listener = listener_service->handles;
			remove_listener(multi_listener,listener);
		}
		size_t handle_command(ipc_define::response_struct const& resp,ipc_sender_back const& send_back)
		{
			size_t cmdid = resp.protocol->get_commandid();
			typedef ipc_handle_map::iterator I;
			I i = _m_ipc_handles.find(cmdid_helper(cmdid).get_cmdid());
			if (i==_m_ipc_handles.end())
			{
				char const* notfound = "command not find";
				send_back(ox::utl::to_data(notfound));
				return constant::__error_commandid_not_found;
			}
			classid* id = i->second;
			if (id->is_single())
				handle_listener_command(*static_cast<listener_t*>(id),cmdid,resp,send_back);
			else if (id->is_return())
				handle_service_command(*static_cast<service_t*>(id),cmdid,resp,send_back);
			else assert(false);
			return 0;
		}
		void handle_service_command(
			service_t& service,
			size_t cmdid,
			ipc_define::response_struct const& resp,
			ipc_sender_back const& send_back)
		{
			assert (service.is_return());
			ipcnode_define::header head;
			//cdata_t data;
			get_event_param(head,resp);
			head.bind = service.handle.bind.bind;
			head.bind_data_begin = service.handle.bind.bind_data_begin;
			head.bind_data_size = service.handle.bind.bind_data_size;
			if (service.handle.bind.threadid==-1 || service.post_task.is_empty())
			{
				service.handle.event(head,send_back);
				return;
			}
			std::string buffer(head.data.begin,head.data.size);
			service.post_task(
				task_single::make(&self::on_service_event_redirect,service.handle.event,head,buffer,send_back),
				service.handle.bind.threadid);
		}
		void handle_listener_command(
			listener_t& listener,
			size_t cmdid,
			ipc_define::response_struct const& resp,
			ipc_sender_back const& send_back)
		{
			assert (listener.is_single());
			/*if (cmdid_helper(cmdid).is_anwser_needed_forcelly())*/ send_back("ok"); /// notify the user received command
			std::string buffer;
			typedef ipc_multi_listener_d::iterator I;
			ipc_multi_listener_d& handles = listener.handles;
			for (I i=handles.begin();i!=handles.end();++i)
			{
				ipcnode_define::header head;
				//cdata_t data;
				get_event_param(head,resp);
				head.bind = i->bind.bind;
				head.bind_data_begin = i->bind.bind_data_begin;
				head.bind_data_size = i->bind.bind_data_size;
				//head.data = data;
				if (i->bind.threadid==-1 || i->post_task.is_empty())
				{
					i->event(head);
					continue;
				}
				buffer.assign(head.data.begin,head.data.size);
				i->post_task(
					task_single::make(&self::on_listener_event_redirect,i->event,head,buffer),
					i->bind.threadid);
			}
		}
		static void on_service_event_redirect(ipc_service_d const& event,ipcnode_define::header const& head,std::string const& buffer,ipc_sender_back const& send_back)
		{
			event(head,send_back);
		}
		static void on_listener_event_redirect(ipc_listener_d const& event,ipcnode_define::header const& head,std::string const& buffer)
		{
			event(head);
		}
		ipc_multi_listener_d* update_multi_listener(size_t cmdid)
		{
			typedef ipc_handle_map::iterator I;
			ipc_multi_listener_d* listeners = 0;
			do {
				I i = _m_ipc_handles.find(cmdid);
				if (i==_m_ipc_handles.end())
				{
					listener_t* plistener = new (std::nothrow) listener_t;
					assert (plistener);
					if (!plistener) return 0;
					_m_ipc_handles[cmdid]=plistener;
					listeners = &plistener->handles;
				}
				else
				{
					listener_t* plistener = static_cast<listener_t*>(i->second);
					assert (plistener);
					listeners = &plistener->handles;
				}
			} while (0);
			return listeners;
		}
		static ipc_multi_listener_d::iterator find(ipc_multi_listener_d& multi_listener,ipc_listener_d const& listener)
		{
			typedef ipc_multi_listener_d::iterator J;
			for (J i=multi_listener.begin();i!=multi_listener.end();++i)
				if (i->event == listener)
					return i;
			return multi_listener.end();
		}
		static bool remove_listener(ipc_multi_listener_d& multi_listener,ipc_listener_d const& listener)
		{
			ipc_multi_listener_d::iterator i = find(multi_listener,listener);
			if (i==multi_listener.end()) return false;
			multi_listener.erase(i);
			return true;
		}
		static bool add_listener(ipc_multi_listener_d& multi_listener,listener_struct const& listener,post_task_d const& post_task)
		{
			ipc_multi_listener_d::iterator i = find(multi_listener,listener.event);
			if (i!=multi_listener.end()) return false;
			listener2_struct ls2;
			ls2.event = listener.event;
			ls2.bind = listener.bind;
			ls2.post_task = post_task;
			multi_listener.push_back(ls2);
			return true;
		}
		void clear_all_commands()
		{
			typedef ipc_handle_map::iterator I;
			for (I i=_m_ipc_handles.begin();i!=_m_ipc_handles.end();++i)
				i->second->destroy_self();
			_m_ipc_handles.clear();
		}
		void post_event(response_struct const& response,ipcnode_define::header const& head)
		{
			if (response.bind.postid==-1 || response.bind.threadid==-1)
			{
				if (!response.event.is_empty()) response.event(head);
				return;
			}
			post_task_proxy::instance().post_task(
				response.bind.postid,
				task_single::make(do_response_event,response.event,head,std::string(head.data.begin,head.data.size)),
				response.bind.threadid);
		}
		void post_group_event(group_response_struct const& response,
			ipcnode_define::group_header const& head,
			ipcnode_define::header* contents,size_t content_count)
		{
			if (response.bind.postid==-1 || response.bind.threadid==-1)
			{
				ipcnode_define::group_header* h = (ipcnode_define::group_header*)(&head);
				h->head_count = content_count;
				h->heads = contents;
				if (!response.event.is_empty()) response.event(head);
				return;
			}
			std::string data;
			ox::utl::vecbuff_helper helper(data);
			helper.init();
			for (size_t i=0;i<content_count;++i)
				helper.add_binary(*(contents+i));
			for (size_t i=0;i<content_count;++i)
				helper.add((contents+i)->data);
			post_task_proxy::instance().post_task(
				response.bind.postid,
				task_single::make(do_group_response_event,response.event,head,data),
				response.bind.threadid);
		}
		static void do_response_event(response_d const& event,ipcnode_define::header const& head,std::string const& data)
		{
			ipcnode_define::header* h = (ipcnode_define::header*)(&head);
			h->data.assign(data.data(),data.size());
			event(head);
		}
		static void do_group_response_event(ipcnode_define::group_response_d const& event,ipcnode_define::group_header const& head,std::string const& data)
		{
			vecbuff_t* vecbuff = vecbuff_t::as_vecbuff(cdata_t(data.data(),data.size()));
			size_t count = vecbuff->item_count();
			assert (count%2==0);
			//if (count%2!=0) return;
			count /= 2;
			for (int i=0;i<count;++i)
			{
				ipcnode_define::header* h = (ipcnode_define::header*)(vecbuff->data_item(i).begin);
				h->data = vecbuff->data_item(i+count);
			}
			ipcnode_define::group_header* h = (ipcnode_define::group_header*)(&head);
			h->heads = (ipcnode_define::header*)(vecbuff->data_item(0).begin);
			h->head_count = count;
			event(head);
		}
		void get_event_param(ipcnode_define::header& head,ipc_define::response_struct const& resp)
		{
			head.errorid = resp.base->errorid;
			head.seq = resp.base->seq;
			head.step = resp.base->step;
			head.commandid = resp.protocol->get_commandid();
			head.source = resp.protocol->get_response_address();
			head.bind = resp._m_user_response_struct.bind.bind;
			head.bind_data_begin = 0;
			head.bind_data_size = 0;
			head.data = resp.protocol->get_data();
			cdata_t binddata = resp.bind_data();
			if (binddata.is_empty()) return;
			vecbuff_t* vecbind = vecbuff_t::as_vecbuff(binddata);
			vecbind->convert_to_absolute();
			head.bind_data_begin = vecbind->head_begin();
			head.bind_data_size = vecbind->item_count();
		}
		static ipc::ipc_one_process_node_list& this_process_ipc_list()
		{
			static ipc::ipc_one_process_node_list _static_ipc_node_list;
			return _static_ipc_node_list;
		}
	}; /// end of communication ipc


___namespace2_end();