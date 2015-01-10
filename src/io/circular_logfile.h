
/*
	copyright:  you can freely copy, reuse and distribute the code, 
	            but should add the head comment in front of your file.
	---------------------------------------------------------------------
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@msn.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	Hobby    :  Playing Table Tenis, Playing China Chess, Speaking English, etc.
	            Programing, Linux, C++, Network, etc.
	P.S.     :  You can contract me at any time.
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "../pkg/time.h"
#include "../pkg/string_simple_kit.h"
//#include "../pkg/string_convert.h"
#include "logfile.h"

namespace ox
{
	namespace io
	{
		template <typename character_tn>
		class circular_logfile
		{
		public:
			typedef character_tn character;
			typedef logfile<character> logfile_t;
			typedef std::basic_string<character> std_string;
			typedef ox::string::string_kit_traits<character> skit_trait;
			typedef ox::string::string_simple_kit<character> strkit;


			circular_logfile()
			{
				_m_maxsize = 10*1024;
			}

			circular_logfile(const character * filepath,size_t maxsize=16*1024) 
			{
				_m_logfile.open(filepath);
			}

			virtual ~circular_logfile()
			{
				close();
			}

		public:
			bool open(const character * filepath,size_t maxsize=16*1024)
			{
				_m_maxsize = maxsize;
				return _m_logfile.open(filepath,true,true);
			}

			void set_maxsize(size_t file_maxsize)
			{
				_m_maxsize = file_maxsize;
			}

			void seek_to_end()
			{
				_m_logfile.seek_to_end();
			}

			void log(const character * str, const character * ln = 0)
			{
				if (!str) return;
				//assert(_m_logfile.isopen());
				if (!_m_logfile.file().isopen()) return;
				size_t str_size = strkit::length(str);
				size_t size = _m_logfile.file().length();
				if (size+str_size>_m_maxsize)
					keep_latter_half();
				_m_logfile.log(str,ln);
			}

			void logformat(character const* fmt,va_list arglist,character const* ln)
			{
				if (!fmt) return;
				//assert(_m_logfile.isopen());
				if (!_m_logfile.file().isopen()) return;
				size_t str_size = strkit::length(fmt);
				size_t size = _m_logfile.file().length();
				if (size+str_size>_m_maxsize)
					keep_latter_half();
				_m_logfile.vprintf(fmt,arglist,ln);
			}

			logfile_t& logfile()
			{
				return _m_logfile;
			}

			void close()
			{
				_m_logfile.close();
			}
		private:
			void keep_latter_half()
			{
				std_string all;
				_m_logfile.file().read_all(all);
				all.erase(0,all.size()/2);
				_m_logfile.file().truncate();
				_m_logfile.log(all.c_str());
			}
		private:
			size_t _m_maxsize;
			logfile_t _m_logfile;
		}; /// end of circular log file
	} /// end of io
} /// end of ox

//#endif // !defined(AFX_LOGFILE_H__A0142287_8209_4094_BFF1_2EFAE882BFE3__INCLUDED_)
