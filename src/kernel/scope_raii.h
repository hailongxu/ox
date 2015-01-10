/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

#include "windows.h"
#include "delegate.h"


#pragma once


struct critical_section
{
	CRITICAL_SECTION _m_critical;
	critical_section()
	{
		::InitializeCriticalSection(&_m_critical);
	}
	void enter()
	{
		::EnterCriticalSection(&_m_critical);
	}
	void leave()
	{
		::LeaveCriticalSection(&_m_critical);
	}
	~critical_section()
	{
		::DeleteCriticalSection(&_m_critical);
	}
	template <typename t> critical_section(t);
	template <typename t> critical_section& operator=(t);
};

struct win_mutex
{
	win_mutex() : _m_mutex_handle(0) {}
	win_mutex(char const* name,bool is_owned=false)
	{
		_m_mutex_handle = 0;
		init(name,is_owned);
	}
	~win_mutex()
	{
		uninit();
	}
	void init(char const* name,bool is_owned=false)
	{
		assert (is_invalid());
		if (!is_invalid()) return;
		_m_mutex_handle = CreateMutexA(0,is_owned?TRUE:FALSE,name);
		if (_m_mutex_handle==0)
			return;
	}
	void uninit()
	{
		if (is_invalid()) return;
		CloseHandle(_m_mutex_handle);
		_m_mutex_handle = 0;//INVALID_HANDLE_VALUE;
	}
	HANDLE handle() const {return _m_mutex_handle;}
	bool is_invalid() const {return _m_mutex_handle==0;}
private:
	HANDLE _m_mutex_handle;
};

struct win_mutex_slave
{
	win_mutex_slave() : _m_mutex_handle(0) {}
	win_mutex_slave(char const* name)
	{
		open(name);
	}
	~win_mutex_slave()
	{
		CloseHandle(_m_mutex_handle);
		_m_mutex_handle = 0;//INVALID_HANDLE_VALUE;
	}
	bool open(char const* name)
	{
		if (_m_mutex_handle!=0) return false;
		_m_mutex_handle = OpenMutexA(MUTEX_ALL_ACCESS,FALSE,name);
		return !is_invalid();
	}
	HANDLE handle() const {return _m_mutex_handle;}
	bool is_invalid() const {return _m_mutex_handle==0;}
private:
	HANDLE _m_mutex_handle;
};

template <bool bmanual>
struct win_event
{
	static HANDLE create(bool init_signed=false,char const* name=0)
	{
		return CreateEventA(0,bmanual?TRUE:FALSE,init_signed?TRUE:FALSE,name);
	}
	static void close(HANDLE handle)
	{
		bool b = CloseHandle(handle)==TRUE?true:false;
		size_t err = b?0:GetLastError();
		assert (b);
	}
	static void set_signaled(HANDLE handle)
	{
		bool b = SetEvent(handle)==TRUE?true:false;
		size_t err = b?0:GetLastError();
		assert (b);
	}
	static void set_unsignaled(HANDLE handle)
	{
		bool b = ResetEvent(handle)==TRUE?true:false;
		size_t err = b?0:GetLastError();
		assert (b);
	}

	win_event(bool init_signed=false,char const* name=0)
	{
		_m_event_handle = CreateEventA(0,bmanual?TRUE:FALSE,init_signed?TRUE:FALSE,name);
	}
	~win_event()
	{
		CloseHandle(_m_event_handle);
		_m_event_handle = 0;
	}
	void set_signaled()
	{
		SetEvent(_m_event_handle);
	}
	void set_unsignaled()
	{
		ResetEvent(_m_event_handle);
	}
	HANDLE handle() const {return _m_event_handle;}
private:
	HANDLE _m_event_handle;
};
typedef win_event<false> win_auto_event;
typedef win_event<true> win_manual_event;


template <typename sync_object>
struct scope_sync;

template <>
struct scope_sync <critical_section>
{
	scope_sync(critical_section& critical)
		: _m_critical_section(critical)
	{
		_m_critical_section.enter();
	}
	~scope_sync()
	{
		_m_critical_section.leave();
	}

	critical_section& _m_critical_section;
	template <typename t> scope_sync(t);
	template <typename t> scope_sync& operator=(t);
};

template <>
struct scope_sync <critical_section*>
{
	scope_sync(critical_section* critical)
		: _m_critical_section(critical)
	{
		if (_m_critical_section)
		_m_critical_section->enter();
	}
	~scope_sync()
	{
		if (_m_critical_section)
		_m_critical_section->leave();
	}

	critical_section* _m_critical_section;
	template <typename t> scope_sync(t);
	template <typename t> scope_sync& operator=(t);
};

template <>
struct scope_sync <win_mutex>
{
	scope_sync(win_mutex& mutex,size_t timeout_mill_second=-1)
		: _m_mutex(mutex)
	{
		DWORD ret = ::WaitForSingleObject(_m_mutex.handle(),timeout_mill_second);
		bool isok = (ret==WAIT_OBJECT_0 || ret==WAIT_ABANDONED);
		assert(isok);
	}
	~scope_sync()
	{
		if (_m_mutex.handle()==INVALID_HANDLE_VALUE) return;
		BOOL b = ReleaseMutex(_m_mutex.handle());
		if (!b)
		{
			int error = GetLastError();
		}
	}
	win_mutex& _m_mutex;
};

template <bool bmanual>
struct scope_sync <win_event<bmanual>>
{
	typedef win_event<bmanual> event_t;
	scope_sync(event_t& event,size_t timeout_mill_second=-1)
		: _m_event(event)
	{
		DWORD ret = ::WaitForSingleObject(_m_event.handle(),timeout_mill_second);
		bool isok = (ret==WAIT_OBJECT_0 || ret==WAIT_ABANDONED);
		assert(isok);
	}
	~scope_sync()
	{
		if (_m_event.handle()==INVALID_HANDLE_VALUE) return;
		if (bmanual) _m_event.set_unsignaled();
	}
	static void wait(HANDLE hevent,size_t timeout_mill_second=-1)
	{
		DWORD ret = ::WaitForSingleObject(hevent,timeout_mill_second);
		bool isok = (ret==WAIT_OBJECT_0 || ret==WAIT_ABANDONED);
		assert(isok);
	}
	event_t& _m_event;
};


typedef scope_sync<critical_section> scope_cs;
typedef scope_sync<critical_section*> scope_csptr;
typedef scope_sync<win_mutex> scope_mutex;
typedef scope_sync<win_auto_event> scope_auto_event;
typedef scope_sync<win_manual_event> scope_manual_event;



template <typename type_tn>
struct scope_raii
{
	typedef delegate<void(type_tn&)> on_event;
	scope_raii(type_tn& obj)
		: _m_obj(obj)
	{
		if (on_init.is_empty()) return ;
		on_init(_m_obj);
	}
	~scope_raii()
	{
		if (on_lize.is_empty()) return ;
		on_lize(_m_obj);
	}

	on_event on_init;
	on_event on_lize;
	type_tn& _m_obj;
};

#include "task_decl.h"
template <typename t>
struct scope_task
{
	typedef ox::thread_task_t task_t;
	scope_task(task_t* task=0)
		: _m_task(task)
	{}
	~scope_task()
	{
		if (!_m_task) _m_task->run();
	}
	task_t* _m_task;
};


#include "atomic_number.h"
template <typename number_tn>
struct scope_raii<atomic_number<number_tn>>
{
	typedef atomic_number<number_tn> atomic_number_t;
	scope_raii(atomic_number_t& number,number_tn exit_value)
		: _m_atomic_number(number)
	{
	}
	scope_raii(atomic_number_t& atomic_num,number_tn start_value,number_tn exit_value)
		: _m_atomic_number(atomic_num)
	{
		_m_atomic_number = start_value;
	}
	~scope_raii()
	{
		_m_atomic_number = _m_exit_value;
	}
	atomic_number_t& _m_atomic_number;
	number_tn _m_exit_value;
};

typedef scope_raii<atomic_number<long>> scope_atomic_long;