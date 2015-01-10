
/**
* @file 读取常用的ini文件的操作
*
* 这个档案定义了ini文件的操作

* 比微软提供的功能多了一项，我认为很重哟啊
* 那就是可以删除ini文件中的某一配置项
* 对不符合ini文件的格式会自动删除和规范
* 注释有三种在行前加“；”，“#”，“//”
* 行前可以加空格和tab
* 【section】section可以使任意文字，但必须是可见
* ini文件的section和key是区分大小写的

* @author xu hailong
* @version 1.0
* @date 2007-5-29
* @data 2008-7-30 : 并到IO空间中
  版权：版权归作者所有，本文件可以自由传播
*/

#pragma once

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <list>
#include <utility>

#include "../def/def.h"
	/// 用到了 procedure_control_c

#include "../cxx/grammer.h"
#include "../cor/platform.h"
//#include "../cor/mm_line.h"
#include "../pkg/string_simple_kit.h"
#include "./file.h"
//#include "./stream_kit.h"
//#include "./application_set.h"

#pragma warning (disable: 4996)

namespace ox
{
	namespace io
	{
		namespace ___
		{
			template <typename character_tn>

			struct vector_file_struct_base
			{
				typedef ox::string::string_simple_kit<character_tn> strkit;
				typedef typename strkit::character character;
				typedef typename strkit::std_string std_string;
				typedef typename strkit::nstring nstring;
				typedef typename strkit::nstrings nstrings;
			};


			//template <typename character_tn>
			//struct vector_file_struct
			//{
			//	typedef vector_file_struct_base<character_tn> base;
			//	typedef character_tn character;
			//	typedef typename base::nstrings nstrings;
			//	nstrings _m_strings;
			//};
		}


		/**
		* @brief class inifile
		*
		* 封装了一些简单实用的操作
		* 写入条目
		* 读出条目
		* 删除条目
		*/

		template <typename character_tn>
		class vectorfile :
			public ___::vector_file_struct_base<character_tn>,
			protected ox::io::file<character_tn>
		{
			typedef ox::character::character_trait<character_tn> character_trait;
			typedef typename character_trait::character character;
			typedef typename strkit::std_string std_string;


			typedef ___::vector_file_struct_base<character> vector_file_struct_base;
			//typedef ___::vector_file_struct<character> vector_file_struct;
			typedef ox::io::file<character> stdfile;


			const character * get_ini_open_flag()
			{
				static const character ls_ini_open_flag [] =
				{
					strkit::to_static_character<char,'r'>::character_value,
					strkit::to_static_character<char,'b'>::character_value,
					strkit::null_character
				};
				return ls_ini_open_flag;
			}
		public:
			vectorfile()
			{
				_m_buffer = 0;
			}
			vectorfile(const character *filename, bool const & is_load = true)
			{
				open(filename,is_load);
			}
			~vectorfile()
			{
				delete [] _m_buffer;
				_m_buffer = 0;
				_m_strings.clear();
			}

			bool open (const character *filename, bool const & is_load = true)
			{
				memmove(_m_ini_file_mode,get_ini_open_flag(),4*sizeof(character));

				if (!stdfile::open(filename,_m_ini_file_mode))
					return false;

				long len = stdfile::length();
				assert (len%sizeof(character)==0 && "the file should be unicode-2bytes file!");
				len /= sizeof(character);
				len += 4;

				_m_buffer = (character*)new character[len];
				if (!_m_buffer) return false;

				stdfile::rewind();
				size_t readed = stdfile::read(_m_buffer,sizeof(character),len);
				stdfile::close();
				assert (readed+4==len);
				if (readed==0) return false;
				::memset((char*)(_m_buffer+readed),0,len-readed);

				character* buffer = _m_buffer;
				size_t buffer_len = readed;
					
				if (sizeof(character)==2) // if is unicode
				{
					int c = buffer[0];
					if (c==0xfeff) ++buffer, --buffer_len;
				}

				/// split by row
				nstrings strs;
				strkit::spliter split;
				split.set_buffer(buffer,buffer_len);
				split.set_seperators(getv_crlf(),3);
				split.set_sepnew(0);
				split(strs);

				if (strs.empty()) return false;

				/// set next spliter property
				character const seps[2] = {const_key_seperator_char,0};
				split.set_seperators(seps);
				split.unset_sep();
				split._m_count = 1;

				/// get max index
				strkit::nstrings last;
				split.set_buffer(strs.back().begin());
				split(last);
				assert (last.size()>1);
				if (last.size()<2)
					return false;
				size_t max_index = strkit::to_ulong(last[0].begin(),10);

				/// reallocate the cache
				_m_strings.resize(max_index+1);

				/// assign each row string
				split.set_sepnew(0);
				size_t last_index = 0;
				strkit::nstrings keyvalue;
				typedef strkit::nstrings::iterator I;
				for (I i=strs.begin(); i!=strs.end(); ++i)
				{
					keyvalue.clear();
					split.set_buffer(i->begin());
					split(keyvalue);
					if (keyvalue.size()<2)
					{
						//assert(false && "the item can not be seperated");
						continue;
					}
					size_t index = strkit::to_ulong(keyvalue[0].begin(),10);
					assert (last_index==0 || index>last_index);
					if (index<=last_index && last_index!=0 && index>max_index)
						continue;
					/// number skips
					if (index>last_index+1)
					{
						for (size_t j=last_index+1;j<index;++j)
							_m_strings[j] = nstring::make((character*)nullstr(),(character*)nullstr());
					}
					/// assign the item
					if (keyvalue.size()>1)
					{
						_m_strings[index] = keyvalue[1];
						last_index = index;
					}
				}

				return true;
			}
			bool reopen (const character *filename=0) { return stdfile::reopen(filename,get_ini_open_flag()); }
			bool close () { return stdfile::close(); }
			bool isopen() { return stdfile::isopen(); } /// 这个该不该要呢？

		public:
			character const* get_value(size_t index)
			{
				if (index<_m_strings.size())
					return _m_strings[index].begin();
				else
					return nullstr();
			}
			nstrings const& items() const
			{
				return _m_strings;
			}

			bool is_empty() const
			{
				return _m_strings.empty();
			}

		protected:
			character* _m_buffer;
			nstrings _m_strings;
			character _m_ini_file_mode[8];
			character const* nullstr() const
			{
				static character const _m_null[2] = {0,0};
				return _m_null;
			}

		private:
			static const character const_section_begin_char = strkit::left_middle_brace_character;
			static const character const_section_end_char = strkit::right_middle_brace_character;
			static const character const_key_seperator_char = strkit::equal_character;
			static const character const_comment_slash_begin_char = strkit::slash_character;
			static const character const_comment_semicommon_begin_char = strkit::semicolon_character;
			static const character const_comment_well_begin_char = strkit::well_character;
			static const character const_end_of_line_linefeed_char = strkit::linefeed_character;
			static const character const_end_of_line_carriagereturn_char = strkit::carriagereturn_character;

			static character * getp_skip_characters()
			{
				static const character ls_skip_characters [] = 
				{
					strkit::space_character,
					strkit::tab_character,
					strkit::null_character
				};
				return const_cast<character*>(ls_skip_characters);
			}

			static character * getv_crlf()
			{
				static const character ls_crlf [] = 
				{
					const_end_of_line_carriagereturn_char,
					const_end_of_line_linefeed_char,
					strkit::null_character
				};
				return const_cast<character*>(ls_crlf);
			}
		};
	} // end of io
} // end of ox

