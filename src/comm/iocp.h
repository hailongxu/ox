

#include <Windows.h>
#include <assert.h>
#include <map>
#include "../ox/nsab.h"
#include "../mos/delegate.h"
#include "../mos/task_feedback.h"
#include "../mos/win_queue_thread.h"
#include "../mos/win_multi_thread.h"
#include "file_service_operation.h"
#include "log_proxy.h"
#include "sender_iodata.h"


#pragma once


___namespace2_begin(ox,comm)


template <typename subthread_trait>
struct iocp_subthread_process
{
	typedef typename subthread_trait::actions_t actions_t;
	typedef typename subthread_trait::filedata_t filedata_t;
	typedef typename subthread_trait::iodata_t iodata_t;
	typedef typename subthread_trait::error_process_d error_process_d;
	typedef typename subthread_trait::last_error_d last_error_d;

	//static size_t error_abandoned_wait_0() {return 735L;}    // winnt

	iocp_subthread_process()
	{
		_m_iocp_handle = 0;
		_m_iodata_exit = 0;
	}
	unsigned operator()()
	{
		filedata_t* _m_filedata;
		iodata_t* _m_iodata;
		while(1)
		{
			assert(_m_iocp_handle && "iocp handle is not valid");
			DWORD tid = GetCurrentThreadId();
			DWORD error_code = 0;
			_m_filedata = 0;
			_m_iodata = 0;
			size_t bytes_transfered = -1;
			bool ret = GetQueuedCompletionStatus(
				_m_iocp_handle,
				(LPDWORD)&bytes_transfered,
				(PULONG_PTR)&_m_filedata,
				(LPOVERLAPPED*)&_m_iodata,
				INFINITE
				) == TRUE;
			
			if (!ret)
			{
				error_code = _m_last_error();
			}

			if (0==_m_iodata)
			{
				assert (false);
				if (!_m_error_process.is_empty())
					_m_error_process(error_code);
				break;
			}
			if (_m_iodata == _m_iodata_exit)
			{
				ox::comm::log_proxy::instance().printf("client iocp [exit] sub thread id[%u] receives exit instruction\r\n",::GetCurrentThreadId());
				break;
			}

			if (!iodata_iocp_check_t::instance().safe_has_and_remove(_m_iodata))
				continue;

			if (_m_iodata->error_code()!=0)
				continue;
			if (_m_iodata->error_code()==0 && error_code!=0)
				_m_iodata->set_error_code(error_code);

			/// action id can be deleted, use function instead *******
			_m_iodata->set_bytes_transfered(bytes_transfered);
			size_t ioactionid = _m_iodata->actionid();
			typedef actions_t::iterator I;
			I i = _m_actions->find(ioactionid);
			if (i==_m_actions->end())
			{
				assert(false && "invalid action, ignore and continue");
				continue;
			}
			/// handle the event
			i->second(_m_iodata);

			if (error_code!=0)
				if (!_m_error_process.is_empty())
					_m_error_process(error_code);
		}
		return 0;
	}

	HANDLE _m_iocp_handle;
	actions_t* _m_actions;
	error_process_d _m_error_process;
	last_error_d _m_last_error;
	iodata_t* _m_iodata_exit;
};


template <typename iocp_trait_tn>
struct iocp_tt
{
	typedef iocp_tt self;
	typedef typename iocp_trait_tn::iodata_base_t iodata_base_t;
	typedef typename iocp_trait_tn::task_iodata_t task_iodata_t;
	typedef typename iocp_trait_tn::filedata_t filedata_t;
	typedef typename iocp_trait_tn::iodata_t iodata_t;
	typedef communication_const constant;
	typedef ox::mos::win_multi_threads<ox::mos::win_thread<int(void)>> threads_t;
	typedef typename threads_t::thread_t thread_t;
	typedef ox::mos::win_queue_thread dispatch_thread_t;
	typedef ox::mos::task_single<void> task_single;
	typedef delegate<void(iodata_t*)> action_t;
	typedef std::map<size_t,action_t> actions_t;
	typedef multi_delegate<void()> all_exited_d;
	typedef delegate<void(size_t)> error_process_d;
	typedef file_handle_operation::last_error_d last_error_d;
	struct iocp_subprocesser_traiter
	{
		typedef actions_t actions_t;
		typedef filedata_t filedata_t;
		typedef iodata_t iodata_t;
		typedef error_process_d error_process_d;
		typedef last_error_d last_error_d;
	};
	typedef iocp_subthread_process<iocp_subprocesser_traiter> iocp_subprocesser;

	iocp_tt() : _m_k(1), _m_c(0), _m_complete_handle(INVALID_HANDLE_VALUE) {}
	void set_thread_count(int k,int c)
	{
		_m_k=k, _m_c=c;
	}
	void init(dispatch_thread_t* dispatch,last_error_d const& last_error)
	{
		on_all_exited.clear();
		_m_threads.on_exiting().clear();
		_m_threads.on_exited().clear();
		_m_threads.on_exiting().add(threads_t::exiting_d::delegate_t(this,&iocp_tt::on_thread_exiting));
		_m_threads.on_exited().add(threads_t::exited_d::delegate_t(this,&iocp_tt::on_all_stoped));
		_m_threads.bind(dispatch);
		_m_last_error = last_error;
		//_m_subprocesser._m_iocp_handle = _m_complete_handle;
		_m_subprocesser._m_actions = &_m_actions;
		_m_subprocesser._m_last_error = _m_last_error;
		_m_subprocesser._m_iodata_exit = &_m_iodata_exit;
		_m_threads.set_id(2);
	}

	int add_filehandle(HANDLE filehandle,filedata_t* filedata)
	{
		int error_code = 0;
		//printf ("iocp: add_filehandle(%p)\n",filehadle);
		HANDLE h = CreateIoCompletionPort(filehandle,_m_complete_handle,(ULONG_PTR)filedata,0);
		if (h==0)
		{
			error_code = GetLastError(); /// or WSAGetLastError();
			assert(h && h==_m_complete_handle);
		}
		return error_code;
	}

	void add_handler(size_t actionid,action_t const& handler)
	{
		_m_actions[actionid] = handler;
	}
	
	bool is_started() const
	{
		return _m_complete_handle != INVALID_HANDLE_VALUE;
	}
	void start()
	{
		_m_complete_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
		_m_subprocesser._m_iocp_handle = _m_complete_handle;
		/// start the sub thread
		size_t subthreads_count = get_thread_count();
		for (size_t i=0;i<subthreads_count;i++)
		{
			char name[32];
			sprintf (name,"iocp-thread-%d",i+1);
			thread_t* th = _m_threads.create_thread(&_m_subprocesser,0,name,true);
			assert(th && "create thread failed");
			//th->add(task_single::make(&_m_subprocesser));
		}
	}

	void astop()
	{
		ox::comm::log_proxy::instance().printf("communication client iocp-[exit] stop called\r\n");
		_m_threads.astop();
		_m_threads.foreach_do(threads_t::action_d(this,&self::exit_iocp_notify));
	}

	void wait(size_t timeout_ms=-1)
	{
		_m_threads.wait_subs(timeout_ms);
	}

	void notify_iocp(filedata_t* filedata,iodata_t* iodata)
	{
		ox::mos::scope_cs lock(iodata_iocp_check_t::instance()._m_cs);
		BOOL b = PostQueuedCompletionStatus(_m_complete_handle,
			0,(ULONG_PTR)filedata,(LPOVERLAPPED)iodata);
		if (b==TRUE) iodata_iocp_check_t::instance().add(iodata);
	}
	static void on_task(iodata_t* _iodata)
	{
		assert (_iodata->actionid()==constant::__file_task);
		iodata_base_t* iodata_base = static_cast<iodata_base_t*>(_iodata);
		task_iodata_t* iotask = static_cast<task_iodata_t*>(iodata_base);
		assert (iotask->_m_task);
		iotask->_m_task->run();
		iotask->destroy_self();
	}
	all_exited_d on_all_exited;
private:
	bool exit_iocp_notify(thread_t* th)
	{
		_m_iodata_exit._m_actionid = constant::__file_exit;
		notify_iocp(0,&_m_iodata_exit);
		return true;
	}
	void on_thread_exiting(thread_t* th,size_t count)
	{
		ox::comm::log_proxy::instance().printf("communication client iocp [exit] thread id[%u] exited %d left\r\n",th->threadid(),count);
		return;
		char buff [128];
		sprintf (buff,"iocp thread id[%u] exited %d left\n",th->threadid(),count);
		OutputDebugStringA(buff);
		//printf ("thread id[%u] %d left\n",th->threadid(),count);
	}
	void on_all_stoped()
	{
		ox::comm::log_proxy::instance().printf("communication client iocp [exit] all sub threads have exited\r\n");
		DWORD error_code = 0;
		BOOL b = ::CloseHandle(_m_complete_handle);
		if (!b)
		{
			error_code = ::GetLastError();
			ox::comm::log_proxy::instance().printf("communication client iocp [exit] Close handle of IOCP failed error:[%u]\r\n",error_code);
			assert(false && "Close handle of IOCP failed");
		}
		_m_complete_handle = INVALID_HANDLE_VALUE;
		if (!on_all_exited.is_empty())
			on_all_exited();
	}
	size_t get_thread_count()
	{
		int count = _m_k*get_processor_count()+_m_c;
		if (count<=0) count = 1;
		return count;
	}
	static size_t get_processor_count()
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		return info.dwNumberOfProcessors;
	}
	int _m_k, _m_c;
	iodata_t _m_iodata_exit;
	threads_t _m_threads;
	HANDLE _m_complete_handle;
	actions_t _m_actions;
	iocp_subprocesser _m_subprocesser;
	last_error_d _m_last_error;
}; /// end of iocp


___namespace2_end()