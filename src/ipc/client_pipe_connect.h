

#include "windows.h"
#include <assert.h>
#include <string>

#include "../ox/nsab.h"
#include "../../ox/pkg/string_simple_kit.h"
#include "../../ox/utl/seplist.h"
#include "../../ox/mos/win_queue_thread.h"
#include "../../ox/comm/communication_const.h"
#include "../../ox/comm/file_service_operation.h"
#include "file_pipe_operation.h"


#pragma once


___namespace2_begin(ox,ipc)

/*
threadop::thread_t
threadop::task_feedback
task_feedback::complete_d
*/
//struct connect_pipe_trait
//{
//	typedef ox::utl::cdata_t cdata_t;
//	typedef ox::mos::win_queue_thread thread_t;
//	typedef file_pipe_operation fileop;
//	typedef fileop::file_handle_t file_handle_t;
//	typedef ox::comm::client_connect_t::file_connected_d file_connected_d;
//	typedef ox::comm::client_connect_t::file_before_connected_d file_before_connected_d;
//	typedef ox::comm::client_connect_t::exited_d exited_d;
//	typedef ox::comm::client_connect_t::connection_info connection_info;
//	typedef ox::comm::client_connect_t::connect_error_list_d connect_error_list_d;
//};

//template <typename connection_trait>
struct client_pipe_connect
{
	typedef client_pipe_connect self;
	typedef ox::utl::cdata_t cdata_t;
	typedef ox::mos::win_queue_thread thread_t;
	typedef file_pipe_operation fileop;
	typedef fileop::file_handle_t file_handle_t;
	typedef ox::comm::client_connect_t::file_connected_d file_connected_d;
	typedef ox::comm::client_connect_t::file_before_connected_d file_before_connected_d;
	typedef ox::comm::client_connect_t::exited_d exited_d;
	typedef ox::comm::client_connect_t::connection_info connection_info;
	typedef ox::comm::client_connect_t::connect_error_list_d connect_error_list_d;

	typedef ox::str::string_simple_kit<char> strkit;
	typedef std::vector<connection_info> connection_list;
	typedef strkit::foreach_line<ox::character::__case_sensitive> foreach_line;

public:
	client_pipe_connect()
	{
		static char const* sep[1] = {","};
		_m_foreach._m_get_line.find.set_seps(sep);
	}
	void start()
	{
		_m_thread.on_exit().assign(this,&self::on_thread_exited);
		_m_thread.start();
	}
	void stop(int mode)
	{
		if (!_m_thread.is_started())
		{
			on_thread_exited(0);
			return;
		}
		_m_thread.stop_next();
	}
	void wait(size_t timeout_ms)
	{
		assert(GetCurrentThreadId()!=_m_thread.threadid());
		_m_thread.join(timeout_ms);
	}
	void connect(cdata_t name,file_before_connected_d const&,file_connected_d const& on_file_connected,OVERLAPPED* overlapped,void* bind)
	{
		_m_thread.add(ox::mos::thread_task_helper::make(this,&self::do_connect,name,on_file_connected,bind));
	}
	exited_d& on_exit()
	{
		return _m_on_exited;
	}
	comm::client_connect_t as_client_connect()
	{
		comm::client_connect_t result;
		result.start.assign(this,&self::start);
		result.stop.assign(this,&self::stop);
		result.wait.assign(this,&self::wait);
		result.connect.assign(this,&self::connect);
		result.on_exit.assign(this,&self::on_exit);
		return result;
	}
	connect_error_list_d& on_error()
	{
		return _m_error_list_event;
	}
private:
	void do_connect(cdata_t const& name,file_connected_d const& on_file_connected,void* bind)
	{
		do_connect_many(name,on_file_connected,bind);
		return;
		do_connect_single(name,on_file_connected,bind);
	}
	void do_connect_single(cdata_t const& name,file_connected_d const& on_file_connected,void* bind)
	{
		typedef ox::str::string_simple_kit<char> strkit;

		size_t error_code = 0;
		char const* conn_name = strkit::strrch<true>(name.rbegin(),name.rend(),'/');
		(conn_name==0) ? (conn_name=name.begin) : conn_name++;
		file_handle_t file_handle = fileop::connect(conn_name,&error_code);
		connection_info conninfo;
		conninfo.errorid = error_code;
		conninfo.file_handle = file_handle;
		conninfo.name = name;
		conninfo.bind = bind;
		on_file_connected(conninfo);
	}
	struct each_handle
	{
		each_handle(connection_info& info,connection_list& errorlist)
			:conninfo(info),_m_error_list(errorlist)
		{}
		bool operator()(char const* b,char const* e)
		{
			size_t error_code = 0;
			char const* conn_name = strkit::strrch<true>(e-1,b-1,'/');
			(conn_name==0) ? (conn_name=b) : conn_name++;
			file_handle_t file_handle = fileop::connect(conn_name,&error_code);
			conninfo.errorid = error_code;
			if (0==error_code)
			{
				conninfo.file_handle = file_handle;
				conninfo.name.assign(conn_name,e-conn_name);
				return false;
			}
			connection_info conn_err;
			conn_err.errorid = error_code;
			conn_err.name.assign(conn_name,e-conn_name);
			conn_err.bind = conninfo.bind;
			_m_error_list.push_back(conn_err);
			return true;
		}
		connection_info& conninfo;
		connection_list& _m_error_list;
	};

	void do_connect_many(cdata_t const& name,file_connected_d const& on_file_connected,void* bind)
	{
		_m_error_list.clear();
		connection_info conninfo;
		conninfo.bind = bind;
		each_handle handle(conninfo,_m_error_list);
		_m_foreach(name.begin,name.end(),foreach_line::line_handle(&handle));
		on_file_connected(conninfo);
		if (_m_error_list_event.is_empty() || _m_error_list.empty()) return;
		_m_error_list_event(&_m_error_list.front(),_m_error_list.size());
	}
	void on_thread_exited(thread_t*)
	{
		if (_m_on_exited.is_empty())
			return;
		_m_on_exited(this);
	}
private:
	thread_t _m_thread;
	exited_d _m_on_exited;
	foreach_line _m_foreach;
	connection_list _m_error_list;
	connect_error_list_d _m_error_list_event;
};

___namespace2_end()