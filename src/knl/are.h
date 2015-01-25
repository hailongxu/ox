// kernel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>


#include <map>
#include "../cxx/delegate.h"
#include "../thread/task_decl.h"
#include "../thread/win_queue_thread.h"
#include "../thread/win_queue_multi_thread.h"
#include "../thread/win_queue_pool_thread.h"
//#include "../thread/communication_helper.h"
#include "../cxx/simple_allocator.h"
#include "../ox/nsab.h"



___namespace_begin(ox,cor)


enum error_enum
{
	__error_null,
	__error_groupid_is_invalid,
};

struct service_d
{
	typedef delegate<void()> init_d;
	typedef delegate<void()> uninit_d;
	typedef delegate<bool()> is_started_d;
	typedef delegate<void()> start_d;
	typedef delegate<void()> stop_d;
	typedef delegate<void(size_t cmdid,ox::run_tt<void>* run)> add_function_d;
	typedef delegate<void(size_t cmdid)> remove_function_d;

	init_d init;
	uninit_d uninit;
	is_started_d is_started;
	start_d start;
	stop_d stop;
	add_function_d add_function;
	remove_function_d remove_function;
};



struct service_t
{
	typedef ox::run_tt<void> run_t;
	typedef std::map<size_t,run_t*> functions_t;
	void init()
	{
		_m_service.init();
	}
	void uninit()
	{
		_m_service.uninit();
	}
	bool is_started() const
	{
		return _m_service.is_started();
	}
	void start()
	{
		_m_service.start();
	}
	void stop()
	{
		_m_service.stop();
	}
	void add_function(size_t cmdid,run_t* run)
	{
		_m_functions[cmdid] = run;
	}
	void remove_function(size_t cmdid)
	{
		_m_functions.erase(cmdid);
	}
	functions_t _m_functions;
	service_d _m_service;
};


struct string_less
{
	bool operator() (std::string const& a,std::string const& b) const
	{
		return a.compare(b)<0;
	}
};
/// abstract runtime environment
struct are_t
{
	struct item_t;
	struct item_multi_t;
	struct item_pool_t;
	typedef ox::win_queue_thread queue_thread_t;
	typedef ox::cxx::simple_allocator allocator_t;
	typedef std::map<std::string,item_t*,string_less> groups_t;
	enum group_type_enum
	{
		__group_type_null,
		__group_type_multi,
		__group_type_pool,
	};
	struct yourid_t
	{
		explicit yourid_t(size_t id):_m_id(id) {}
		operator size_t() const {return _m_id;}
		size_t value() const {return _m_id;}
		size_t _m_id;
	};
	struct item_t
	{
		group_type_enum category;
		item_t(allocator_t& allocator,group_type_enum group_type)
			: _m_allocator(allocator)
			, category(group_type)
		{}
		void destroy()
		{
			switch(category)
			{
			case __group_type_multi: static_cast<item_multi_t*>(this)->destroy(); break;
			case __group_type_pool: static_cast<item_pool_t*>(this)->destroy(); break;
			default: assert(false);
			}
		}
		void add(queue_thread_t* thread)
		{
			switch(category)
			{
			case __group_type_multi: static_cast<item_multi_t*>(this)->_m_threads.add_thread(thread); break;
			//case __group_type_pool: static_cast<item_pool_t*>(this)->_m_threads.add_thread(thread); break;
			default: assert(false);
			}
		}
		void remove(queue_thread_t* thread)
		{
			switch(category)
			{
			case __group_type_multi: static_cast<item_multi_t*>(this)->remove(thread); break;
			case __group_type_pool: static_cast<item_pool_t*>(this)->remove(thread); break;
			default: assert(false);
			}
		}
		queue_thread_t* create()
		{
			switch(category)
			{
			case __group_type_multi: return static_cast<item_multi_t*>(this)->create();
			case __group_type_pool: return static_cast<item_pool_t*>(this)->create();
			default: assert(false);
			}
		}
		allocator_t& _m_allocator;
	};
	struct item_multi_t : item_t
	{
		item_multi_t(allocator_t& allocator)
			: item_t(allocator,__group_type_multi) {}
		void destroy()
		{
			_m_threads.stop();
			_m_allocator._delete(this);
		}
		ox::win_queue_multi_threads _m_threads;
	};
	struct item_pool_t : item_t
	{
		item_pool_t(allocator_t& allocator)
			: item_t(allocator,__group_type_pool) {}
		void destroy()
		{
			_m_threads.stop();
			_m_allocator._delete(this);
		}
		ox::win_queue_pool_thread _m_threads;
	};
	queue_thread_t* add_thread(char const* groupid)
	{
		item_t* item = get_or_new_group(groupid);
		if (!item) return 0;
		return item->create();
	}
	void remove_thread(char const* groupid,size_t threadid)
	{
		group_type_enum gt = group_type(groupid);
		//if (gt==__group_type_multi)
	}
	/// if id is pool, dispatch, if not, id is threadid
	void add(ox::thread_task_t* task, char const* poolid)
	{
		item_t* item = get_group(poolid);
		if (!item || item->category!=__group_type_pool) return;
		item_pool_t* pool = static_cast<item_pool_t*>(item);
		pool->_m_threads.add(task);
	}
	void add(ox::thread_task_t* task, size_t threadid,char const* multiid)
	{
		ox::win_queue_multi_threads* multi = as_multi(multiid);
		if (!multi) return;
		multi->add_task(task,threadid);
	}
	ox::win_queue_multi_threads* as_multi(char const* multiid)
	{
		item_t* item = get_group(multiid);
		if (!item || item->category!=__group_type_multi) return 0;
		item_multi_t* multi = static_cast<item_multi_t*>(item);
		return &multi->_m_threads;
	}
	item_t* get_group(char const* groupid,size_t* errorid=0)
	{
		if (errorid) *errorid=__error_null;
		group_type_enum gt = group_type(groupid);
		if (gt==__group_type_null)
		{
			if (errorid) *errorid=__error_groupid_is_invalid;
			return 0;
		}
		groups_t::iterator i = _m_groups.find(groupid);
		if (i!=_m_groups.end())
			return i->second;
		return 0;
	}
	item_t* get_or_new_group(char const* groupid)
	{
		size_t errorid = 0;
		item_t* item = get_group(groupid,&errorid);
		if (item) return item;
		if (errorid!=0) return 0;
		if (groupid[1]=='1')
			item = _m_allocator._new<item_multi_t>(_m_allocator);
		else
			item = _m_allocator._new<item_pool_t>(_m_allocator);
		if (!item) return 0;
		_m_groups.insert(groups_t::value_type(groupid,item));
		return item;
	}
	item_t* enter_group(queue_thread_t* thread,char const* groupid)
	{
		item_t* item = get_or_new_group(groupid);
		if (!item) return 0;
		item->add(thread);
		return item;
	}
	item_t* leave_group(queue_thread_t* thread,char const* groupid)
	{
		item_t* item = get_group(groupid);
		if (!item) return 0;
		item->remove(thread);
		return item;
	}
	item_t* move(queue_thread_t* thread,char const* groupid_from,char const* groupid_to)
	{
		item_t* item_from = get_group(groupid_from);
		item_t* item_to = get_group(groupid_to);
		if (!item_from || !item_to) return 0;
		item_from->remove(thread);
		item_to->add(thread);
		return item_to;
	}
	/// "t1":"multi-group", "t2":"pool-group"
	static group_type_enum group_type(char const* groupid)
	{
		bool is_valid = groupid && groupid[0] && groupid[1] && groupid[2];
		if (!is_valid) return __group_type_null;
		if (groupid[0]!='t' &&  (groupid[1]!='1' || groupid[1]!='2'))
			return __group_type_null;
		return groupid[1]=='1'?__group_type_multi:__group_type_pool;
	}
	allocator_t _m_allocator;
	groups_t _m_groups;
};


struct command_call
{
	void operator()()
	{
		//
	}
};

struct services_t
{
	typedef std::map<size_t,service_t> _services_t;
	service_t* create_service(size_t serviceid)
	{
		service_t& service = (_m_services[serviceid] = service_t());
		return &service;
	}
	void destroy_service(size_t serviceid)
	{
		_m_services.erase(serviceid);
	}
	service_t* get_service(size_t serviceid)
	{
		_services_t::iterator i = _m_services.find(serviceid);
		if (i==_m_services.end())
			return 0;
		return &i->second;
	}
	_services_t _m_services;
};

struct kernel
{
	typedef ox::run_tt<void> run_t;

	service_t* create_service(size_t serviceid)
	{
		return _m_services.create_service(serviceid);
	}
	void destroy_service(size_t serviceid)
	{
		_m_services.destroy_service(serviceid);
	}
	void add_cmd(size_t serviceid,size_t cmdid,run_t* run)
	{
		service_t* service = _m_services.get_service(serviceid);
		if (!service) return;
		service->add_function(cmdid,run);
	}
	void remove_cmd(size_t serviceid,size_t cmdid)
	{
		service_t* service = _m_services.get_service(serviceid);
		if (!service) return;
		service->remove_function(cmdid);
	}
	template <typename func_tn>
	void get_function(size_t serviceid,size_t cmdid,func_tn& fun)
	{
		fun.clear();
		services_t::iterator i = _m_services.find(serviceid);
		if (i==_m_services.end())
			return;
		service_t& service = i->second;
	}
	services_t _m_services;
	are_t _m_are;
};


___namespace_end()