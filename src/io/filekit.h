

#include <map>
#include "../cxx/delegate.h"
#include "../pkg/string_simple_kit.h"
#include "file.h"
#include "directory.h"



namespace ox
{
	namespace io
	{
		template <typename character_tn>
		bool read_file_into_string(std::basic_string<character_tn>& buffer,std::basic_string<character_tn> const& filepath,size_t off=0,size_t length=-1,long* filebytes=0)
		{
			typedef character_tn character;
			character const flag[3] = {'r','b',0};
			/// read the file into wide buffer
			ox::io::file<character> ffile;
			bool b = ffile.open(filepath.c_str(),flag);
			if (!b) return false;
			long len = ffile.length();
			if (filebytes) *filebytes = len;
			if (len==0) return true;
			assert(len%sizeof(character)==0);
			long wlen = (sizeof(character)==1?len+1:len)/sizeof(character);
			buffer.resize(wlen);
			if (off>=len) off = len-1;
			if (length==-1 || off+length>len) length = len-off;
			ffile.seek(ox::io::file_helper::__seek_begin,off);
			size_t read = ffile.read((character*)buffer.data(),1,length);
			assert (read==length);
			if (read==0) return false;
			return true;
		}

		template <typename character_tn>
		bool read_file_into_string(std::basic_string<character_tn> const& filepath,std::basic_string<character_tn>& buffer,long* filebytes=0)
		{
			typedef character_tn character;
			character const flag[3] = {'r','b',0};
			/// read the file into wide buffer
			ox::io::file<character> ffile;
			bool b = ffile.open(filepath.c_str(),flag);
			if (!b) return false;
			long len = ffile.length();
			if (filebytes) *filebytes = len;
			if (len==0) return false;
			assert(len%sizeof(character)==0);
			long wlen = (len+sizeof(character)-1)/sizeof(character);
			buffer.resize(wlen);
			size_t read = ffile.read((character*)buffer.data(),1,len);
			assert (read==len);
			if (read==0) return false;
			return true;
		}

		template <typename character_tn>
		size_t append_buffer_to_file(std::basic_string<character_tn> const& filepath, character_tn const* buffer, size_t buffer_len)
		{
			typedef character_tn character;
			character const flag[3] = {'r','w',0};
			/// read the file into wide buffer
			ox::io::file<character> ffile;
			bool b = ffile.open(filepath.c_str(),flag);
			if (!b) return false;
			long len = ffile.length();
			if (filebytes) *filebytes = len;
			if (len==0) return false;
			assert(len%sizeof(character)==0);
			long wlen = (sizeof(character)==1?len+1:len)/sizeof(character);
			ffile.seek(ox::io::file<character>::helper::__seek_end,0);
			size_t lenwrite = ffile.write(buffer,sizeof(character),buffer_len);
			assert (lenwrite==buffer_len);
			ffile.flush();
			ffile.close();
			return lenwrite;
		}
		template <typename character_tn>
		struct foreach_fileline
		{
			typedef character_tn character;
			static ox::character::is_case_sensitive_c const casive = ox::character::__case_sensitive;
			typedef ox::utl::localpath_kit<character> pathkit;
			typedef ox::str::string_simple_kit<character> strkit;
			typedef typename strkit::std_string std_string;
			typedef ox::io::directory_kit<character> dirkit;
			typedef delegate<bool(character const*,character const*)> line_handle;

			bool operator()(std_string const& filepath,line_handle const& handle)
			{
				typedef strkit::get_line<casive> get_line;

				long len = 0;
				std_string str;
				if (!read_file_into_string<character>(filepath,str,&len))
					return false;

				//// prepare get-line
				character const* text = str.data();
				character const* text_end = text+str.length();
				if (*text==0xfeff) ++text;
				get_line getline(text,text_end);

				character const* end = 0;
				character const* begin = 0;

				while (true)
				{
					begin = getline(&end);
					if (!begin || !*begin) break;
					if (!handle(begin,end)) break;
				}

				return true;
			}
		};

		template <typename character_tn>
		struct fileline_match
		{
			typedef character_tn character;
			typedef ox::str::string_simple_kit<character> strkit;
			typedef ox::io::foreach_fileline<character> eachline_t;
			typedef typename eachline_t::line_handle line_handle;
			typedef typename eachline_t::std_string std_string;
			static ox::character::is_case_sensitive_c const casive = ox::character::__case_sensitive;
			typedef std::map<std_string,size_t> keymap;

			void init(character const* filepath)
			{
				line_handle handle(this,&fileline_match::on_line);
				eachline_t eachline;
				eachline(filepath,handle);
			}

			bool operator()(character const* str_begin,character const* str_end)
			{
				bool ret = false;
				std_string str(str_begin,str_end-str_begin);
				for (keymap::iterator i=_m_keymap.begin(); i!=_m_keymap.end();++i)
				{
					character const* key_begin = i->first.c_str();
					character const* key_end = i->first.c_str()+i->first.length();
					character const* str = strkit::strstr<casive>(str_begin,str_end,key_begin,key_end);
					if (str)
					{
						i->second ++;
						ret = true;
						//_m_keymap[i->first] = true;
						//_m_keymap_true[i->first] = true;
						//i = _m_keymap.erase(i);
					}
					//else
					//{
					//	++i;
					//}
				}
				return ret;
			}

			keymap _m_keymap;

		private:
			bool on_line(character const* begin,character const* end)
			{
				std_string key(begin,end-begin);
				if (key.empty()) return true;
				_m_keymap[key] = 0;
				return true;
			}
		};

		template <typename character_tn>
		struct cross_compare
		{
			typedef character_tn character;
			typedef ox::str::string_simple_kit<character> strkit;
			typedef typename strkit::std_string std_string;
			typedef std::set<std_string> ext_set;

			typedef ox::io::directory_kit<character> directory;
			typedef typename directory::finddata_t finddata_t;
			typedef typename directory::pathkit pathkit;
			typedef ox::io::foreach_fileline<character> eachline_t;
			typedef typename eachline_t::line_handle line_handle;
			typedef ox::io::fileline_match<character> fileline_match;

			typedef delegate<void(character const*,character const*,bool)> match_func;

			cross_compare()
				: _m_file_handle(_m_exts_ignore,_m_exts_care,_m_exts_met)
			{}

		private:
			struct fileline_handle
			{
				void init(cross_compare* all)
				{
					mother = all;
				}
				bool operator()(character const* str_begin,character const* str_end)
				{
					bool b = mother->keyfile_match(str_begin,str_end);
					if (!mother->on_line.is_empty())
						mother->on_line(str_begin,str_end,b);
					return true;
				}
				cross_compare* mother;
			};

			struct file_handle
			{
				file_handle(ext_set const& exts_ignore,ext_set const& exts_care,ext_set& all_exts)
					: _m_exts_ignore(exts_ignore)
					, _m_exts_care(exts_care)
					, _m_all_exts(all_exts)
				{}
				void init(cross_compare* all)
				{
					mother = all;
					_m_eachline_mode = false;
				}

				bool is_validext(character const* ext) const
				{
					if (_m_exts_care.empty())
						return true;
					if (_m_exts_care.end()==_m_exts_care.find(ext))
						return false;
					return true;
				}
				bool is_exclude_ext(character const* ext) const
				{
					if (_m_exts_ignore.end()==_m_exts_ignore.find(ext))
						return false;
					return true;
				}

				bool analyze_file(character const* filepath)
				{
					buffer.clear();
					if (!ox::io::read_file_into_string<character>(filepath,buffer))
						return false;
					bool b = mother->keyfile_match(buffer.c_str(),buffer.c_str()+buffer.length());
					if (mother->on_file.is_empty())
						mother->on_file(filepath,filepath+strkit::length(filepath),b);
					return true;
				}

				bool analyze_eachline(character const* filepath)
				{
					line_handle handle(&mother->_m_fileline_handle);
					eachline_t eachline;
					eachline(filepath,handle);
					return true;
				}

				bool analyze(character const* filepath)
				{
					if (_m_eachline_mode)
						return analyze_eachline(filepath);
					else
						return analyze_file(filepath);
				}

				size_t operator()(finddata_t const& fd,size_t index)
				{
					static size_t count = 0;
					count++;
					printf ("\n%6d,",count);
					for(size_t i=0;i<fd.depth;i++) printf ("   ");

					printf ("%s",fd.filename());
					if (fd.attribute()&directory::attr_directory)
						printf("/");

					std_string dir = fd.filedir;
					pathkit::remove_last(dir);
					pathkit::add_rpath(dir,fd.filename(),'\\');
					character const* ext = 0;
					bool hasext = pathkit::get_ext(fd.filename(),ext);
					if (!hasext) return 3;
					strkit::to_lower((character*)ext);

					_m_all_exts.insert(ext);
					if (is_exclude_ext(ext)) return 1;
					if (!is_validext(ext)) return 3;
					printf (" +\n");

					if (!analyze(dir.c_str()))
						printf ("   XXXXX");
					return 3;
				}

				cross_compare* mother;
				bool _m_eachline_mode;
				std_string buffer;
				ext_set const& _m_exts_ignore;
				ext_set const& _m_exts_care;
				ext_set& _m_all_exts;
			};

		public:
			void start(character const* keyfilepath,character const* targetpath,bool linemode)
			{
				keyfile_match.init(keyfilepath);

				_m_file_handle.init(this);
				_m_fileline_handle.init(this);
				_m_file_handle._m_eachline_mode = linemode;

				directory::processnode_func filehandle(&_m_file_handle);
				_m_directory.traverse(targetpath,filehandle);
			}

			match_func on_line;
			match_func on_file;
			ext_set _m_exts_ignore;
			ext_set _m_exts_care;
			ext_set _m_exts_met;
			fileline_match keyfile_match;
		private:
			directory _m_directory;
			file_handle _m_file_handle;
			fileline_handle _m_fileline_handle;
		};
	} /// io
} /// ox