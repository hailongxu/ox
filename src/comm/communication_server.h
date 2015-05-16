
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
//#include <new.h>
//#define  WIN32_LEAN_AND_MEAN
//#include <WinSock2.h>
//#pragma comment(lib,"ws2_32.lib")

#include "../ox/nsab.h"
#include "atomic_number.h"
#include "server_helper.h"



#pragma once


___namespace2_begin(ox,comm)


template <typename helper_tn>
struct communication_server
{
	typedef helper_tn helper;
	typedef typename helper::reqid_t reqid_t;
	typedef typename helper::response_struct response_struct;
	typedef typename helper::on_error_d on_error_d;
	typedef typename helper::protocol_enum protocol_enum;
	typedef typename helper::sender_struct sender_struct;
	typedef typename helper::receiver_struct receiver_struct;
	typedef typename helper::response_d response_d;
	typedef typename helper::constant constant;
	typedef typename helper::connection_filter_d connection_filter_d;
private:
	typedef communication_server self;
	typedef typename helper::thread_t thread_t;
	typedef typename helper::threads_t threads_t;
	typedef typename helper::task_single task_single;
	typedef typename helper::data_t data_t;
	typedef typename helper::allocator_t allocator_t;
	typedef typename helper::fileinfo_t fileinfo_t;
	typedef typename helper::iodata_t iodata_t;
	typedef typename helper::rw_iodata_t rw_iodata_t;
	typedef typename helper::conn_iodata_t conn_iodata_t;
	typedef typename helper::connection_pool_t connection_pool_t;
	typedef typename helper::connection_struct connection_struct;
	typedef typename helper::request_list_t request_list_t;
	typedef typename helper::iocp_t iocp_t;
	typedef typename helper::file_event_handler_t file_event_handler_t;
	typedef typename helper::file_handle_t file_handle_t;
	typedef typename helper::sender_iodata sender_iodata;
	typedef typename helper::file_error_d file_error_d;
	typedef typename helper::iodata_error_d iodata_error_d;
	typedef typename helper::response_result_enum response_result_enum;
	typedef typename helper::all_exited_d all_exited_d;
	typedef typename helper::post_task_d post_task_d;
	typedef typename helper::system_complete_d system_complete_d;
	typedef abc::server_accept_t server_accept_t;
	typedef abc::file_handle_operation file_handle_operation;


public:
	self():_m_connection_pool(_m_allocator) {}
	void init(server_accept_t const& saccept,file_handle_operation const& handle_operation)
	{
		_m_exit_starting = 0;
		_m_server_accept = saccept;
		_m_file_handle_operation = handle_operation;
		_m_dispatch_thread.on_exit().assign(this,&self::when_dispatch_exited);
		_m_server_accept.on_exit().assign(this,&self::when_connect_exit);
		_m_server_accept.on_connected().assign(this,&self::on_connection_connected);
		_m_on_request_complete.assign(this,&self::on_request_complete);
		/// init the event thread
		_m_file_event_handler._m_file_handle_operation = handle_operation;
		_m_file_event_handler._m_dispatch_thread = &_m_dispatch_thread;
		_m_file_event_handler.on_connect_complete.assign(this,&self::on_connection_return);
		_m_file_event_handler.on_request_complete.assign(this,&self::on_request_complete);
		_m_file_event_handler.on_iodata_error.assign(this,&self::on_iodata_error);
		/// init communication file
		_m_file_handle_operation.init();
		/// init iocp, and the action event handler
		typedef iocp_t::action_t action_t;
		_m_iocp.init(&_m_dispatch_thread,_m_file_handle_operation.last_error);
		_m_iocp.add_handler(constant::__file_connect,
			action_t(&_m_file_event_handler,&file_event_handler_t::on_connection_arrived));
		_m_iocp.add_handler(constant::__file_read,
			action_t(&_m_file_event_handler,&file_event_handler_t::internal_on_read_req));
		_m_iocp.add_handler(constant::__file_write,
			action_t(&_m_file_event_handler,&file_event_handler_t::internal_on_write_req));
		_m_iocp.on_all_exited.add(this,&self::when_iocp_exited);
	}

	void start(char const* server_name)
	{
		assert(server_name && *server_name);
		_m_dispatch_thread.start();
		_m_iocp.start();
		size_t error_code = _m_server_accept.start(server_name);
		if (error_code!=0 && !on_error().is_empty())
			on_error()(server_name,error_code);
	}
	void stop(all_exited_d const& on_exit)
	{
		if (_m_exit_starting.value())
			return;
		_m_exit_starting = 1;
		_m_dispatch_thread.add(task_single::make(this,&self::do_stop,on_exit));
	}

	void lize()
	{
		_m_file_handle_operation.uninit();
	}

	/// should be called before start
	template <typename param_t>
	void set_receiver_param(param_t const& param)
	{
		if (!_m_receiver.extra_destruct.is_empty())
		{
			_m_receiver.extra_destruct((char*)_m_receiver_buffer.data());
			_m_receiver.extra_destruct.clear();
			_m_receiver_buffer.clear();
		}

		_m_receiver_buffer.resize(sizeof(param));
		char* buff = (char*)_m_receiver_buffer.data();
		param_t* prm = new (buff) param_t (param);
		
		_m_receiver = prm->as_receiver_struct();
		//_m_file_event_handler.on_data_arrived = _m_receiver.completed;
	}
	template <typename param_t>
	void set_sender_param(param_t const& param)
	{
		if (!_m_sender.extra_destruct.is_empty())
		{
			_m_sender.extra_destruct((char*)_m_sender_buffer.data());
			_m_sender.extra_destruct.clear();
			_m_sender_buffer.clear();
		}
		_m_sender_buffer.resize(sizeof(param));
		char* buff = (char*)_m_sender_buffer.data();
		param_t* prm = new (buff) param_t (param);
		_m_sender = prm->as_sender_struct();
	}

	void erase(reqid_t const& reqid)
	{
		erase(reqid.seq,reqid.name);
	}
	void erase(size_t seq,file_handle_t handle)
	{
		if (_m_exit_starting.value())
			return;
		/// high priority
		_m_dispatch_thread.add(task_single::make(this,&self::do_erase_request,seq,handle));
	}

	void close_file(file_handle_t handle)
	{
		if (_m_exit_starting.value())
			return;
		_m_dispatch_thread.add(task_single::make(this,&self::do_close_file,handle));
	}

	on_error_d& on_error()
	{
		return _m_file_event_handler.on_error;
	}
	
private:
	void on_connection_connected(size_t errorid,file_handle_t file_handle_accepted,char const* name)
	{
		/// notify the dispatch thread
		fileinfo_t* fi = _m_connection_pool.add(file_handle_accepted);
		fi->file_handle = file_handle_accepted;
		fi->fsend.assign(this,&self::send);
		conn_iodata_t* iodata = _m_connection_pool.make<conn_iodata_t>();
		iodata->_m_file_handle = file_handle_accepted;
		iodata->_m_fileinfo = fi;
		iodata->_m_on_system_complete.assign(&_m_file_event_handler,&file_event_handler_t::on_connect_complete_event);
		_m_iocp.notify_iocp(0,iodata);
		if (iodata->_m_errorid!=0) return;
		_m_iocp.add_filehandle(file_handle_accepted,0);
	}
	void on_connection_return(iodata_t* _iodata)
	{
		do {
			if (_m_exit_starting.value())
				break;
			assert(_iodata->actionid()==constant::__file_connect);
			conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);

			if (iodata->_m_errorid!=0)
			{
				on_connection_error(_iodata);
				break;
			}

			int error_code = 0;

			file_handle_t file_handle = iodata->_m_file_handle;
			if (file_handle_operation::is_handle_invalid(file_handle))
			{
				assert(false && "socket is error [serious error]");
				/// socket becomes error after connections?
				error_code = communication_const::__error_socket_changes_unknownlly;
				break;
			}

			/// tell the dispatch thread, to dispatch again
			receive(iodata->_m_fileinfo);
		} while(0);

		_iodata->destroy_self();
	}
	void do_on_request_complete(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		fileinfo_t* fi = iodata->_m_fileinfo;
		fi->used = constant::__file_idle;
		if (iodata->_m_response_return == constant::__resp_finished_and_keepalive &&
			iodata->_m_errorid == 0)
		{
			dispatch_file(fi);
			fi->erase_iodata_from_runing(iodata->_m_seq);
		}
		else
		{
			if (fi->errorid==0) fi->errorid = iodata->_m_errorid;
			if (fi->errorid==0) fi->errorid = constant::__error_canceled_by_user;
			_m_file_handle_operation.close(iodata->_m_file_handle);
			fi->erase_iodata_from_runing(iodata->_m_seq);
			on_file_error(fi);
			fi->tryto_destroy_self();
		}
	}
	void on_request_complete(iodata_t* _iodata)
	{
		if (::GetCurrentThreadId()==_m_dispatch_thread.threadid())
			return do_on_request_complete(_iodata);
		_m_dispatch_thread.add(task_single::make(this,&self::do_on_request_complete,_iodata));
	}
	void start_to_exit()
	{
		_m_exit_starting = true;
		_m_iocp.stop();
	}
	void do_close_file(file_handle_t handle)
	{
		fileinfo_t* fi = _m_connection_pool.find(handle);
		if (!fi) return;
		file_handle_operation.close(handle);
		on_file_error(fi,size_t(handle));
	}
	void receive(fileinfo_t* fileinfo)
	{
		size_t extra_size = _m_receiver_buffer.size();
		fileinfo_t* fi = fileinfo;
		rw_iodata_t* piodata = _m_connection_pool.make<rw_iodata_t>('r',extra_size);
		if (!piodata)
		{
			assert(false && "new iodata fail");
			return;
		}
		rw_iodata_t& iodata = *piodata;
		if (!_m_receiver.extra_construct.is_empty())
			_m_receiver.extra_construct(iodata.extra_data(),_m_receiver_buffer.data());
		iodata._m_extra_destruct = _m_receiver.extra_destruct;
		iodata._m_complete = _m_receiver.get_response(iodata.extra_data());
		iodata._m_when_request_finished = _m_receiver.default_result;
		iodata._m_fileinfo = fi;
		iodata._m_file_handle = fi->file_handle;
		iodata._m_on_system_complete.assign(&_m_file_event_handler,&file_event_handler_t::recv_for_first_back);
		iodata.init_data_for_recv();
		iodata._m_step = constant::__step_read;
		iodata._m_actionid = constant::__file_read;
		safe_add_iodata(piodata);
	}
	void send(fileinfo_t* fileinfo,data_t data)
	{
		size_t extra_size = _m_sender_buffer.size();
		assert(fileinfo);
		fileinfo_t* fi = fileinfo;
		rw_iodata_t* wiodata = _m_connection_pool.make<rw_iodata_t>('w',extra_size);
		if (!wiodata)
		{
			assert(false && "new iodata fail");
			return;
		}
		rw_iodata_t& iodata = *wiodata;
		if (!_m_sender.extra_construct.is_empty())
			_m_sender.extra_construct(iodata.extra_data(),_m_sender_buffer.data());
		_m_sender.to_binary(iodata._m_buffer,_m_sender_buffer.data(),data);
		iodata._m_extra_destruct = _m_sender.extra_destruct;
		iodata._m_complete = _m_sender.get_response(iodata.extra_data());
		iodata._m_when_request_finished = _m_sender.default_result;
		iodata._m_step = constant::__step_write;
		iodata._m_actionid = constant::__file_write;
		iodata._m_file_handle = fi->file_handle;
		iodata._m_fileinfo = fi;
		iodata._m_on_system_complete.assign(&_m_file_event_handler,&file_event_handler_t::write_to_user);
		iodata.assign_data_for_send();
		safe_add_iodata(wiodata);
	}
	void safe_add_iodata(iodata_t* iodata)
	{
		if (GetCurrentThreadId()==_m_dispatch_thread.threadid())
			return do_add_iodata(iodata);
		_m_dispatch_thread.add(task_single::make(this,&self::do_add_iodata,iodata));
	}
	void dispatch_file(fileinfo_t* fileinfo)
	{
		if (_m_exit_starting.value())
			return;
		typedef helper::list_helper list_helper;
		assert(fileinfo);
		fileinfo_t* fi = fileinfo;
		if (fi->wait.empty()) return;
		iodata_t* iodata = fi->wait.front();
		if (fi->errorid!=0) return;
		if (fi->used==constant::__file_busy && iodata->_m_actionid==constant::__file_write) return;
		fi->running.push_back(iodata);
		fi->wait.pop_front();
		iodata->_m_errorid = sender_iodata(_m_file_handle_operation)(fi->file_handle,iodata);
		if (0 == iodata->_m_errorid)
		{
			if (iodata->_m_actionid==constant::__file_write) fi->used=constant::__file_busy;
			return;
		}
		rw_iodata_t* rwiodata = static_cast<rw_iodata_t*>(iodata);
		_m_file_event_handler.on_rwerror_happened(rwiodata);
	}

	void do_erase_request(size_t seq, file_handle_t handle)
	{
		fileinfo_t* si = _m_connection_pool.find(handle);
		if (!si) return;
		si->erase_iodata_from_wait(seq);
	}
	void do_add_iodata(iodata_t* iodata)
	{
		assert (iodata && iodata->_m_fileinfo);
		iodata->_m_fileinfo->add_iodata(iodata);
		dispatch_file(iodata->_m_fileinfo);
	}

	void do_stop(all_exited_d on_exit)
	{
		on_all_exited = on_exit;
		_m_iocp.stop();
	}

	void when_iocp_exited()
	{
		_m_server_accept.stop();
	}

	void when_connect_exit(void*)
	{
		_m_dispatch_thread.stop_next(); 
	}

	void when_dispatch_exited(thread_t*)
	{
		if (!on_all_exited.is_empty())
			on_all_exited(this);
	}

	void on_connection_error(iodata_t* _iodata)
	{
		typedef typename fileinfo_t::request_list_t request_list_t;
		typedef typename request_list_t::iterator I;

		assert(_iodata->actionid()==constant::__file_connect);
		conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);
		fileinfo_t* fi = iodata->_m_fileinfo;
		assert(fi /*&& fd*/);
		size_t error_code = iodata->_m_errorid;
		internal_on_connection_error(fi,error_code);
	}
	void on_connection_error(connection_struct const& connection,size_t error_code)
	{
		fileinfo_t* fi = _m_connection_pool.find(file_handle_t(connection.name));
		if (!fi) return;
		filedata_t* fd = fi->find_fid(connection.file_number);
		internal_on_connection_error(fi,error_code);
	}
	void internal_on_connection_error(fileinfo_t* fileinfo,size_t error_code)
	{
		typedef typename fileinfo_t::request_list_t request_list_t;
		typedef typename request_list_t::iterator I;

		//assert(_iodata->actionid()==iodata_t::__file_connect);
		fileinfo_t* fi = fileinfo;
		assert(fi);

		_m_file_handle_operation.close(fi->file_handle);
		//fi->erase_file_from_creating(fd);
		//fd = 0;

		/// if no valid file notify the user error with all the wait requests
		//if (fi->dispatchable_file_count()<=0)
		{
			/// notify the global wait to error
			for (I i=fi->wait.begin();i!=fi->wait.end();++i)
			{
				rw_iodata_t* rwiodata = static_cast<rw_iodata_t*>(*i);
				rwiodata->_m_errorid = error_code;
				_m_file_event_handler.notify_event_iodata_error(rwiodata);
			}
			fi->move_wait_to_running_tail();
		}
	}

	void on_iodata_error(iodata_t* piodata)
	{
		assert(piodata && piodata->_m_errorid!=0);
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(piodata);
		iodata->destroy_self();
	}

	void on_file_error(fileinfo_t* fileinfo)
	{
		/// notify each iodata which is error contained in filedata
		assert(fileinfo/*&& filedata->_m_errorid!=0*/);
		fileinfo_t* fi = fileinfo;
		assert(fi->errorid);
		notify_event_reqlist_rwiodata_error(fi->wait,fi->errorid);
		fi->wait.clear();
	}

	void notify_event_reqlist_rwiodata_error(request_list_t& reqlist,size_t error_code)
	{
		typedef request_list_t::iterator I;
		for (I i=reqlist.begin();i!=reqlist.end();++i)
		{
			rw_iodata_t* iodata = static_cast<rw_iodata_t*>(*i);
			iodata->_m_fileinfo = 0;
			iodata->_m_errorid = error_code;
			_m_file_event_handler.notify_event_iodata_error(iodata);
		}
	}
	void notify_event_prepare_error(rw_iodata_t* iodata)
	{
		_m_file_event_handler.notify_event_iodata_error(iodata);
	}

	void notify_event_prepare_error(sender_struct const& sender,size_t errorid)
	{
		do_outter_event_error_at_prepare(sender,errorid);
	}

	void notify_event_prepare_error(char const* name,size_t errorid)
	{
		on_error()(name,errorid);
	}

	void do_outter_event_error_at_prepare(sender_struct sender,size_t errorid)
	{
		response_struct eventdata;
		assign(eventdata,sender,0,errorid);
		sender.completed(eventdata);
	}

	static void assign(response_struct& respdata,sender_struct const& sender,size_t seq,size_t errorid)
	{
		respdata.seq = seq;
		respdata.step = constant::__step_prepare;
		respdata.errorid = errorid;
		respdata.lid = sender.lid;
		respdata.file_number = sender.file_number;
		respdata.data = sender.data.data;
		respdata.size = sender.data.size;
		respdata.buffer_size = sender.data.size;
		respdata.outofband = sender.outofband;
		respdata.outofband2 = sender.outofband2;
	}

public:
	all_exited_d on_all_exited;
	
private:
	ox::mos::atomic_long _m_exit_starting;
	thread_t _m_dispatch_thread;
	server_accept_t _m_server_accept;
	file_handle_operation _m_file_handle_operation;
	iocp_t _m_iocp;
	allocator_t _m_allocator;
	connection_pool_t _m_connection_pool;
	file_event_handler_t _m_file_event_handler;
	system_complete_d _m_on_request_complete;
	system_complete_d _m_system_send;
	receiver_struct _m_receiver;
	std::string _m_receiver_buffer;
	sender_struct _m_sender;
	std::string _m_sender_buffer;
};


___namespace2_end()