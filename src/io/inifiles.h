
/*
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	时间     :  2008-1-14

*/



#include "../utl/path_kit.h"
#include "inifile.h"



#pragma once



namespace ox
{
	namespace io
	{
		template <typename character_tn>
		struct idtopath_tt
		{
			typedef ox::utl::localpath_kit<character_tn> pathkit;
			typedef typename pathkit::std_string std_string;
			typedef character_tn character;

			idtopath_tt()
				: _m_dir(0)
				, _m_prefilename(0)
			{}

			character const* operator()(character const* id)
			{
				if (_m_dir) _m_path = _m_dir;
				pathkit::add_rpath(_m_path,_m_prefilename,(character)'\\');
				if (id && *id)
				{
					//pathkit::add_rpath(_m_path,id,(character)'\\');
					if (_m_prefilename&&*_m_prefilename) _m_path.push_back((character)'.');
					_m_path.append(id);
				}
				character ext[] = {(character)'.',(character)'i',(character)'n',(character)'i',0};
				_m_path.append(ext);
				return _m_path.c_str();
			}
			character const* _m_dir;
			character const* _m_prefilename;
			std_string _m_path;
		};

		template <typename character_tn, typename idtopath_tn>
		struct inifiles
		{
			typedef idtopath_tn idtopath_type;
			typedef character_tn character;
			typedef ox::string::string_kit_traits<character> skit_trait;
			typedef inifile<character> inifile_type;
			typedef typename inifile_type::std_string std_string;
			typedef ox::shell::shell_kit<character_tn> shkit;
			struct record
			{
				record() { file=0; berased=false; }
				inifile_type* file;
				bool berased;
			};
			typedef std::map<std_string,record> inifiles_type;
			typedef typename inifiles_type::iterator I;
			
			inifiles()
			{
				_m_is_load = false;
			}

			idtopath_type& idtopath() { return _m_idtopath; }

			inifile_type* open(character const* id)
			{
				return open(id, _m_is_load);
			}

			inifile_type* open(character const* id, bool isload)
			{
				character nullid [] =
				{
					skit_trait::to_static_character<char,'\0'>::character_value,
					0
				};

				if (!id) id = nullid;
				inifile_type* file = find_file(id);
				if (file) return file;
				_m_inifiles[id] = record();
				record& r = _m_inifiles[id];
				r.file = new inifile_type;
				character const* path = _m_idtopath(id);
				if (!path || !*path) return 0;
				if (!r.file->open(path,isload)) return 0;
				return r.file;
			}

			inifile_type* find_file(character const* id)
			{
				assert (id);
				I i = _m_inifiles.find(id);
				if (i!=_m_inifiles.end())
					return i->second.berased ? 0 : i->second.file;
				return 0;
			}

			record* find_record(character const* id)
			{
				assert (id);
				I i = _m_inifiles.find(id);
				if (i!=_m_inifiles.end())
					return &i->second;
				return 0;
			}

			bool is_empty(character const* id) const
			{
				inifile_type* f = find_file(id);
				if (!f) return true;
				return f->is_empty();
			}

			void load(character const* id)
			{
				inifile_type* f = find_file(id);
				if (!f) return;
				f->load();
			}

			size_t erase(character const* id = 0)
			{
				if (!id)
				{
					for (I i=_m_inifiles.begin(); i!=_m_inifiles.end(); ++i)
						i->second.berased = true;
					return _m_inifiles.size();
				}
				else if (*id)
				{
					record* r = find_record(id);
					if (!r->file) return 0;
					r->berased = true;
					return 1;
				}
				return 0;
			}

			void commit(character const* id = 0)
			{
				if (!id)
				{
					for (I i=_m_inifiles.begin(); i!=_m_inifiles.end();)
					{
						if (i->second.berased)
						{
							i->second.file->close();
							shkit::rm(i->second.file->filepath().c_str());
							i->second.file = 0;
							i = _m_inifiles.erase(i);
						}
						else
						{
							i->second.file->commit();
							++i;
						}
					}
				}
				else if (*id)
				{
					I i = _m_inifiles.find(id);
					if (i==_m_inifiles.end()) return;

					record* r = &i->second;
					if (!r) return;
					if (!r->file) return;
					if (r->berased)
					{
						r->file->close();
						shkit::rm(r->file->filepath().c_str());
						_m_inifiles.erase(i);
					}
					else
						r->file->commit();
				}
			}

			void clear(character const* id = 0)
			{
				if (!id)
				{
					for (I i=_m_inifiles.begin(); i!=_m_inifiles.end(); ++i)
					{
						i->second.file->close();
						delete i->second.file;
						i->second.file = 0;
					}
					_m_inifiles.clear();
				}
				else if (*id)
				{
					I i = _m_inifiles.find(id);
					if (i==_m_inifiles.end()) return;
					i->second.file->close();
					delete i->second.file;
					i->second.file = 0;
					_m_inifiles.erase(i);
				}
			}

			void close(character const* id = 0)
			{
				clear(id);
			}

			inifiles_type _m_inifiles;
			idtopath_type _m_idtopath;
			bool _m_is_load;
		};
	} /// end of io
} /// end of ox

