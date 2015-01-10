
/*
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	时间     :  2008-1-14

*/



#include "../ox/utl/path_kit.h"
#include "logdef.h"
#include "logfile.h"



#pragma once



namespace ox
{
	namespace io
	{
		template <typename character_tn, typename idtopath_tn>
		struct logfiles
		{
			typedef idtopath_tn idtopath_type;
			typedef character_tn character;
			typedef ox::string::string_kit_traits<character> skit_trait;
			typedef logfile<character> logfile_type;
			typedef typename logfile_type::skit_trait::std_string std_string;
			typedef std::map<std_string,logfile_type> logfiles_type;
			typedef typename logfiles_type::iterator I;
			
			logfiles()
			{
				_m_is_appended = false;
				_m_pretext_mode = __logpretext_time;
			}

			void log_appended(bool appended = false) { _m_is_appended = appended; }

			idtopath_type& idtopath() { return _m_idtopath; }

			logfile_type* open(character const* id)
			{
				return open(id, _m_is_appended);
			}

			logfile_type* open(character const* id, bool isappend)
			{
				character nullid [] =
				{
					skit_trait::to_static_character<char,'\0'>::character_value,
					0
				};

				if (!id) id = nullid;
				logfile_type* file = find_logfile(id);
				if (file) return file;
				_m_logfiles[id] = logfile_type();
				logfile_type& f = _m_logfiles[id];
				character const* path = _m_idtopath(id);
				if (!path || !*path) return 0;
				if (!f.open(path,isappend)) return 0;
				return &f;
			}

			void log(logpretext_enum mode, character const* id, character const* str, character const* ln = 0)
			{
				logfile_type* f = open(id);
				if (!f) { assert(false); return; }
				f->seek_to_end();
				if (mode & __logpretext_ignore) mode = _m_pretext_mode;
				if (mode) f->log_pretext(mode),f->log_space();
				f->log(str,ln);
				f->seek_to_end();
			}

			void vprintf (logpretext_enum mode, character const* id, const character * format, va_list arglist, character const* ln = 0)
			{
				logfile_type* f = open(id);
				if (!f) { assert(false); return; }

				f->seek_to_end();
				if (mode & __logpretext_ignore) mode = _m_pretext_mode;
				if (mode) f->log_pretext(mode),f->log_space();
				f->vprintf(format, arglist, ln);
				f->seek_to_end();
			}

			void logformat (logpretext_enum mode, character const* id, const character * format, ...)
			{
				logfile_type* f = open(id);
				if (!f) { assert(false); return; }

				f->seek_to_end();
				if (mode & __logpretext_ignore) mode = _m_pretext_mode;
				if (mode) f->log_pretext(mode),f->log_space();
				va_list arglist;
				va_start(arglist, format);
				f->vprintf(format, arglist);
				f->flushall();
				f->seek_to_end();
			}

			void logformatln (logpretext_enum mode, const character * format, ...)
			{
				logfile_type* f = open(id);
				if (!f) { assert(false); return; }

				f->seek_to_end();
				va_list arglist;
				va_start(arglist, format);
				if (mode & __logpretext_ignore) mode = _m_pretext_mode;
				if (mode) f->log_pretext(mode),f->log_space();
				character ln[] =
				{
					skit_trait::to_static_character<char,'\n'>::character_value,
					0
				};
				f->vprintf(format, arglist, ln);
				f->flushall();
				f->seek_to_end();
			}

			logfile_type* find_logfile(character const* id)
			{
				assert (id);
				I i = _m_logfiles.find(id);
				if (i!=_m_logfiles.end())
					return &(i->second);
				return 0;
			}

			logfiles_type _m_logfiles;
			idtopath_type _m_idtopath;
			logpretext_enum _m_pretext_mode;
			bool _m_is_appended;
		};
	} /// end of io
} /// end of ox

