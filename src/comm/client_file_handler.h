

#include <assert.h>
#include "../cxx/delegate.h"
#include "../mos/task_feedback.h"
#include "sender_iodata.h"

#pragma once


___namespace2_begin(ox,comm)


template <typename handler_trait>
struct client_file_handler /// the name is not suitable
{
	typedef client_file_handler self;
	typedef typename handler_trait::thread_t thread_t;
	typedef typename handler_trait::task_single task_single;
	typedef typename handler_trait::constant constant;
	typedef typename handler_trait::file_handle_operation file_handle_operation;
	typedef typename handler_trait::filedata_t filedata_t;
	typedef typename handler_trait::fileinfo_t fileinfo_t;
	typedef typename handler_trait::iodata_t iodata_t;
	typedef typename handler_trait::rw_iodata_t rw_iodata_t;
	typedef typename handler_trait::conn_iodata_t conn_iodata_t;
	typedef typename handler_trait::request_list_t request_list_t;
	typedef typename handler_trait::on_error_d on_error_d;
	typedef typename handler_trait::file_error_d file_error_d;
	typedef typename handler_trait::iodata_error_d iodata_error_d;
	typedef typename handler_trait::response_struct response_struct;
	typedef typename handler_trait::system_complete_d system_complete_d;
	typedef typename handler_trait::file_error_d file_error_d;
	typedef typename handler_trait::response_result_enum response_result_enum;
	typedef typename handler_trait::post_task_d post_task_d;
	typedef typename handler_trait::stream_recv_d stream_recv_d;

	typedef sender_iodata_tt<iodata_t,rw_iodata_t> sender_iodata;


	void internal_on_iodata_error(iodata_t* _iodata)
	{
		assert(_iodata->actionid()==constant::__file_error);
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		notify_event_iodata_delete(iodata);
	}

	void on_connection_arrived(iodata_t* _iodata)
	{
		assert(_iodata->actionid()==constant::__file_connect);
		conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);
		if (!iodata->_m_on_system_complete.is_empty())
			iodata->_m_on_system_complete(iodata);
	}

	void internal_on_read_req(iodata_t* _iodata)
	{
		assert(_iodata->actionid()==constant::__file_read);
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		if (_iodata->_m_bytes_transfered==0 && _iodata->_m_errorid!=0)
			_iodata->_m_errorid = constant::__error_socket_read_nothing;
		if (!iodata->_m_on_system_complete.is_empty())
			iodata->_m_on_system_complete(iodata);
	}

	void internal_on_write_req(iodata_t* _iodata)
	{
		assert(_iodata->actionid()==constant::__file_write);
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		if (iodata->_m_errorid==0 &&
			iodata->_m_bytes_hasbeen_transfered<iodata->_m_bytes_willbe_transfered)
		{
			continue_to_send_next(iodata);
		}
		else if (!iodata->_m_on_system_complete.is_empty())
		{
			/// send finished
			iodata->_m_on_system_complete(iodata);
		}
	}

	void on_connect_complete_event(iodata_t* _iodata)
	{
		conn_iodata_t* iodata = static_cast<conn_iodata_t*>(_iodata);
		notify_dispatch_connect_complete(_iodata);
	}

	void on_package_send_complete(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		assert (iodata->_m_actionid==constant::__file_write);
		do_outter_event_for_package_write(iodata);
	}
	void on_stream_send_complete(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		assert (iodata->_m_actionid==constant::__file_write);
		do_outter_event_for_stream_write(iodata);
	}

	void continue_to_send_next(rw_iodata_t* iodata)
	{
		iodata->_m_errorid = sender_iodata(_m_file_handle_operation)(iodata->_m_file_handle,iodata);
		if (iodata->_m_errorid==0) return;
		iodata->_m_on_system_complete(iodata);
	}

	void start_to_recv_first(iodata_t* _iodata,system_complete_d const& sys_complete)
	{
		rw_iodata_t* piodata = static_cast<rw_iodata_t*>(_iodata);
		rw_iodata_t& iodata = *piodata;
		iodata.init_data_for_recv();
		iodata._m_step = constant::__step_read;
		iodata._m_actionid = constant::__file_read;
		iodata._m_on_system_complete.assign(sys_complete);
		/// just one read action, not all
		iodata._m_errorid = sender_iodata(_m_file_handle_operation)(iodata._m_file_handle,piodata);
		if (iodata._m_errorid==0) return;
		if (!sys_complete.is_empty()) sys_complete(piodata);
	}
	/// iodata_from_write._m_seq/_m_complete
	/// filedata.socket_client
	void start_to_recv_first_for_package(iodata_t* _iodata_from_write)
	{
		start_to_recv_first(_iodata_from_write,system_complete_d(this,&self::package_recv_for_back));
	}
	void start_to_recv_first_for_stream(iodata_t* _iodata_from_write)
	{
		start_to_recv_first(_iodata_from_write,system_complete_d(this,&self::stream_recv_for_back));
	}

	void start_to_recv_again_for_back(iodata_t* _iodata_from_read)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata_from_read);
		iodata->assign_data_for_recv();
		/// just one read action, not all
		iodata->_m_errorid = sender_iodata(_m_file_handle_operation)(iodata->_m_file_handle,iodata);
		if (iodata->_m_errorid==0) return;
		iodata->_m_on_system_complete(iodata);
	}

	/// for the message from the server response
	void package_recv_for_back(iodata_t* _iodata)
	{
		assert(_iodata->actionid()==constant::__file_read);
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		do_outter_event_for_package_read(iodata);
	}

	void stream_recv_for_back(iodata_t* _iodata)
	{
		assert(_iodata->actionid()==constant::__file_read);
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		do_outter_event_for_stream_read(iodata);
	}

	void post_task(size_t threadid,ox::mos::task_tt<void>* task)
	{
		if (_m_post_task.is_empty() || threadid==0)
			_m_dispatch_thread.add(task);
		_m_post_task(threadid,task);
	}

	thread_t* _m_dispatch_thread;
	post_task_d _m_post_task;
	file_handle_operation _m_file_handle_operation;
	on_error_d on_error;
	iodata_error_d on_iodata_delete;
	system_complete_d on_connect_complete;
	system_complete_d on_request_complete;
	stream_recv_d _m_stream_recv;
	//system_complete_d on_stream_read_complete;
	
public:
	void notify_event_iodata_delete(rw_iodata_t* iodata)
	{
		do_outter_event_delete(iodata);
	}
	void notify_event_iodata(rw_iodata_t* iodata)
	{
		do_outter_event(iodata);
	}
	void notify_event_reqlist_rwiodata_delete(request_list_t& reqlist,size_t error_code)
	{
		typedef request_list_t::iterator I;
		for (I i=reqlist.begin();i!=reqlist.end();++i)
		{
			rw_iodata_t* iodata = static_cast<rw_iodata_t*>(*i);
			iodata->_m_filedata = 0;
			iodata->_m_errorid = error_code;
			notify_event_iodata_delete(iodata);
		}
	}

private:
	void notify_dispatch_connect_complete(iodata_t* iodata)
	{
		assert(_m_dispatch_thread);
		_m_dispatch_thread->add(task_single::make(on_connect_complete,iodata));
	}
	void notify_dispatch_request_complete(iodata_t* iodata)
	{
		on_request_complete(iodata);
	}
	void notify_dispatch_iodata_rwerror(iodata_t* iodata)
	{
		assert(_m_dispatch_thread);
		_m_dispatch_thread->add(task_single::make(on_iodata_delete,iodata));
	}
	void notify_dispatch_stream_read_again(iodata_t* iodata)
	{
		assert(_m_dispatch_thread);
		rw_iodata_t* riodata = static_cast<rw_iodata_t*>(iodata);
		_m_dispatch_thread->add(task_single::make(
			_m_stream_recv,
			iodata->_m_filedata,
			riodata->output().value()));
	}
	void do_outter_event(rw_iodata_t* iodata)
	{
		response_struct response_data;
		assign(response_data,iodata);
		iodata->_m_response_return = iodata->_m_complete(response_data);
	}
	void do_outter_event_delete(rw_iodata_t* iodata)
	{
		do_outter_event(iodata);
		notify_dispatch_iodata_rwerror(iodata);
	}
	void do_outter_event_for_package_read(rw_iodata_t* iodata)
	{
		response_struct response_data;
		assign(response_data,iodata);
		response_result_enum ret = iodata->_m_complete(response_data);
		iodata->_m_response_return = ret;
		assert(ret<=constant::__resp_read_new);
		if (iodata->_m_errorid==0 && ret==constant::__resp_later)
			return;
		do_outter_event_for_package_read_for_bottom_half(iodata);
	}
	void do_outter_event_for_stream_read(rw_iodata_t* iodata)
	{
		response_struct response_data;
		assign(response_data,iodata);
		response_result_enum ret = iodata->_m_complete(response_data);
		iodata->_m_response_return = ret;
		assert(ret<=constant::__resp_read_new);
		if (iodata->_m_errorid==0 && ret==constant::__resp_later)
			return;
		do_outter_event_for_stream_read_for_bottom_half(iodata);
	}
	void do_outter_event_for_package_write(rw_iodata_t* iodata)
	{
		response_struct response_data;
		assign(response_data,iodata);
		response_result_enum ret = iodata->_m_complete(response_data);
		iodata->_m_response_return = ret;
		assert(ret<=constant::__resp_read_new);
		if (iodata->_m_errorid==0 && ret==constant::__resp_later)
			return;
		do_outter_event_for_package_write_for_bottom_half(iodata);
	}
	void do_outter_event_for_stream_write(rw_iodata_t* iodata)
	{
		response_struct response_data;
		assign(response_data,iodata);
		response_result_enum ret = iodata->_m_complete(response_data);
		iodata->_m_response_return = ret;
		assert(ret<=constant::__resp_read_new);
		if (iodata->_m_response_return==constant::__resp_unfinished)
			iodata->_m_response_return=constant::__resp_finished_and_keepalive;
		if (ret==constant::__resp_later && iodata->_m_errorid==0)
			return;
		do_outter_event_for_stream_write_for_bottom_half(iodata);
	}
	static void assign(response_struct& response_data,rw_iodata_t const* iodata)
	{
		assert(iodata->_m_bytes_transfered<=iodata->_m_buffer.size());
		response_data.seq = iodata->_m_seq;
		response_data.step = iodata->_m_step;
		response_data.errorid = iodata->_m_errorid;
		response_data.name = iodata->_m_name.c_str();
		response_data.file_number = iodata->_m_file_number;
		response_data.data = iodata->_m_buffer.data();
		response_data.size = iodata->_m_bytes_transfered;
		response_data.size_hasbeen_transfered = iodata->_m_bytes_hasbeen_transfered;
		//response_data.size_willbe_transfered = iodata->_m_bytes_willbe_transfered;
		response_data.buffer_size = iodata->buffer_size();
		response_data.outofband = iodata->_m_outofband;
		response_data._m_flag = &iodata->_m_flag;
		response_data.result_notify = &iodata->_m_result_notify;
		response_data.write_back = iodata->_m_fileinfo?
			&iodata->_m_fileinfo->group->_m_send_back : 0;
		response_data.response._m_object = iodata;
	}
public:
	/// bottom half
	void do_outter_event_for_package_read_for_bottom_half(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		assert(iodata->actionid()==constant::__file_read);
		if (iodata->_m_errorid==0)
			if (iodata->_m_response_return<=constant::__resp_unfinished)
				return start_to_recv_again_for_back(iodata);
		notify_dispatch_request_complete(iodata);
	}
	void do_outter_event_for_stream_read_for_bottom_half(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		assert(iodata->actionid()==constant::__file_read);
		if (iodata->_m_errorid==0)
		{
			if (iodata->_m_response_return<=constant::__resp_finished_and_keepalive)
			{
				//if (iodata->_m_response_return==constant::__resp_finished_and_keepalive)
				//	iodata->_m_seq=iodata_t::seq_next();
				return start_to_recv_again_for_back(iodata);
			}
			if (iodata->_m_response_return==constant::__resp_read_new)
			{
				notify_dispatch_stream_read_again(iodata);
				notify_dispatch_iodata_rwerror(iodata);
				return;
			}
		}
		notify_dispatch_request_complete(iodata);
	}
	void do_outter_event_for_package_write_for_bottom_half(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		assert(iodata->actionid()==constant::__file_write);
		if (iodata->_m_errorid==0)
			if (iodata->_m_response_return==constant::__resp_unfinished)
				if (iodata->is_answer_needed().value())
					return start_to_recv_first_for_package(iodata);
		notify_dispatch_request_complete(iodata);
	}
	void do_outter_event_for_stream_write_for_bottom_half(iodata_t* _iodata)
	{
		rw_iodata_t* iodata = static_cast<rw_iodata_t*>(_iodata);
		assert(iodata->actionid()==constant::__file_write);
		notify_dispatch_request_complete(iodata);
	}
};


___namespace2_end()