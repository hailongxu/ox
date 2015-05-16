


#include "../ox/nsab.h"
#include "log_proxy_decl.h"
#include "../../include/ox/pkg/string_simple_kit.h"


#pragma once


___namespace2_begin(ox,comm)


struct log_proxy
{
	static log_proxy& instance()
	{
		static log_proxy _static_log;
		return _static_log;
	}
	void printf (const char * format, ...)
	{
		if (_m_log.is_empty()) return;
		typedef ox::str::string_simple_kit<char> strkit;
		std::string out;
		int size = 0;
		va_list arglist;
		va_start(arglist, format);
		size = strkit::vsprintf(out,format,arglist);
		va_end(arglist);
		_m_log(out.c_str());
	}
	void puts(std::string const& str)
	{
		if (_m_log.is_empty()) return;
		_m_log(str.c_str());
	}
	communication_public::logf_d& logf()
	{
		return _m_log;
	}
private:
	log_proxy(){}
	communication_public::logf_d _m_log;
};


___namespace2_end()