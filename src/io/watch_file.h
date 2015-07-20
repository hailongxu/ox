

#include <string>
#include "win_shell_kit.h"
#include "../cxx/delegate.h"
#include "../mos/win_queue_thread_with_timer.h"
#include "../mos/task_feedback.h"
#include "../utl/destructor.h"


#pragma once



namespace ox
{
namespace io
{

struct watch
{
	typedef watch self;
	/// last-size
	typedef delegate<void(char const* file,size_t,size_t)> file_changed_d;
	struct item_t
	{
		size_t id;
		std::string path;
		FILETIME last_time;
		size_t interval_ms;
		size_t last_size;
		file_changed_d on_event;
	};
	typedef std::vector<item_t> files_t;

	self()
	{
		_m_id = 0;
	}
	void add(char const* path,file_changed_d const& on_event,size_t interval_ms)
	{
		_m_thread.add(ox::task_single<int>::make(this,&self::do_add,std::string(path),on_event,interval_ms));
	}

	void start()
	{
		_m_thread.start();
	}
	void stop()
	{
		_m_thread.stop();
	}

	bool on_timer(size_t id,LONGLONG period_micro_second,void const* bind)
	{
		size_t fid = (size_t)bind;
		item_t* pitem = find(fid);
		if (!pitem)
		{
			assert (false);
			return false;
		}
		item_t& item = *pitem;
		FILETIME write_time;
		HANDLE h = ox::shell::win_open_existing_file(item.path.c_str());
		if (h==INVALID_HANDLE_VALUE)
		{
			assert (false);
			return false;
		}
		ox::utl::win_handle_defer defer(h);
		bool b = ox::shell::win_get_filetime(h,0,&write_time,0);
		if (!b)
		{
			assert (false);
			return false;
		}

		///test
		//static size_t i = 0;
		//SYSTEMTIME st1,st2;
		//ox::shell::file_time_to_local_system(write_time,st2);
		//ox::shell::file_time_to_local_system(item.last_time,st1);
		//printf ("%u : last:(%u,%u) now(%u,%u)\n",++i,st1.wSecond,st1.wMilliseconds,st2.wSecond,st2.wMilliseconds);

		if (0==ox::shell::compare(write_time,item.last_time))
			return true;
		item.last_time = write_time;
		size_t last_size = item.last_size;
		item.last_size = ox::shell::win_get_file_size(h);
		if (item.on_event.is_empty())
		{
			assert (false);
			return false;
		}
		item.on_event(item.path.c_str(),last_size,item.last_size-last_size);
		return true;
	}

	void do_add(std::string& path,file_changed_d const& on_event,size_t interval_ms)
	{
		item_t item;
		item.id = ++_m_id;
		item.path = path;
		HANDLE h = ox::shell::win_open_existing_file(path.c_str(),false,true);
		if (h==INVALID_HANDLE_VALUE)
		{
			item.last_size = 0;
			item.last_time.dwHighDateTime = 0;
			item.last_time.dwLowDateTime = 0;
		}
		else
		{
			ox::utl::win_handle_defer defer(h);
			bool b = ox::shell::win_get_filetime(h,0,&item.last_time,0);
			if (!b) return;
			item.last_size = ox::shell::win_get_file_size(h);
		}
		
		item.interval_ms = interval_ms;
		item.on_event = on_event;
		_m_files.push_back(item);
		_m_thread.add_timer_milli(interval_ms,ox::win_high_timer::event_d(this,&self::on_timer),(void const*)(item.id));
	}

	item_t* find(size_t id)
	{
		for (size_t i=0;i<_m_files.size();++i)
		{
			bool b = _m_files[i].id==id;
			if (!b) continue;
			return &_m_files[i];
		}
		return 0;
	}
	ox::win_queue_thread_with_timer _m_thread;
	files_t _m_files;
	size_t _m_id;
};



}
}