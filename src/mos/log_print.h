

#include "../io/logfile.h"
#include "../utl/path_kit.h"
#include "winop.h"


#pragma once


namespace ox
{
	struct communication_log
	{
		static std::string get_log_path()
		{
			typedef ox::utl::localpath_kit<char> kit;
			std::string exepath = sys::get_current_exe_path<char>();
			std::string dllpath = sys::get_current_module_path<char>();
			SYSTEMTIME st;
			GetLocalTime(&st);
			char time_buffer[32];
			sprintf(time_buffer,"%04u'%02u'%02u-%02u'%02u'%02u'%02u",
				st.wYear,st.wMonth,st.wDay,
				st.wHour,st.wMinute,st.wSecond,st.wMilliseconds
				);
			DWORD pid = GetCurrentProcessId();
			char str_pid[16];
			sprintf(str_pid,"%u",pid);
			
			kit::keep_last(dllpath);
			
			std::string path;
			path.append(exepath);kit::remove_last(path);
			path.append(dllpath);
			path.append("+t");
			path.append(time_buffer);
			path.append("+");kit::keep_last(exepath);
			path.append(exepath);
			path.append("+pid");
			path.append(str_pid);
			path.append(".log");
			return path;
		}
		communication_log(bool is_create=false)
		{
			if (is_create) init();
		}
		void init()
		{
			std::string logfilepath = get_log_path();
			_m_log.open(logfilepath.c_str());
		}
		void logln(char const* str)
		{
			std::string strln(str);
			strln.append("\r\n");
			_m_log.log(strln.c_str());
		}
		void log(char const* str)
		{
			char buff[64] = {0};
			_m_log.log_pretext(buff,ox::io::__logpretext_datetime);
			char bufftid[32] = {0};
			_m_log.log_pretext_number(bufftid,GetCurrentThreadId());
			std::string line(buff);
			line.append(bufftid);
			line.append(" ");
			line.append(str);
			_m_log.log(line.c_str());
		}
		ox::io::logfile<char> _m_log;
	};
}