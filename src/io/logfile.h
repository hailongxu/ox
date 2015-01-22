
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
#include "../pkg/string_convert.h"
#include "logdef.h"
#include "file.h"

namespace ox
{
	namespace io
	{
		template <typename character_tn>
		class logfile
		{
		public:
			typedef ox::io::file<character_tn> stdfile;
			typedef typename stdfile::character character;
			typedef ox::str::string_kit_traits<character> skit_trait;

			static const character char_return_const = skit_trait::to_static_character<char,'\n'>::character_value;
			static const character char_a_const = skit_trait::to_static_character<char,'a'>::character_value;
			static const character char_w_const = skit_trait::to_static_character<char,'w'>::character_value;
			static const character char_t_const = skit_trait::to_static_character<char,'t'>::character_value;

			logfile()
			{}

			logfile(const character * filepath, bool isappend=true,bool isread=false) 
			{
				open (filepath,isappend,isread);
			}

			virtual ~logfile()
			{
				close();
			}

		public:
			bool open(const character * filepath, bool isappend=true,bool isread=false)
			{
				character log_open_flag[4]= {0,0,0,0};
				size_t i = 0;
				log_open_flag[i++] = isappend?'a':'w';
				if (isread) log_open_flag[i++] = '+';
				log_open_flag[i] = char_t_const;
				if(_m_logfile.isopen())
					_m_logfile.close();
				return _m_logfile.open(filepath, log_open_flag);
			}

			stdfile& file() {return _m_logfile;}
			stdfile const& file() const {return _m_logfile;}

			void seek_to_end()
			{
				_m_logfile.seek(file_helper::__seek_end,0);
			}

			void log(const character c)
			{
				//assert(_m_logfile.isopen());
				if (!_m_logfile.isopen()) return;
				seek_to_end();
				_m_logfile.putc(c);
				_m_logfile.flush();
				seek_to_end();
			}

			void log(const character * str, const character * ln = 0)
			{
				//assert(_m_logfile.isopen());
				if (!_m_logfile.isopen()) return;
				seek_to_end();
				_m_logfile.puts(str);
				if (ln && *ln) _m_logfile.puts(ln);
				_m_logfile.flush();
				seek_to_end();
			}

			void logformat (const character * format, ...)
			{
				if (!_m_logfile.isopen()) return;
				seek_to_end();
				va_list arglist;
				va_start(arglist, format);
				vprintf (format, arglist);
				_m_logfile.flush();
				seek_to_end();
			}

			void logformatln (const character * format, ...)
			{
				if (!_m_logfile.isopen()) return;
				seek_to_end();
				va_list arglist;
				va_start(arglist, format);
				vprintf (format, arglist);
				log (char_return_const);
				seek_to_end();
			}

			void log_space()
			{
				if (!_m_logfile.isopen()) return;
				seek_to_end();
				assert(_m_logfile.isopen());
				log(skit_trait::to_static_character<char,' '>::character_value);
				seek_to_end();
			}

			static void log_pretext_number(character* buf,size_t number)
			{
				typedef ox::str::string_simple_kit<character> strkit;
				character fmt_number[] = {'[','%','u',']','\0'};
				strkit::sprintf(buf,fmt_number,number);
			}
			static void log_pretext(character* buf,logpretext_enum mode=__logpretext_null)
			{
				if (buf==0) return;
				if (mode & __logpretext_compile_time)
				{
					//log(skit_trait::to_static_character<char,'['>::character_value);
					//character buffer [16];
					//ox::str::conv::to_string(__DATE__,buffer);
					//log(buffer);
					//log(skit_trait::to_static_character<char,' '>::character_value);
					//ox::str::conv::to_string(__TIME__,buffer);
					//log(buffer);
					//log(skit_trait::to_static_character<char,']'>::character_value);
					//log(skit_trait::to_static_character<char,' '>::character_value);
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

				size_t length = 0;
				character* fmt = fmt_buffer;
				//character buf[64] = {0};
				typedef ox::str::string_simple_kit<character> strkit;

				SYSTEMTIME sys;
				GetLocalTime( &sys );

				if (mode & __logpretext_date && mode & __logpretext_time)
				{
					length = strkit::sprintf(buf,fmt,
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
					length = strkit::sprintf(buf,fmt, sys.wYear,sys.wMonth,sys.wDay);
				}
				else if (mode & __logpretext_time)
				{
					fmt_buffer [9] = skit_trait::to_static_character<char,'['>::character_value;
					fmt = &fmt_buffer [9];
					length = strkit::sprintf(buf,fmt, sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
				}
			}
			void log_pretext(logpretext_enum mode)
			{
				if (!_m_logfile.isopen()) return;
				if (mode == __logpretext_null) return;

				seek_to_end();
				character buf[64] = {0};
				log_pretext(buf,mode);
				log(buf);
			}

			int vprintf (const character * format, va_list arglist, const character* ln = 0)
			{
				if (!_m_logfile.isopen()) return 0;
				seek_to_end();
				int ret = _m_logfile.vprintf(format, arglist);
				if (ln && *ln) _m_logfile.puts(ln);
				return ret;
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
