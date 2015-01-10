// LogFile.h: interface for the CLogFile class.
//
//////////////////////////////////////////////////////////////////////

/*
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	时间     :  2008-1-14
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "windows.h"
#include "logdef.h"
#include "file.h"

namespace ox
{
	namespace io
	{
		template <typename character_tn>
		struct logconsole
		{
			typedef ox::io::file<character_tn> stdfile;
			typedef typename stdfile::stdfile_trait_t cfile_trait;
			typedef typename stdfile::character character;
			typedef ox::string::string_kit_traits<character> skit_trait;

			static const character char_return_const = skit_trait::to_static_character<char,'\n'>::character_value;
			static const character char_a_const = skit_trait::to_static_character<char,'a'>::character_value;
			static const character char_w_const = skit_trait::to_static_character<char,'w'>::character_value;
			static const character char_t_const = skit_trait::to_static_character<char,'t'>::character_value;

			bool open()
			{
				if (_m_logfile.isopen()) return true;
				return reopen();
			}

			bool reopen()
			{
				character log_filename[] =
				{
					skit_trait::to_static_character<char,'C'>::character_value,
					skit_trait::to_static_character<char,'O'>::character_value,
					skit_trait::to_static_character<char,'N'>::character_value,
					skit_trait::to_static_character<char,'O'>::character_value,
					skit_trait::to_static_character<char,'U'>::character_value,
					skit_trait::to_static_character<char,'T'>::character_value,
					skit_trait::to_static_character<char,'$'>::character_value,
					skit_trait::null_character
				};
				character log_flag[] =
				{
					skit_trait::to_static_character<char,'w'>::character_value,
					skit_trait::null_character
				};
				return _m_logfile.reopen(log_filename,log_flag,stdout);
			}

			void seek_to_end()
			{
				_m_logfile.seek(file_helper::__seek_end,0);
			}

			void log(const character c)
			{
				assert(_m_logfile.isopen());
				seek_to_end();
				_m_logfile.putc(c);
				_m_logfile.flushall();
			}

			void log(const character* id, const character * str, const character * ln = 0)
			{
				assert(_m_logfile.isopen());
				seek_to_end();
				if (id)
				{
					_m_logfile.puts(L"[");
					_m_logfile.puts(id);
					_m_logfile.puts(L"] ");
				}
				_m_logfile.puts(str);
				if (ln && *ln) _m_logfile.puts(ln);
				_m_logfile.flushall();
				seek_to_end();
			}

			int log_v (const character* id, const character * format, va_list arglist, const character* ln = 0)
			{
				seek_to_end();
				if (id)
				{
					_m_logfile.puts(L"[");
					_m_logfile.puts(id);
					_m_logfile.puts(L"] ");
				}
				int ret = _m_logfile.vprintf(format, arglist);
				if (ln && *ln) _m_logfile.puts(ln);
				return ret;
			}

			void logformat (const character* id, const character * format, ...)
			{
				seek_to_end();
				va_list arglist;
				va_start(arglist, format);
				_m_logfile.vprintf(id,format, arglist);
				_m_logfile.flushall();
			}

			void logformatln (const character* id, const character * format, ...)
			{
				seek_to_end();
				va_list arglist;
				va_start(arglist, format);
				_m_logfile.vprintf(id,format, arglist,L"\n");
			}

			void log_space()
			{
				seek_to_end();
				assert(_m_logfile.isopen());
				log(skit_trait::to_static_character<char,' '>::character_value);
			}

			void log_pretext(logpretext_enum mode)
			{
				if (mode == __logpretext_null) return;

				seek_to_end();

				if (mode & __logpretext_compile_time)
				{
					log(skit_trait::to_static_character<char,'['>::character_value);
					character buffer [16];
					ox::string::conv::to_string(__DATE__,buffer);
					log(0,buffer);
					log(skit_trait::to_static_character<char,' '>::character_value);
					ox::string::conv::to_string(__TIME__,buffer);
					log(0,buffer);
					log(skit_trait::to_static_character<char,']'>::character_value);
					log(skit_trait::to_static_character<char,' '>::character_value);
				}

				//typedef ox::pkg::datetime_t<character> time_t;
				//time_t time;
				//time.assign_from_now();
				//tm * t = time.to_tm();
				character fmt_buffer[] =
				{
					 skit_trait::to_static_character<char,'['>::character_value,
					 skit_trait::to_static_character<char,'%'>::character_value,
					 skit_trait::to_static_character<char,'d'>::character_value,
					 skit_trait::to_static_character<char,'-'>::character_value,
					 skit_trait::to_static_character<char,'%'>::character_value,
					 skit_trait::to_static_character<char,'d'>::character_value,
					 skit_trait::to_static_character<char,'-'>::character_value,
					 skit_trait::to_static_character<char,'%'>::character_value,
					 skit_trait::to_static_character<char,'d'>::character_value,
					 skit_trait::to_static_character<char,' '>::character_value,
					 skit_trait::to_static_character<char,'%'>::character_value,
					 skit_trait::to_static_character<char,'0'>::character_value,
					 skit_trait::to_static_character<char,'2'>::character_value,
					 skit_trait::to_static_character<char,'d'>::character_value,
					 skit_trait::to_static_character<char,':'>::character_value,
					 skit_trait::to_static_character<char,'%'>::character_value,
					 skit_trait::to_static_character<char,'0'>::character_value,
					 skit_trait::to_static_character<char,'2'>::character_value,
					 skit_trait::to_static_character<char,'d'>::character_value,
					 skit_trait::to_static_character<char,':'>::character_value,
					 skit_trait::to_static_character<char,'%'>::character_value,
					 skit_trait::to_static_character<char,'0'>::character_value,
					 skit_trait::to_static_character<char,'2'>::character_value,
					 skit_trait::to_static_character<char,'d'>::character_value,
					 skit_trait::to_static_character<char,'.'>::character_value,
					 skit_trait::to_static_character<char,'%'>::character_value,
					 skit_trait::to_static_character<char,'0'>::character_value,
					 skit_trait::to_static_character<char,'3'>::character_value,
					 skit_trait::to_static_character<char,'d'>::character_value,
					 skit_trait::to_static_character<char,']'>::character_value,
					 skit_trait::to_static_character<char,'\0'>::character_value,
				};

				character* fmt = fmt_buffer;
				character buf[64] = {0};
				typedef ox::string::string_simple_kit<character> strkit;

				SYSTEMTIME sys;
				GetLocalTime( &sys );

				if (mode & __logpretext_date && mode & __logpretext_time)
				{
					strkit::sprintf(buf,fmt,
						sys.wYear,//t->tm_year+1900,
						sys.wMonth,//t->tm_mon+1,
						sys.wDay,//t->tm_mday,
						sys.wHour,//t->tm_hour,
						sys.wMinute,//t->tm_min,
						sys.wSecond,//t->tm_sec);
						sys.wMilliseconds);
				}
				else if (mode & __logpretext_date)
				{
					fmt_buffer [9] = skit_trait::to_static_character<char,']'>::character_value;
					fmt_buffer [10] = skit_trait::to_static_character<char,'\0'>::character_value;
					strkit::sprintf(buf,fmt, sys.wYear,sys.wMonth,sys.wDay);
				}
				else if (mode & __logpretext_time)
				{
					fmt_buffer [9] = skit_trait::to_static_character<char,'['>::character_value;
					fmt = &fmt_buffer [9];
					strkit::sprintf(buf,fmt, sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
				}

				log(0,buf);
			}


			void close()
			{
				_m_logfile.close();
			}

		private:
			stdfile _m_logfile;
		};
	} /// end of io
} /// end of ox

//#endif // !defined(AFX_LOGFILE_H__A0142287_8209_4094_BFF1_2EFAE882BFE3__INCLUDED_)
