/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */

#include "windows.h"
#include <assert.h>
#include "../ox/nsab.h"
#include "../cxx/delegate.h"


#pragma once


___namespace2_begin(ox,mos)

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
	~win_mutex()
	{
		uninit();
	}
	win_mutex() : _m_mutex_handle(0)
	{
		_m_mutex_handle = 0;
		init(0,false);
	}
	win_mutex(bool init_later) : _m_mutex_handle(0)
	{}
	win_mutex(char const* name,bool is_owned=false)
	{
		_m_mutex_handle = 0;
		init(name,is_owned);
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
	~win_mutex_slave()
	{
		CloseHandle(_m_mutex_handle);

	}
	win_mutex_slave()
	{
        _m_mutex_handle = 0;//INVALID_HANDLE_VALUE;
		open(0);
	}
	win_mutex_slave(bool init_later) : _m_mutex_handle(0)
	{}
	win_mutex_slave(char const* name)
	{
        _m_mutex_handle = 0;//INVALID_HANDLE_VALUE;
		open(name);
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
	typedef HANDLE handle_t;
	static HANDLE create(bool init_signed=false,char const* name=0)
	{
		return CreateEventA(0,bmanual?TRUE:FALSE,init_signed?TRUE:FALSE,name);
	}
	static void close(HANDLE handle)
	{
		bool b = CloseHandle(handle)==TRUE?true:false;
		size_t err = b?0:GetLastError();
		assert(!handle||b);
	}
	static void set_signaled(HANDLE handle)
	{
		bool b = SetEvent(handle)==TRUE?true:false;
		size_t err = b?0:GetLastError();
		assert (!handle||b);
	}
	static void set_unsignaled(HANDLE handle)
	{
		bool b = ResetEvent(handle)==TRUE?true:false;
		size_t err = b?0:GetLastError();
		assert(!handle||b);
	}
	static bool is_handle_valid(HANDLE handle)
	{
		return handle != 0;
	}
	static HANDLE invalid_handle()
	{
		return HANDLE(0);
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
		DWORD ret = ::WaitForSingleObject(_m_mutex.handle(),(DWORD)timeout_mill_second);
		bool isok = (ret==WAIT_OBJECT_0 || ret==WAIT_ABANDONED || WAIT_TIMEOUT);
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
	enum wait_enum
	{
		__success = WAIT_OBJECT_0,
		__timeout = WAIT_TIMEOUT,
		__abandoned = WAIT_ABANDONED,
		__failed = WAIT_FAILED,
	};
	typedef win_event<bmanual> event_t;
	scope_sync(event_t& event,size_t timeout_mill_second=-1)
		: _m_event(event)
	{
		wait_enum ret = wait(_m_event.handle(),timeout_mill_second);
		bool isok = (ret==__success || ret==__abandoned);
		assert(isok);
	}
	~scope_sync()
	{
		if (_m_event.handle()==INVALID_HANDLE_VALUE) return;
		if (bmanual) _m_event.set_unsignaled();
	}
	static wait_enum wait(HANDLE hevent,size_t timeout_mill_second=-1)
	{
		DWORD ret = ::WaitForSingleObject(hevent,(DWORD)(timeout_mill_second));
		bool isok = (ret==WAIT_OBJECT_0 || ret==WAIT_ABANDONED);
		assert(isok);
		return wait_enum(ret);
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

___namespace2_end()



#include "task_decl.h"

___namespace2_begin(ox,mos)

template <typename t>
struct scope_task
{
	typedef ox::mos::thread_task_t task_t;
	scope_task(task_t* task=0)
		: _m_task(task)
	{}
	~scope_task()
	{
		if (!_m_task) _m_task->run();
	}
	task_t* _m_task;
};

___namespace2_end()



#include "atomic_number.h"

___namespace2_begin(ox,mos)

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

template <typename number_tn,size_t delta_tc=1>
struct scope_delta
{
	scope_delta(number_tn& atomic_num)
		: _m_atomic_number(atomic_num)
	{
		_m_atomic_number += delta_tc;
	}
	~scope_delta()
	{
		_m_atomic_number -= delta_tc;
	}
	number_tn& _m_atomic_number;
};

template <typename number_tn,size_t delta_tc>
struct scope_delta<atomic_number<number_tn>,delta_tc>
{
	typedef atomic_number<number_tn> atomic_number_t;
	scope_delta(atomic_number_t& atomic_num)
		: _m_atomic_number(atomic_num)
	{
		_m_atomic_number += delta_tc;
	}
	~scope_delta()
	{
		_m_atomic_number -= delta_tc;
	}
	atomic_number_t& _m_atomic_number;
};

typedef scope_delta<atomic_number<long>> scope_delta_atomic_long;


___namespace2_end()