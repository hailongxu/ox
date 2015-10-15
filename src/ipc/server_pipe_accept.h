

#include <assert.h>
#include "../mos/win_queue_thread.h"
#include "../comm/communication_const.h"
#include "../comm/file_service_operation.h"
#include "file_pipe_operation.h"


#pragma once



___namespace2_begin(ox,ipc)

	struct accept_pipe_trait
	{
		typedef ox::mos::win_queue_thread thread_t;
		typedef file_pipe_operation fileop;
		typedef ox::comm::file_handle_operation::file_handle_t file_handle_t;
		typedef ox::comm::server_accept_t::file_accepted_d file_accepted_d;
		typedef ox::comm::server_accept_t::started_d started_d;
		typedef ox::comm::server_accept_t::exited_d exited_d;
	};

	template <typename connection_trait>
	struct server_pipe_accept
	{
		typedef server_pipe_accept self;
		typedef ox::mos::win_queue_thread thread_t;
		typedef typename connection_trait::fileop fileop;
		typedef typename connection_trait::started_d started_d;
		typedef typename connection_trait::exited_d exited_d;
		typedef typename connection_trait::file_handle_t file_handle_t;
		typedef typename connection_trait::file_accepted_d file_accepted_d;

		self() : _m_bind(0), _m_file_listened(file_handle_operation::invalid_file_handle()) {}

		size_t start(ox::utl::cdata_t name)
		{
			_m_name.assign(name.begin,name.size);
			_m_thread.on_exit().assign(this,&self::on_thread_exited);
			_m_thread.add(abc::task_single<int>::make(this,&self::do_start));
			return _m_thread.start()?0:-1;
		}
		void stop()
		{
			if (!_m_thread.is_started())
			{
				on_thread_exited(0);
				return;
			}
			_m_thread.stop_next();
			if (!file_handle_operation::is_handle_invalid(_m_file_listened))
				fileop::close(_m_file_listened);
		}
		void wait(size_t timeout_ms)
		{
			assert(GetCurrentThreadId()!=_m_thread.threadid());
			_m_thread.join(timeout_ms);
		}
		file_accepted_d& on_connection_connected(void const* param)
		{
			_m_bind = param;
			return _m_on_connection_accepted;
		}
		started_d& on_started()
		{
			return _m_on_started;
		}
		exited_d& on_exit()
		{
			return _m_on_exited;
		}
		char const* name()
		{
			return _m_name.c_str();
		}
		comm::server_accept_t as_server_accept()
		{
			comm::server_accept_t result;
			result.start.assign(this,&self::start);
			result.stop.assign(this,&self::stop);
			result.wait.assign(this,&self::wait);
			result.on_connected.assign(this,&self::on_connection_connected);
			result.on_started.assign(this,&self::on_started);
			result.on_exit.assign(this,&self::on_exit);
			result.name.assign(this,&self::name);
			return result;
		}

	private:
		void do_start()
		{
			while (1)
			{
				static const size_t __1024_buf_size = communication_const::__send_receive_buffer_size_default;
				size_t error_code = 0;
				_m_file_listened = fileop::create(_m_name.c_str(),__1024_buf_size,__1024_buf_size);
				if (file_handle_operation::is_handle_invalid(_m_file_listened))
				{
					error_code = GetLastError();
					return;
				}
				if (!_m_on_started.is_empty())
				{
					_m_on_started(ox::utl::to_data(_m_name));
					_m_on_started.clear();
				}
				error_code = fileop::accept(_m_file_listened);
				bool is_error_or_exited = error_code!=0 || _m_thread.is_exiting();
				if (is_error_or_exited || _m_on_connection_accepted.is_empty())
					break;
				_m_on_connection_accepted(error_code,_m_file_listened,_m_name.c_str(),_m_bind);
			}
		}

		void on_thread_exited(thread_t*)
		{
			if (_m_on_exited.is_empty())
				return;
			_m_on_exited(this);
		}

		void const* _m_bind;
		thread_t _m_thread;
		file_accepted_d _m_on_connection_accepted;
		started_d _m_on_started;
		exited_d _m_on_exited;
		file_handle_t _m_file_listened;
		std::string _m_name;
	};

	template <typename connection_trait>
	struct server_pipe_aaccept
	{
		typedef server_pipe_aaccept self;
		typedef ox::mos::win_thread<unsigned()> thread_t;
		typedef typename connection_trait::fileop fileop;
		typedef typename connection_trait::started_d started_d;
		typedef typename connection_trait::exited_d exited_d;
		typedef typename connection_trait::file_handle_t file_handle_t;
		typedef typename connection_trait::file_accepted_d file_accepted_d;

		self() : _m_bind(0), _m_file_listened(file_handle_operation::invalid_file_handle())
		{
			_m_exiting = 0;
			clear_overlapped();
		}

		void init_set_started_instance(size_t file_count)
		{
			_m_file_count_started = file_count;
		}

		size_t start(ox::utl::cdata_t name)
		{
			_m_name.assign(name.begin,name.size);
			_m_thread.on_run().assign(this,&self::do_start);
			_m_thread.on_exit().assign(this,&self::on_thread_exited);
			return _m_thread.start()?0:-1;
		}
		void stop()
		{
			if (!_m_thread.is_started())
			{
				on_thread_exited(0);
				return;
			}
			_m_exiting = 1;
			_m_accept_event.set_signaled();
			fileop::close(_m_file_listened);
		}
		void wait(size_t timeout_ms)
		{
			assert(GetCurrentThreadId()!=_m_thread.threadid());
			_m_thread.join(timeout_ms);
		}
		file_accepted_d& on_connection_connected(void const* param)
		{
			_m_bind = param;
			return _m_on_connection_accepted;
		}
		started_d& on_started()
		{
			return _m_on_started;
		}
		exited_d& on_exit()
		{
			return _m_on_exited;
		}
		char const* name()
		{
			return _m_name.c_str();
		}
		comm::server_accept_t as_server_accept()
		{
			comm::server_accept_t result;
			result.start.assign(this,&self::start);
			result.stop.assign(this,&self::stop);
			result.wait.assign(this,&self::wait);
			result.on_connected.assign(this,&self::on_connection_connected);
			result.on_started.assign(this,&self::on_started);
			result.on_exit.assign(this,&self::on_exit);
			result.name.assign(this,&self::name);
			return result;
		}

	private:
		
		unsigned do_start()
		{
			_m_accept_overlapped.hEvent = _m_accept_event.handle();
			size_t error_code = 0;
			clear_accept_event();
			while (1)
			{
				if (is_exiting()) break;
				if (!is_handle_ready()) /// create pipe event
				{
					error_code = create_ipc_pipe();
					error_code = aaccept();
					trigger_once_on_started_event();
					if (error_code==0)
					{
						notify_accepted_event(error_code); /// ok, accepted already
						clear_accept_handle();
						continue; /// create again
					}
				}
				/// on pending process
				if (is_exiting()) break;
				bool waited = wait_accept_event();
				if (is_exiting()) break;
				assert (waited);
				if (waited) /// accept event coming
				{
					error_code = aget_overlapped_result();
					notify_accepted_event(error_code);
					clear_accept_handle();
					clear_accept_event();
				}
			} /// end of while
			return 0;
		}

		bool is_exiting() const
		{
			return _m_exiting.value()==1;
		}
		bool is_handle_ready() const
		{
			return !file_handle_operation::is_handle_invalid(_m_file_listened);
		}
		void clear_accept_handle()
		{
			_m_file_listened = file_handle_operation::invalid_file_handle();
		}
		void clear_accept_event()
		{
			_m_accept_event.set_unsignaled();
		}
		size_t aget_overlapped_result()
		{
			return fileop::aget_overlapped_result(_m_file_listened,&_m_accept_overlapped);
		}
		bool wait_accept_event()
		{
			DWORD wait_result = WaitForSingleObject(_m_accept_event.handle(),-1);
			return wait_result==WAIT_OBJECT_0;
		}
		size_t create_ipc_pipe()
		{
			static const size_t __1024_buf_size = communication_const::__send_receive_buffer_size_default;
			size_t error_code = 0;
			_m_file_listened = fileop::create(_m_name.c_str(),__1024_buf_size,__1024_buf_size);
			if (file_handle_operation::is_handle_invalid(_m_file_listened))
				error_code = GetLastError();
			return error_code;
		}
		size_t aaccept()
		{
			return fileop::aaccept(_m_file_listened,&_m_accept_overlapped);
		}
		void notify_accepted_event(size_t error_code)
		{
			_m_on_connection_accepted(error_code,_m_file_listened,_m_name.c_str(),_m_bind);
		}
		void trigger_once_on_started_event()
		{
			if (_m_on_started.is_empty()) return;
			_m_on_started(ox::utl::to_data(_m_name));
			_m_on_started.clear();
		}
		void on_thread_exited(thread_t*)
		{
			if (_m_on_exited.is_empty())
				return;
			_m_on_exited(this);
		}
		void clear_overlapped()
		{
			memset(&_m_accept_overlapped,0,sizeof(_m_accept_overlapped));
		}

		void const* _m_bind;
		thread_t _m_thread;
		file_accepted_d _m_on_connection_accepted;
		started_d _m_on_started;
		exited_d _m_on_exited;
		file_handle_t _m_file_listened;
		std::string _m_name;
		size_t _m_file_count_started;
		size_t _m_parall_count;
		ox::mos::win_manual_event _m_accept_event;
		OVERLAPPED _m_accept_overlapped;
		ox::mos::atomic_long _m_exiting;
	};


___namespace2_end()