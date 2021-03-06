

#include "../ox/nsab.h"
#include "../io/circular_logfile.h"
#include "../mos/win_queue_thread.h"


#pragma once


___namespace2_begin(ox,comm)


struct alog_t
{
	typedef abc::win_queue_thread thread_t;
	ox::io::circular_logfile<char> _m_log;
	thread_t _m_thread;

	void start(char const* path,size_t maxsize=64*1024)
	{
		_m_log.open(path,maxsize);
		_m_thread.start();
	}
	void stop()
	{
		_m_thread.stop_next();
		_m_log.close();
	}
	thread_t::exit_d& on_exit()
	{
		return _m_thread.on_exit();
	}
	void log(char const* str,char const* ln=0)
	{
		std::string str1,str2;
		if (str) str1.assign(str);
		if (ln) str2.assign(ln);
		_m_thread.add(abc::task_single<int>::make(this,&alog_t::do_log,str1,str2));
	}
	void do_log(std::string const& str,std::string const& ln)
	{
		_m_log.logfile().log_pretext(ox::io::__logpretext_datetime);
		_m_log.log(str.c_str(),ln.c_str());
	}
}; /// end of alog


___namespace2_end()