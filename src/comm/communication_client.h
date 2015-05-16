
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

#include "windows.h"
#include <string>
//#define  WIN32_LEAN_AND_MEAN
//#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")

#include "../ox/nsab.h"
#include "../mos/atomic_number.h"
#include "client_helper.h"
#include "log_proxy.h"
#include "communication_define.inl.h"


#pragma once


___namespace2_begin(ox,comm)


template <typename helper_tn>
struct communication_client
{
	typedef communication_client self;
	typedef helper_tn helper;
	typedef client_connection<communication_client> connection;
	typedef client_group<communication_client> group_t;
	typedef client_multi_group<group_t> multi_group_t;
	typedef typename helper::allocator_t allocator_t;
	typedef typename helper::cdata_t cdata_t;
	typedef typename helper::reqid_t reqid_t;
	typedef typename helper::response_struct response_struct;
	typedef typename helper::on_error_d on_error_d;
	typedef typename helper::protocol_enum protocol_enum;
	typedef typename helper::option_filemax option_filemax;
	typedef typename helper::option_event_connection_created_successfully option_event_connection_created_successfully;
	typedef typename helper::sender_struct sender_struct;
	typedef typename helper::response_d response_d;
	typedef typename helper::constant constant;
	typedef typename helper::connection_filter_d connection_filter_d;
	typedef typename helper::all_exited_d all_exited_d;
	typedef typename helper::timer_t timer_t;
private:
	typedef typename helper::dispatch_thread_t dispatch_thread_t;
	typedef typename helper::thread_t thread_t;
	typedef typename helper::threads_t threads_t;
	typedef typename helper::task_single task_single;
	typedef typename helper::thread_task_t thread_task_t;
	typedef typename helper::client_connect_t client_connect_t;
	typedef typename helper::server_accept_t server_accept_t;
	typedef typename helper::fileinfo_t fileinfo_t;
	typedef typename helper::filedata_t filedata_t;
	typedef typename helper::iodata_t iodata_t;
	typedef typename helper::rw_iodata_t rw_iodata_t;
	typedef typename helper::conn_iodata_t conn_iodata_t;
	typedef typename helper::task_iodata_t task_iodata_t;
	typedef typename helper::iodata_base_t iodata_base_t;
	typedef typename helper::connection_pool_t connection_pool_t;
	typedef typename helper::connection_struct connection_struct;
	typedef typename helper::file_list_t file_list_t;
	typedef typename helper::random_file_list_t random_file_list_t;
	typedef typename helper::request_list_t request_list_t;
	typedef typename helper::file_dispatchable_enum file_dispatchable_enum;
	typedef typename helper::iocp_t iocp_t;
	typedef typename helper::file_event_handler_t file_event_handler_t;
	typedef typename helper::file_handle_operation file_handle_operation_t;
	typedef typename helper::file_handle_t file_handle_t;
	typedef typename helper::file_error_d file_error_d;
	typedef typename helper::iodata_error_d iodata_error_d;
	typedef typename helper::response_result_enum response_result_enum;
	typedef typename helper::post_task_d post_task_d;
	typedef typename helper::system_complete_d system_complete_d;
	typedef typename helper::connection_info connection_info;
	typedef typename helper::connection_create_d connection_create_d;
	typedef typename helper::connection_created_successfully_d connection_created_successfully_d;
	typedef typename helper::connection_usable_d connection_usable_d;
	typedef typename helper::stream_output_t stream_output_t;
	typedef sender_iodata_tt<iodata_t,rw_iodata_t> sender_iodata;

protected:
	friend class group_t;

public:
	~communication_client()
	{
		assert (!_m_dispatch_thread.is_running());
		if (_m_dispatch_thread.is_running()) DebugBreak();
	}
	self()
		: _m_connection_pool(_m_allocator)
		, _m_multi_group(_m_allocator)
	{}

public:
	void init_param_set_thread_count(int k,int c)
	{
		_m_iocp.set_thread_count(k,c);
	}
	void init_param_set_client_connect(client_connect_t const& cconnet)
	{
		if (!_m_client_connect.start.is_empty())
		{
			assert(!_m_client_connect.stop.is_empty());
			assert(!_m_client_connect.wait.is_empty());
		}
		_m_client_connect = cconnet; 
		_m_client_connect.on_exit().assign(this,&self::when_connect_exit);
	}
	void init_param_set_server_accept(server_accept_t const& saccept,void const* bind)
	{
		if (!_m_server_accept.start.is_empty())
		{
			assert(!_m_server_accept.stop.is_empty());
			assert(!_m_server_accept.wait.is_empty());
		}
		_m_server_accept = saccept;
		_m_server_accept.on_exit().assign(this,&self::when_accept_exit);
		_m_server_accept.on_connected(bind).assign(this,&self::on_connection_accepted);
	}
	void init_param_set_handle_operation(file_handle_operation_t const& file_handle_operation)
	{
		_m_file_handle_operation = file_handle_operation;
	}
	//void init_param_set_server_name(char const* name)
	//{
	//	
	//}
	allocator_t allocator() {return _m_allocator;}
public:
	bool init()
	{
		log_proxy::instance().printf("communication client init\r\n");
		if (_m_exit_starting !=0)
		{
			log_proxy::instance().printf("communication client init not enabled, while at waiting\r\n");
			return false;
		}
		/// the base variable
		_m_dispatch_thread.set_id(1,"dispatch thread");
		_m_dispatch_thread.on_exit().assign(this,&self::when_dispatch_exited);
		_m_connection_pool.get_group().assign(this,&self::get_group);
		_m_on_request_complete.assign(this,&self::on_request_complete);
		_m_on_stream_request_complete.assign(this,&self::on_stream_request_complete);
		/// init the event thread
		_m_file_event_handler._m_file_handle_operation = _m_file_handle_operation;
		_m_file_event_handler._m_dispatch_thread = &_m_dispatch_thread;
		_m_file_event_handler._m_stream_recv.assign(this,&self::stream_recv);
		_m_file_event_handler.on_connect_complete.assign(this,&self::on_connection_return);
		_m_file_event_handler.on_request_complete.assign(this,&self::on_request_complete);
		_m_file_event_handler.on_iodata_delete.assign(this,&self::on_iodata_delete);
		/// init file
		_m_file_handle_operation.init();
		/// init iocp, and the action event handler
		typedef iocp_t::action_t action_t;
		_m_iocp.init(&_m_dispatch_thread,_m_file_handle_operation.last_error);
		_m_iocp.add_handler(constant::__file_error,
			action_t(&_m_file_event_handler,&file_event_handler_t::internal_on_iodata_error));
		_m_iocp.add_handler(constant::__file_connect,
			action_t(&_m_file_event_handler,&file_event_handler_t::on_connection_arrived));
		_m_iocp.add_handler(constant::__file_read,
			action_t(&_m_file_event_handler,&file_event_handler_t::internal_on_read_req));
		_m_iocp.add_handler(constant::__file_write,
			action_t(&_m_file_event_handler,&file_event_handler_t::internal_on_write_req));
		_m_iocp.add_handler(constant::__file_task,
			action_t(iocp_t::on_task));
		_m_iocp.on_all_exited.add(this,&self::when_iocp_exited);
		return true;
	}

	size_t start(cdata_t server_name=cdata_t())
	{
		size_t error_code = 0;
		log_proxy::instance().printf("communication prepare to start\r\n");
		if (_m_exit_starting != 0)
		{
			log_proxy::instance().printf("communication client start not enabled, while at exiting\r\n");
			error_code = constant::__error_start_not_allowed_at_running;
			return error_code;
		}
		_m_exit_starting = 1;
		_m_dispatch_thread.start();
		_m_iocp.start();
		do{
			do{ /// start server
				if (!(!server_name.is_empty() && *server_name.begin)) break;
				if (_m_server_accept.start.is_empty()) break;
				error_code = _m_server_accept.start(server_name);
				log_proxy::instance().printf("communication server [%s] start called\n",_m_server_accept.name());
			} while (0);
			if (error_code!=0/* && !on_error().is_empty()*/)
				break;
				//on_error()(server_name,error_code);
			do { /// start client
				if (_m_client_connect.start.is_empty()) break;
				_m_client_connect.start();
				log_proxy::instance().printf("communication client called\r\n");
			} while (0);
		} while (0);
		//assert (error_code==0);
		if (error_code!=0)
			stop();
		return error_code;
	}
	bool is_started() const
	{
		return _m_exit_starting.value()!=constant::__state_null;
		//return _m_dispatch_thread.is_started();
	}
	bool is_exiting() const
	{
		return _m_exit_starting.value()>=constant::__state_stop_starting;
	}
	//void stop()
	//{
	//	log_proxy::instance().printf("communication client [exit] stop called\r\n");
	//	if (_m_exit_starting.value())
	//		return;
	//	_m_exit_starting = 1;
	//	_m_dispatch_thread.add(task_single::make(this,&self::do_stop));
	//}
	//void wait(size_t timeout_ms=-1)
	//{
	//	_m_exit_starting = 1;
	//	log_proxy::instance().printf("communication client [exit] wait dispatch called\r\n");
	//	assert(::GetCurrentThreadId()!=_m_dispatch_thread.threadid());
	//	//_m_client_connect.wait(timeout_ms);
	//	_m_dispatch_thread.join(timeout_ms);
	//	log_proxy::instance().printf("communication client [exit] wait dispatch successfully\r\n");
	//	_m_exit_starting = 0;
	//}
	void stop(size_t timeout_ms=-1)
	{
		log_proxy::instance().printf("communication client [exit] stop-and-wait called\r\n");
		assert(::GetCurrentThreadId()!=_m_dispatch_thread.threadid());
		if (is_exiting())
		{
			log_proxy::instance().printf("communication client [exit] stop-and-wait has been called already\r\n");
			return;
		}
		_m_exit_starting = 2;
		_m_dispatch_thread.add(task_single::make(this,&self::do_stop));
		log_proxy::instance().printf("communication client [exit] dispatch thread start to be waited\r\n");
		_m_dispatch_thread.join(timeout_ms);
		log_proxy::instance().printf("communication client [exit] stop-and-wait successfully\r\n");
		_m_exit_starting = 0;
	}

	void lize()
	{
		_m_file_handle_operation.uninit();
	}

	void try_post_task(thread_task_t* task)
	{
		if (::GetCurrentThreadId()==_m_dispatch_thread.threadid() && _m_dispatch_thread.is_started())
			return task->run();
		_m_dispatch_thread.add(task);
	}
	void post_task_high(thread_task_t* task)
	{
		_m_dispatch_thread.add_high(task);
	}
	void post_task(thread_task_t* task)
	{
		_m_dispatch_thread.add(task);
	}
	void post_timeout_milli(ox::mos::timer_task_t* task,size_t milli_seconds)
	{
		_m_dispatch_thread.add_timer_milli(task,milli_seconds);
	}

	void post_task_to_iocp(thread_task_t* task)
	{
		assert (task);
		task_iodata_t* task_iodata = _m_allocator._new<task_iodata_t>();
		assert (task_iodata);
		if (!task_iodata) return;
		task_iodata->_m_task = task;
		task_iodata->_m_allocator = &_m_allocator;
		iodata_base_t* iodata_base = static_cast<iodata_base_t*>(task_iodata);
		iodata_t* iodata = static_cast<iodata_t*>(iodata_base);
		_m_iocp.notify_iocp(0,iodata);
	}

	connection create_connection(std::string const& name)
	{
		size_t error_code = 0;
		if (is_exiting())
		{
			error_code = communication_const::__error_start_to_exit_normally;
			do_outter_event_prepare_error(name,error_code);
			return connection(error_code);
		}

		size_t file_number = fileinfo_t::file_number_next();
		_m_dispatch_thread.add(task_single::make(
			this,&self::do_connect,name,file_number,constant::__undispatchable));
		return connection(name,file_number,this);
	}
	//void try_connect(std::string const& name)
	//{
	//	size_t error_code = 0;
	//	if (_m_exit_starting.value())
	//	{
	//		error_code = communication_const::__error_start_to_exit_normally;
	//		do_outter_event_prepare_error(name,error_code);
	//		return;
	//	}
	//	_m_dispatch_thread.add(task_single::make(
	//		this,&self::do_try_connect,name,constant::__undispatchable));
	//}
	void create_stream(char const* name,unsigned char index=0)
	{
		_m_dispatch_thread.add(task_single::make(this,&self::do_add_stream,name,index));
	}
	sender_struct* copy_sender_struct(sender_struct const& sdata)
	{
		size_t size = sizeof(sdata)+sdata.extra_size;
		sender_struct* ss = (sender_struct*)_m_allocator._new(size);
		if (!ss) {assert(false && "create stream fail");return 0;}
		sdata.copy_construct(*ss);
		return ss;
	}
	sender_struct* copy_sender_struct(sender_struct const& sdata,std::string& buf)
	{
		size_t size = sizeof(sdata)+sdata.extra_size;
		buf.resize(size);
		if (buf.size()<size) {assert(false && "create stream fail");return 0;}
		sender_struct* ss = (sender_struct*)(buf.data());
		sdata.copy_construct(*ss);
		return ss;
	}
	void set_recv_index_to_stream(char const* name,size_t index)
	{
		_m_dispatch_thread.add(task_single::make(this,&self::do_set_recv_index_to_stream,name,index));
	}
	bool set_recv_to_stream(char const* name,sender_struct const& sdata,size_t index)
	{
		return set_sendrecv_to_stream(true,name,sdata,index);
	}
	bool set_send_to_stream(char const* name,sender_struct const& sdata,size_t index)
	{
		return set_sendrecv_to_stream(false,name,sdata,index);
	}
	bool set_sendrecv_to_stream(bool isrecv,char const* name,sender_struct const& sdata,size_t index)
	{
		sender_struct* ss = copy_sender_struct(sdata);
		if (!ss) return false;
		_m_dispatch_thread.add(task_single::make(this,&self::do_set_sendrecv_to_stream,isrecv,name,ss,index));
		return true;
	}
	void set_send_back(char const* name,communication_define::send_back_d const& send_back)
	{
		_m_dispatch_thread.add(task_single::make(this,&self::do_set_send_back,name,send_back));
	}
	//void set_set_param_to_stream(char const* name,set_data_to_sender_struct_d const& set_param)
	//{
	//	_m_dispatch_thread.add(task_single::make(this,&self::do_set_set_param_to_stream,name,set_param));
	//}
	void create_package(char const* name)
	{
		_m_dispatch_thread.add(task_single::make(this,&self::do_add_package,name));
	}
	//void close_stream(char const* name)
	//{
	//	_m_dispatch_thread.add(task_single::make(this,&self::do_close_stream,name));
	//}
	//void close_stream(group_t* stream)
	//{
	//	_m_dispatch_thread.add(task_single::make(this,&self::do_close_stream,stream));
	//}
	void set_option(option_filemax const& odata)
	{
		if (is_exiting())
			return;
		_m_dispatch_thread.add(task_single::make(this,&self::do_set_option_filemax,odata));
	}
	void set_option(option_event_connection_created_successfully const& odata)
	{
		if (is_exiting())
			return;
		_m_dispatch_thread.add(task_single::make(this,&self::do_set_option_on_connection_created_successfully,odata));
	}
	size_t seq() const
	{
		return iodata_t::seq_next();
	}
	reqid_t send(sender_struct const& sdata)
	{
		reqid_t ret;
		if (is_exiting())
		{
			ret.errorid = communication_const::__error_start_to_exit_normally;
			do_outter_event_prepare_error(sdata,ret.errorid);
			return ret;
		}
		if (sdata.is_data_null())
		{
			ret.errorid = communication_const::__error_data_sent_is_zero;
			do_outter_event_prepare_error(sdata,ret.errorid);
			return ret;
		}

		size_t file_number = sdata.file_number;
		rw_iodata_t* iodata = _m_connection_pool.make<rw_iodata_t>('w',sdata.extra_size);
		if (!iodata)
		{
			ret.errorid = communication_const::__error_no_enough_memory;
			do_outter_event_prepare_error(sdata,ret.errorid);
			return ret;
		}
		if (!sdata.extra_construct.is_empty())
			sdata.extra_construct(iodata->extra_data(),sdata.extra_size,sdata.extra_param);
		iodata->_m_extra_destruct = sdata.extra_destruct;
		iodata->_m_protocol = sdata.protocol;
		iodata->_m_complete = sdata.completed;
		iodata->_m_outofband = sdata.outofband;
		iodata->_m_name = sdata._m_name;
		iodata->_m_file_number = sdata.file_number;
		iodata->_m_flag = sdata._m_flag;
		iodata->_m_buffersize_for_sendrecving = sdata.buffer_size_for_sendrecving;
		iodata->_m_request_complete = &_m_on_request_complete;
		sdata.to_binary(iodata->_m_buffer);
		iodata->assign_data_for_send();
		if (iodata->response_flag().is_prepare_event_enabled())
			notify_event_prepare(iodata);
		_m_dispatch_thread.add(task_single::make(this,&self::do_send,iodata));
		strcpy(ret.name,sdata._m_name);
		ret.seq = iodata->_m_seq;
		return ret;
	}
	void erase(reqid_t const& reqid)
	{
		erase(reqid.seq,reqid.name);
	}
	void erase(size_t seq,char const* name)
	{
		if (is_exiting())
			return;
		_m_dispatch_thread.add(task_single::make(this,&self::do_erase_request,seq,name));
	}
	void notify_request_result(reqid_t const& reqid)
	{
		notify_request_result(reqid.seq,reqid.name);
	}
	void notify_request_result(size_t seq,char const* name)
	{
		if (is_exiting())
			return;
		_m_dispatch_thread.add(task_single::make(this,&self::do_notify_request_result,seq,name));
	}

	void close_index(std::string const& name)
	{
		if (is_exiting())
			return;
		_m_dispatch_thread.add(task_single::make(this,&self::do_close_index,name));
	}
	void close_file(size_t file_number,std::string const& name)
	{
		if (is_exiting())
			return;
		_m_dispatch_thread.add(task_single::make(this,&self::do_close_file,name,file_number));
	}
	on_error_d& on_error()
	{
		return _m_file_event_handler.on_error;
	}
	connection_create_d& on_connection_before_create()
	{
		return _m_on_connection_before_create;
	}
	connection_create_d& on_connection_after_create()
	{
		return _m_on_connection_after_create;
	}
	connection_created_successfully_d& on_connection_created_successfully()
	{
		return _m_on_physical_connection_created_successfully;
	}
	all_exited_d& on_exited()
	{
		return _m_on_all_exited;
	}

private:
	void on_connection_prepared_notified(connection_struct const& connection,bool bprepared)
	{
		if (is_exiting()) return;
		if (::GetCurrentThreadId()==_m_dispatch_thread.threadid())
			return do_connection_result_notified(connection,bprepared);
		_m_dispatch_thread.add(task_single::make(this,&self::do_connection_result_notified,connection,bprepared));
	}
	void on_connection_return(iodata_t* _iodata)
	{
		do {
			if (is_exiting())
				break;
			assert(_iodata->actionid()==constant::__file_connect);
			conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);


			size_t file_number = iodata->_m_file_number;
			fileinfo_t* fi = iodata->_m_fileinfo;
			filedata_t* fd = iodata->_m_filedata; /// in creating
			assert(fi && fd);
			assert (fi->find_filedata(file_number)==fd);
			if (iodata->_m_errorid!=0/* || !fd*/)
			{
				//if (!fd && iodata->_m_errorid==0)
				//	iodata->_m_errorid = constant::__error_connection_deleted;
				on_connection_error(_iodata);
				break;
			}

			int error_code = 0;

			file_handle_t file_handle = iodata->_m_file_handle;
			if (file_handle==file_handle_operation_t::invalid_file_handle())
			{
				assert(false && "socket is error [serious error]");
				/// do something
				/// remove the req from the req-q
				/// socket becomes error after connections?
				error_code = communication_const::__error_socket_changes_unknownlly;
				break;
			}

			/// whether we find the server
			fd->life = constant::__file_created;
			/// send the cache data
			/// we can send the socket first, and then send others
			group_t* group = fi->group;
			fd->used = constant::__file_idle;
			if (_m_on_physical_connection_created_successfully.is_empty())
			{
				if (iodata->_m_dispatchable==constant::__dispatchable)
					fi->add_idle(fd);
				if (!group->on_connection_usable.is_empty())
					group->on_connection_usable(fd);
				fd->dispatch();
				break;
			}
			/// user do with the connection first
			if (iodata->_m_dispatchable==constant::__dispatchable)
				fi->add_suspend(fd);
			connection_struct connection;
			connection.name = fd->fileinfo->name;
			connection.file_number = fd->_m_file_number;
			connection_filter_d connection_prepared(this,&self::on_connection_prepared_notified);
			bool suspended = _m_on_physical_connection_created_successfully(this,connection,connection_prepared);
			if (!suspended)
			{
				if (iodata->_m_dispatchable==constant::__dispatchable)
					fi->move_suspend_to_idle(fd);
				if (!group->on_connection_usable.is_empty())
					group->on_connection_usable(fd);
				fd->dispatch();
				break;
			}
			fd = fi->find_filedata(file_number);
			assert (!fd || fd==iodata->_m_filedata);
			if (!fd || group->on_connection_usable.is_empty()) break;
			//group->on_connection_usable(fd);
		} while(0);

		_iodata->destroy_self();
	}

	void do_set_option_filemax(option_filemax ostruct)
	{
		size_t error_code = 0;
		fileinfo_t* fi = _m_connection_pool.add(ostruct.name,error_code,ostruct.line_max);
		assert(error_code==0 && fi && "create file info fail");
	}
	void do_set_option_on_connection_created_successfully(option_event_connection_created_successfully ostruct)
	{
		_m_on_physical_connection_created_successfully = ostruct.on_connection_created_successfully;
	}
	void do_connect(std::string const& name,size_t file_number,file_dispatchable_enum dispatchable)
	{
		size_t error_code = 0;
		fileinfo_t* fi = _m_connection_pool.add(name,error_code,-1);
		if (!fi)
		{
			do_outter_event_prepare_error(name,error_code);
			return;
		}
		do_connect_file(fi,file_number,dispatchable);
	}
	void do_connect_dispatable_file(fileinfo_t* fi)
	{
		do_connect_file(fi,fi->file_number_next(),constant::__dispatchable);
	}
	void do_connect_file(fileinfo_t* fileinfo,size_t file_number,file_dispatchable_enum dispatchable)
	{
		assert(dispatchable==constant::__dispatchable || dispatchable==constant::__undispatchable);
		filedata_t* fd = fileinfo->add_filedata(file_number);
		/// ok, we can create one, and return
		assert(fd && fd->life==constant::__file_unknown);
		fd->life = constant::__file_creating;
		if (dispatchable==constant::__dispatchable)
		{
			fileinfo->file_creating.push_back(fd);
			fd->_m_dispatch.assign(this,&self::dispatch_free);
		}
		else
		{
			fd->_m_dispatch.assign(this,&self::dispatch_direct);
		}

		conn_iodata_t* iodata = fd->make<conn_iodata_t>();
		assert(iodata && "create iodata fail");
		iodata->_m_filedata = fd;
		assert (!fileinfo->name.is_empty() && 0==*fileinfo->name.end());
		iodata->_m_name.assign(fileinfo->name.begin,fileinfo->name.size);
		iodata->_m_dispatchable = dispatchable;
		iodata->_m_file_number = fd->_m_file_number;
		iodata->_m_file_handle = fd->file_handle;
		iodata->_m_fileinfo = fd->fileinfo;
		//_m_iocp.add_filehandle((HANDLE)fd->file_handle,fd);
		iodata->_m_on_system_complete.assign(
			&_m_file_event_handler,&file_event_handler_t::on_connect_complete_event);
		//sender_iodata()(fd->file_handle,iodata);
		//_m_connect_thread.add(task_single::make(
		//	this,&self::internal_do_connect,iodata));
		client_connect_t::file_connected_d _on_connected(this,&self::on_connected);
		if (is_exiting()) return;
		_m_client_connect.connect(ox::utl::to_data(iodata->_m_name),_on_connected,iodata);
	}

	/// this done in dispatch thread
	void do_send(rw_iodata_t* _iodata)
	{
		log_proxy::instance().printf("communication.do_send(seq:%u,fn:%u,name:%s)\r\n",
			_iodata->_m_seq,_iodata->_m_file_number,_iodata->_m_name.c_str());
		fileinfo_t* fi = _m_connection_pool.add(_iodata->_m_name,_iodata->_m_errorid,-1);
		if (!fi)
		{
			notify_event_iodata_error(_iodata);
			return;
		}
		rw_iodata_t& iodata = *_iodata;
		iodata._m_fileinfo = fi;
		iodata._m_actionid = constant::__file_write;
		iodata._m_step = constant::__step_in_cache;
		iodata.message_more().set(fi->group->message_more().is_message_more());

		if (!fi->add_iodata(_iodata))
			return notify_event_iodata_error(_iodata);
		dispatch_task(_iodata);
	}
	void do_recv(rw_iodata_t* _iodata)
	{
		fileinfo_t* fi = _m_connection_pool.find(_iodata->_m_name);
		if (!fi)
		{
			_iodata->_m_errorid = constant::__error_invalid_group_name;
			do_outter_event_prepare_error(_iodata);
			return;
		}
		rw_iodata_t& iodata = *_iodata;
		iodata._m_fileinfo = fi;
		iodata._m_actionid = constant::__file_read;
		iodata._m_step = constant::__step_in_cache;
		iodata->_m_step = constant::__step_read;
		//_m_file_event_handler.start_to_recv_first_for_stream(iodata);
	}
	void do_close_index(std::string const& name)
	{
		fileinfo_t* fi = _m_connection_pool.find(name);
		if (!fi) return;
		fi->file_max_count = 0; /// can not make new files
		fi->close_all_file(_m_file_handle_operation.close);
		do_outter_event_reqlist_rwiodata_error(fi->wait,constant::__error_canceled_by_user);
	}
	void do_close_file(std::string const& name,size_t file_number)
	{
		fileinfo_t* fi = _m_connection_pool.find(name);
		if (!fi) return;
		filedata_t* fd = fi->find_filedata(file_number);
		if (!fd) return;
		_m_file_handle_operation.close(fd->file_handle);
		on_file_error(fi,file_number);
	}
	void do_connection_result_notified(connection_struct const& connection,bool bprepared)
	{
		if (is_exiting())
			return;
		fileinfo_t* fi = _m_connection_pool.find(connection.name);
		if (!fi)
		{
			assert(false && "the key has been erased!");
			return;
		}
		if (!bprepared)
		{
			on_connection_error(connection,constant::__error_connection_prepared_error);
			return;
		}
		fi->add_idle_from_suspend(connection.file_number);
		filedata_t* fd = fi->find_filedata(connection.file_number);
		if (!fd)
		{
			assert(false && "no file number in file info");
			return;
		}
		if (!fi->group->on_connection_usable.is_empty())
			fi->group->on_connection_usable(fd);
		fd->dispatch();
	}
	/// with the error do with
	void post_iodata(filedata_t* filedata,rw_iodata_t* iodata)
	{
		post_iodata(filedata,iodata,filedata->fileinfo->group->on_write_complete);
	}
	void post_iodata(filedata_t* filedata,rw_iodata_t* iodata,system_complete_d const& on_write_complete)
	{
		assert(filedata && iodata);
		iodata->_m_on_system_complete = on_write_complete;
		iodata->_m_filedata = filedata;
		iodata->_m_fileinfo = filedata->fileinfo;
		iodata->_m_file_number = filedata->_m_file_number;
		iodata->_m_file_handle = filedata->file_handle;
		//iodata->_m_step = constant::__step_write;

		/// the step should be processed immediately
		/// should return __resp_later, or else leading the request canceled
		if (iodata->event_before_write().is_enabled())
		{
			iodata->_m_step = constant::__step_before_write;
			//if (!iodata->_m_on_system_complete.is_empty())
			//	iodata->_m_on_system_complete(iodata);
			//if (iodata->_m_response_return!=constant::__resp_later)
			//	return;
			bool is_continue = true;
			do_outter_event_before_write(iodata,is_continue);
			if (!is_continue) return;
		}
		iodata->_m_step = constant::__step_write;
		iodata->_m_errorid = sender_iodata(_m_file_handle_operation)(filedata->file_handle,iodata);
		if (0 == iodata->_m_errorid) return;
		log_proxy::instance().printf("communication.do_send(seq:%u,fn:%u,name:%s)\r\n",
			iodata->_m_seq,iodata->_m_file_number,iodata->_m_name.c_str());
		if (!iodata->_m_on_system_complete.is_empty())
			iodata->_m_on_system_complete(iodata);
	}

	void do_erase_request(size_t seq, std::string const& name)
	{
		fileinfo_t* si = _m_connection_pool.find(name.c_str());
		if (!si)
			return;
		si->erase_iodata_from_wait(seq);
	}
	void do_notify_request_result(size_t seq, std::string const& name)
	{
		fileinfo_t* si = _m_connection_pool.find(name.c_str());
		if (!si)
		{
			assert (false && "connection has been erased or not existed by unknown reason");
			return;
		}
		iodata_t* iodata = si->find_iodata_from_running(seq);
		if (!iodata)
		{
			assert (false && "the req id does not exist");
			return;
		}
		assert (iodata->_m_errorid==0 && "you break the rules: reponse later should trigger under no error happens");
		rw_iodata_t* _iodata = static_cast<rw_iodata_t*>(iodata);
		if (_iodata->_m_response_return != constant::__resp_later)
		{
			assert(false && "current response is not later");
			return;
		}
		_iodata->_m_response_return = constant::__resp_unfinished;
		assert (iodata->_m_step==constant::__step_write || iodata->_m_step==constant::__step_read);
		if (iodata->_m_step==constant::__step_read)
			_iodata->_m_fileinfo->group->on_read_for_bottom_half(iodata);
		else if (iodata->_m_step==constant::__step_write)
			_iodata->_m_fileinfo->group->on_write_for_bottom_half(iodata);
		else
			assert(false && "not support other phrase at present");
	}

	void do_stop()
	{
		log_proxy::instance().printf("communication client [exit] do_stop called\r\n");
		/// start the connect service and iocp service to stop
		size_t count = 0;
		if (!_m_client_connect.stop.is_empty()) count++;
		if (!_m_server_accept.stop.is_empty()) count++;
		if (_m_iocp.is_started()) count++;
		_m_exit_starting.add_and_return_old(count);
		if (!_m_client_connect.stop.is_empty()) _m_client_connect.stop(1); /// abnormal exit
		if (!_m_server_accept.stop.is_empty()) _m_server_accept.stop();
		if (_m_iocp.is_started()) _m_iocp.astop();
	}

	void when_connect_exit(void*)
	{
		log_proxy::instance().printf("communication client [exit] connection exited all\r\n");
		long v = --_m_exit_starting;
		if (v<=2) _m_dispatch_thread.stop_next();
	}
	void when_accept_exit(void*)
	{
		log_proxy::instance().printf("communication client [exit] accept exited all\r\n");
		long v = --_m_exit_starting;
		if (v<=2) _m_dispatch_thread.stop_next();
	}
	void when_iocp_exited()
	{
		log_proxy::instance().printf("communication client [exit] iocp exited all\r\n");
		long v = --_m_exit_starting;
		if (v<=2) _m_dispatch_thread.stop_next();
	}

	void when_dispatch_exited(thread_t*)
	{
		log_proxy::instance().printf("communication client [exit] dispatch exited all\r\n");
		if (!_m_client_connect.wait.is_empty())
		{
			log_proxy::instance().printf("communication connect [exit] wait called\r\n");
			_m_client_connect.wait(-1);
			log_proxy::instance().printf("communication connect [exit] wait successfully\r\n");
		}
		if (!_m_server_accept.wait.is_empty())
		{
			log_proxy::instance().printf("communication accept [exit] wait called\r\n");
			_m_server_accept.wait(-1);
			log_proxy::instance().printf("communication accept [exit] wait successfully\r\n");
		}
		if (_m_iocp.is_started())
		{
			log_proxy::instance().printf("communication iocp [exit] wait called\r\n");
			_m_iocp.wait(-1);
			log_proxy::instance().printf("communication iocp [exit] wait successfully\r\n");
		}
		_m_connection_pool.clear();
		_m_multi_group.clear();
		if (!_m_on_all_exited.is_empty())
			_m_on_all_exited(this);
		_m_file_handle_operation.uninit();
		log_proxy::instance().printf("communication client [exit] dispatch exited successfully\r\n");
	}

	void on_connection_error(iodata_t* _iodata)
	{
		typedef typename fileinfo_t::request_list_t request_list_t;
		typedef typename request_list_t::iterator I;

		assert(_iodata->actionid()==constant::__file_connect);
		conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);
		filedata_t* fd = iodata->_m_filedata;
		fileinfo_t* fi = fd->fileinfo;
		assert(fi && fd);
		size_t error_code = iodata->_m_errorid;
		internal_on_connection_error(fi,fd,error_code);
	}
	void on_connection_error(connection_struct const& connection,size_t error_code)
	{
		fileinfo_t* fi = _m_connection_pool.find(std::string(connection.name.begin,connection.name.size));
		if (!fi) return;
		filedata_t* fd = fi->find_filedata(connection.file_number);
		internal_on_connection_error(fi,fd,error_code);
	}
	void internal_on_connection_error(fileinfo_t* fileinfo,filedata_t* filedata,size_t error_code)
	{
		typedef typename fileinfo_t::request_list_t request_list_t;
		typedef typename request_list_t::iterator I;
		typedef typename random_file_list_t::iterator J;

		filedata_t* fd = filedata;
		fileinfo_t* fi = fileinfo;
		assert(fi);

		_m_file_handle_operation.close(fd->file_handle);

		/// notify the direct reqlist in current file to error
		if (!fd->_m_wait.empty())
		{
			request_list_t& reqlist = fd->_m_wait;
			do_outter_event_reqlist_rwiodata_error(reqlist,error_code);
			fi->move_list_to_running(reqlist);
		}
		if (!fi->remove_file_from_creating(fd))
			fi->remove_file_from_suspend(fd);
		log_proxy::instance().printf("internal_on_connection_error() erase file %u\r\n",fd->_m_file_number);
		fi->erase_filedata(fd->_m_file_number);

		/// if no valid file notify the user error with all the wait requests
		if (fi->file_count()<=0)
		{
			do_outter_event_reqlist_rwiodata_error(fi->wait,error_code);
			fi->move_list_to_running(fi->wait);
		}
	}

	void on_iodata_delete(iodata_t* piodata)
	{
		log_proxy::instance().printf("on_iodata_delete() seq=%u,fid=%u\r\n",piodata->_m_seq,piodata->_m_file_number);
		assert(piodata);// && piodata->_m_errorid!=0);
		fileinfo_t* fi = piodata->_m_fileinfo;
		fi->erase_iodata_from_runing(piodata);
		if (!fi->is_null()) return;
		assert(fi == _m_connection_pool.find(ox::utl::to_string(fi->name)));
		_m_connection_pool.erase(ox::utl::to_string(fi->name));
	}

	void on_file_error(fileinfo_t* fileinfo,size_t file_number)
	{
		/// notify each iodata which is error contained in filedata
		assert(fileinfo/*&& filedata->_m_errorid!=0*/);
		typedef typename fileinfo_t::request_list_t request_list_t;

		fileinfo_t* fi = fileinfo;
		filedata_t* fd = 0;
		fd = fi->find_filedata(file_number);
		if (!fd)
		{
			if (!fi->remove_filedata_from_busy(file_number))
				fi->remove_filedata_from_idle(file_number);
			return;
		}
		request_list_t& reqlist = fd->_m_wait;
		do_outter_event_reqlist_rwiodata_error(reqlist,-1);
		reqlist.clear();
		if (!fi->remove_filedata_from_busy(file_number))
			fi->remove_filedata_from_idle(file_number);
		log_proxy::instance().printf("on_file_error() erase file %u\r\n",file_number);
		fi->erase_filedata(file_number);
	}
	void on_request_complete(iodata_t* _iodata)
	{
		_m_dispatch_thread.add_high(task_single::make(this,&self::do_on_request_complete,_iodata));
	}
	void on_stream_request_complete(iodata_t* _iodata)
	{
		_m_dispatch_thread.add_high(task_single::make(this,&self::do_on_request_complete,_iodata));
	}
	void do_on_request_complete(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		fileinfo_t* fi = iodata->_m_fileinfo;
		size_t file_number = iodata->_m_file_number;
		file_handle_t file_handle = iodata->_m_file_handle; 
		size_t error_code = iodata->_m_errorid;
		bool is_dispatchable = iodata->dispatchable().is_enabled();

		log_proxy::instance().printf("do_on_request_complete() seq=%u,act:%u,err=%u,fid=%u\r\n",iodata->_m_seq,iodata->_m_actionid,iodata->_m_errorid,iodata->_m_file_number);

		bool is_successfully = error_code==0 && iodata->_m_response_return==constant::__resp_finished_and_keepalive;
		while (is_successfully)
		{
			filedata_t* fd = fi->find_filedata(file_number);
			if (!fd)
			{
				assert (false && "connection has been deleted");
				log_proxy::instance().printf("do_on_request_complete() can not find fid=%u fh=%p\r\n",file_number,file_handle);
				iodata->_m_errorid = constant::__error_connection_deleted;
				is_successfully = false;
				break;
			}
			log_proxy::instance().printf("do_on_request_complete() dispatch fid=%u fh=%p\r\n",file_number,file_handle);
			fi->return_filedata(fd);
			fd->used = constant::__file_idle;
			if (is_dispatchable) fd->dispatch();
			break;
		}
		if (!is_successfully)
		{
			log_proxy::instance().printf("do_on_request_complete() error fid=%u fh=%p\r\n",file_number,file_handle);
			_m_file_handle_operation.close(file_handle);
			on_file_error(fi,file_number);
			if (is_dispatchable) dispatch_free_fileinfo(fi);
		}
		on_iodata_delete(iodata);
	}

	void do_outter_event_reqlist_rwiodata_error(request_list_t& reqlist,size_t error_code)
	{
		_m_file_event_handler.notify_event_reqlist_rwiodata_delete(reqlist,error_code);
	}
	void do_outter_event_prepare_error(rw_iodata_t* iodata)
	{
		_m_file_event_handler.notify_event_iodata_delete(iodata);
	}
	void notify_event_iodata_error(rw_iodata_t* iodata)
	{
		if (iodata->_m_on_system_complete.is_empty())
		{
			iodata->_m_actionid = constant::__file_error;
			iodata->_m_on_system_complete.assign(&_m_file_event_handler,&file_event_handler_t::internal_on_iodata_error);
		}
		_m_iocp.notify_iocp(0,iodata);
	}
	void notify_event_prepare(rw_iodata_t* iodata)
	{
		_m_file_event_handler.notify_event_iodata(iodata);
	}

	void do_outter_event_prepare_error(sender_struct const& sender,size_t errorid)
	{
		do_outter_event_error_at_prepare(sender,errorid);
	}

	void do_outter_event_prepare_error(std::string const& name,size_t errorid)
	{
		on_error()(ox::utl::to_data(name),errorid);
	}

	void do_outter_event_error_at_prepare(sender_struct sender,size_t errorid)
	{
		response_struct eventdata;
		assign(eventdata,sender,0,errorid);
		sender.completed(eventdata);
	}
	void do_outter_event_before_write(rw_iodata_t* iodata, bool& is_continue)
	{
		is_continue = true;
		_m_file_event_handler.notify_event_iodata(iodata);
		switch (iodata->_m_response_return)
		{
		case constant::__resp_unfinished: break;
		case constant::__resp_finished_and_keepalive:
		case constant::__resp_finished_and_close:
			is_continue = false;
			do_on_request_complete(iodata);
			break;
		case constant::__resp_later:
			is_continue = false;
			break;
		default: assert(false && "invalid resp result at event before write");
		}
	}

	static void assign(response_struct& respdata,sender_struct const& sender,size_t seq,size_t errorid)
	{
		respdata.seq = seq;
		respdata.step = constant::__step_prepare;
		respdata.errorid = errorid;
		respdata.name = sender._m_name;
		respdata.file_number = sender.file_number;
		respdata.data = sender.data.begin;
		respdata.size = sender.data.size;
		respdata.buffer_size = sender.data.size;
		respdata.outofband = sender.outofband;
	}

	void internal_do_connect(iodata_t* _iodata)
	{
		//assert(_iodata->actionid()==iodata_t::__file_connect);
		//conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);
		//locationid localid = helper::locationid_helper::make("127.0.0.1",0);
		//fileop_t::bind(fd->file_handle,localid);
		//iodata->_m_errorid = fileop_t::connect(iodata->_m_file_handle,iodata->_m_lid);
		//_m_iocp.notify_iocp(0,iodata);
		//if (iodata->_m_errorid!=0) return;
		//_m_iocp.add_filehandle((HANDLE)iodata->_m_file_handle,iodata->_m_filedata);
	}

	void on_connection_accepted(size_t errorid,file_handle_t file_handle_accepted,char const* name,void const* bind)
	{
		char const* sgname = (char const*)bind;
		fileinfo_t* fi = _m_connection_pool.add(sgname,errorid,-1);
		if (!fi)
		{
			do_outter_event_prepare_error(sgname,errorid);
			return;
		}
		size_t file_number = fileinfo_t::file_number_next();
		filedata_t* fd = fi->add_filedata(file_number);
		assert(fd && fd->life==constant::__file_unknown);
		fd->life = constant::__file_creating; /// for server, not any help
		conn_iodata_t* iodata = fd->make<conn_iodata_t>();
		assert(iodata && "create iodata fail");
		iodata->_m_filedata = fd;
		iodata->_m_name = name;
		iodata->_m_dispatchable = constant::__dispatchable;
		iodata->_m_file_number = fd->_m_file_number;
		iodata->_m_file_handle = fd->file_handle;
		iodata->_m_fileinfo = fd->fileinfo;
		iodata->_m_on_system_complete.assign(
			&_m_file_event_handler,&file_event_handler_t::on_connect_complete_event);

		connection_info coninf;
		coninf.errorid = errorid;
		coninf.file_handle = file_handle_accepted;
		coninf.bind = iodata;
		on_connected(coninf);
	}

	void on_connected(connection_info const& conninfo)
	{
		log_proxy::instance().printf("on_connected(),err:%u fh:%p\r\n",conninfo.errorid,conninfo.file_handle);
		conn_iodata_t* iodata = static_cast<conn_iodata_t*>(conninfo.bind);
		if (!_m_on_connection_after_create.is_empty())
		{
			connection_struct cs;
			cs.name = conninfo.name;
			cs.file_number = iodata->_m_file_number;
			_m_on_connection_after_create(this,cs);
		}
		iodata->_m_errorid = conninfo.errorid;
		iodata->_m_file_handle = conninfo.file_handle;
		iodata->_m_filedata->file_handle = conninfo.file_handle;
		if (is_exiting()) return;
		size_t errorid = iodata->_m_errorid;
		file_handle_t file_handle = iodata->_m_file_handle;
		filedata_t* filedata = iodata->_m_filedata;
		post_task_high(task_single::make(this,&self::on_connection_return,iodata));
		if (errorid!=0) return;
		_m_iocp.add_filehandle(file_handle,filedata);
	}

	void dispatch_task(rw_iodata_t* _iodata)
	{
		assert(_iodata);
		filedata_t* fd = _iodata->_m_filedata;
		fileinfo_t* fi = _iodata->_m_fileinfo;
		/// dispatch as free
		if (fd==0) return dispatch_free_fileinfo(fi);
		/// dispatch as direct
		if (fd->life==constant::__file_creating||fd->life==constant::__file_unknown)
			return;
		dispatch_direct(fd);
	}

	void dispatch_free_fileinfo(fileinfo_t* fileinfo)
	{
		typedef file_list_t::iterator I;
		typedef helper::list_helper list_helper;
		assert(fileinfo);
		fileinfo_t* fi = fileinfo;
		request_list_t &wait = fi->wait;
		file_list_t& idle_list = fi->file_idle;
		while(!idle_list.empty())
		{
			filedata_t* fd = idle_list.front();
			dispatch_free(fd);
			if (wait.empty()) break;
		} /// end of for each idle fd

		if (!fi->file_idle.empty()) return;
		if (fi->wait.empty()) return;
		typedef request_list_t::iterator K;
		for (K i=wait.begin();i!=wait.end();++i)
		{
			iodata_t* piodata = wait.front();
			rw_iodata_t* iodata = static_cast<rw_iodata_t*>(piodata);
			assert(iodata->_m_file_number==0);
			if (fi->can_add_file())
				do_connect_dispatable_file(fi);
		}
	}
	void dispatch_free(filedata_t* filedata)
	{
		assert(filedata);
		filedata_t* fd = filedata;
		fileinfo_t* fi = filedata->fileinfo;
		assert(fi);
		if (fi->wait.empty()) return;
		/// ok, we find a global waiting
		/// get a waiting
		iodata_t* piodata = fi->wait.front();
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(piodata);
		/// if the global waiting front is a normal waiting
		assert(iodata->_m_file_number==0);
		if (iodata->_m_file_number==0)
		{
			fi->running.push_back(iodata);
			fi->wait.pop_front();
			fi->move_idle_to_busy(fd);
			post_iodata(fd,iodata);
		}
		return;
	}
	void dispatch_direct(filedata_t* filedata)
	{
		if (!filedata)
		{
			assert(false && "no file number");
			return;
		}
		filedata_t* fd = filedata;
		fileinfo_t* fi = filedata->fileinfo;
		if (fd->used==constant::__file_busy)
			return;
		request_list_t& wait = fd->_m_wait;
		if (wait.empty())
			return;
		iodata_t* piodata = wait.front();
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(piodata);
		
		assert(iodata->_m_file_number!=0);
		fi->running.push_back(iodata);
		wait.pop_front();
		post_iodata(fd,iodata,fi->group->on_write_complete_for_independence);
	}
	void on_stream_connection_usable(filedata_t* filedata)
	{
		group_t* stream = filedata->fileinfo->group;
		stream_recv(filedata,stream->output_first().value());
	}
	void stream_recv(filedata_t* filedata,size_t output_index)
	{
		log_proxy::instance().printf("communication.stream_recv\r\n");
		group_t* stream = filedata->fileinfo->group;
		sender_struct* psdata = stream->get_recv(output_index);
		assert (psdata);
		sender_struct& sdata = *psdata;

		rw_iodata_t* iodata = _m_connection_pool.make<rw_iodata_t>('r',sdata.extra_size);
		if (!iodata)
		{
			size_t error_code = communication_const::__error_no_enough_memory;
			do_outter_event_prepare_error(sdata,error_code);
			return;
		}
		if (!sdata.extra_construct.is_empty())
			sdata.extra_construct(iodata->extra_data(),sdata.extra_size,sdata.extra_param);
		iodata->_m_extra_destruct = sdata.extra_destruct;
		iodata->_m_protocol = sdata.protocol;
		iodata->_m_outofband = sdata.outofband;
		//iodata->_m_outofband = sdata.outofband2;
		iodata->_m_complete = sdata.completed;
		//sdata.completed(response_struct());
		iodata->_m_buffersize_for_sendrecving = sdata.buffer_size_for_sendrecving;
		iodata->_m_file_number = filedata->_m_file_number;
		iodata->_m_file_handle = filedata->file_handle;
		iodata->_m_filedata = filedata;
		iodata->_m_fileinfo = filedata->fileinfo;
		assert (!filedata->fileinfo->name.is_empty() && *filedata->fileinfo->name.end()==0);
		ox::utl::assign(iodata->_m_name,filedata->fileinfo->name); /// just showing for user
		iodata->_m_step = constant::__step_read;
		filedata->fileinfo->add_iodata_into_running(iodata);
		_m_file_event_handler.start_to_recv_first_for_stream(iodata);
	}
	void do_add_package(std::string const& name)
	{
		group_t* package = _m_multi_group.add(name);
		if (!package)
		{
			assert(false && "create stream group fail");
			return;
		}
		package->_m_name = name;
		package->_m_communication = this;
		package->on_connection_usable.clear();
		package->message_more().set(false);
		package->on_write_complete_for_independence.assign(&_m_file_event_handler,&file_event_handler_t::on_package_send_complete);
		package->on_write_complete.assign(&_m_file_event_handler,&file_event_handler_t::on_package_send_complete);
		package->on_write_for_bottom_half.assign(&_m_file_event_handler,&file_event_handler_t::do_outter_event_for_package_write_for_bottom_half);
		package->on_read_for_bottom_half.assign(&_m_file_event_handler,&file_event_handler_t::do_outter_event_for_package_read_for_bottom_half);
	}
	void do_add_stream(std::string const& name,unsigned char index)
	{
		group_t* stream = _m_multi_group.add(name);
		if (!stream)
		{
			assert(false && "create stream group fail");
			return;
		}
		stream->_m_name = name;
		stream_output_t(stream->_m_flag).set(index);
		//_m_allocator._delete(stream->_m_recv_param[0]);
		//_m_allocator._delete(stream->_m_recv_param[1]);
		stream->_m_communication = this;
		stream->on_connection_usable.assign(this,&self::on_stream_connection_usable);
		stream->message_more().set(true);
		stream->on_write_complete_for_independence.assign(&_m_file_event_handler,&file_event_handler_t::on_package_send_complete);
		stream->on_write_complete.assign(&_m_file_event_handler,&file_event_handler_t::on_stream_send_complete);
		//stream->on_response = sdata->completed;
		stream->on_write_for_bottom_half.assign(&_m_file_event_handler,&file_event_handler_t::do_outter_event_for_stream_write_for_bottom_half);
		stream->on_read_for_bottom_half.assign(&_m_file_event_handler,&file_event_handler_t::do_outter_event_for_stream_read_for_bottom_half);
	}
	void do_set_recv_index_to_stream(std::string const& name,size_t index)
	{
		group_t* stream = _m_multi_group.find(name);
		if (!stream)
		{
			assert(false && "stream has not been found");
			return;
		}
		assert (index<stream->__recv_output_max);
		stream_output_t(stream->_m_flag).set(index);
	}
	void do_set_sendrecv_to_stream(bool brecv,std::string const& name,sender_struct* sdata,size_t index)
	{
		group_t* stream = _m_multi_group.find(name);
		if (!stream)
		{
			assert(false && "stream has not been found");
			_m_allocator._delete(sdata);
			return;
		}
		if (brecv) stream->set_recv(sdata,index);
		else stream->set_send(sdata,index);
	}
	void do_set_send_back(std::string const& name,communication_define::send_back_d const& send_back)
	{
		group_t* stream = _m_multi_group.find(name);
		if (!stream)
		{
			assert(false && "stream has not been found");
			return;
		}
		stream->send_back().assign(send_back);
	}
	void do_close_stream(std::string const& name) /// will close the stream
	{
		group_t* stream = _m_multi_group.find(name);
		if (!stream) return;
		_m_allocator._delete(stream->_m_recv_param);
		_m_allocator._delete(stream);
		_m_multi_group.erase(name);
	}
	void do_close_stream(group_t* stream) /// will close the stream
	{
		if (!stream) return;
		do_close_stream(stream->_m_name);
	}
	group_t* get_group(char const* name)
	{
		size_t name_len = 0;
		char const* p = strchr(name,'/');
		if (!p) return 0;
		name_len = p-name;
		return _m_multi_group.find(name,name_len);
	}

public:
	dispatch_thread_t& dispatch_thread() {return _m_dispatch_thread;}
	dispatch_thread_t const& dispatch_thread() const {return _m_dispatch_thread;}
private:
	ox::mos::atomic_long _m_exit_starting;///0,null; 1,run; 2: start exiting
	dispatch_thread_t _m_dispatch_thread;
	allocator_t _m_allocator;
	connection_pool_t _m_connection_pool;
	iocp_t _m_iocp;
	client_connect_t _m_client_connect;
	server_accept_t _m_server_accept;
	file_event_handler_t _m_file_event_handler;
	file_handle_operation_t _m_file_handle_operation;
	system_complete_d _m_on_request_complete;
	system_complete_d _m_on_stream_request_complete;
	all_exited_d _m_on_all_exited;
	connection_create_d _m_on_connection_before_create;
	connection_create_d _m_on_connection_after_create;
	connection_created_successfully_d _m_on_physical_connection_created_successfully;
	multi_group_t _m_multi_group;
};


___namespace2_end()