
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
#include "../pkg/string_simple_kit.h"
#include "../utl/path_kit.h"
#include "./file.h"
#include "./stream_kit.h"
#include "./application_set.h"
#include "./shell_kit.h"

#pragma warning (disable: 4996)

namespace ox
{
	namespace io
	{
		namespace ___
		{
			template <typename character_tn>

			struct ini_file_struct_base
			{
				typedef ox::string::string_simple_kit<character_tn> strkit;
				typedef typename strkit::character character;
				typedef typename strkit::std_string std_string;

				typedef std_string section_id_t;
				typedef std_string key_t;
				typedef std_string value_t;
				typedef std::pair<key_t,value_t> item_raw_t;
				typedef std::list<item_raw_t> item_set_raw_t;
				typedef typename item_set_raw_t::iterator item_i_raw_t;

				typedef ox_application_simple_set<section_id_t,key_t,item_i_raw_t> ox_ini_application_set;
				typedef typename ox_ini_application_set::basic_units_map section_t;
				typedef typename ox_ini_application_set::basic_application_set section_set_t;

				typedef struct ox_ini_section_raw_t
				{
					section_id_t section_id;
					item_set_raw_t item_set;
				}	section_raw_t;

				typedef std::list<section_raw_t> section_set_raw_t;
			};

			template <typename character_tn>

			class ini_file_sort_struct : 
				virtual public ini_file_struct_base<character_tn>, 
				public ini_file_struct_base<character_tn>::ox_ini_application_set
			{
			public:
				item_i_raw_t & refv_ini_item_i_value(section_id_t const & section_id, key_t const & key_id)
				{
					return refv_value_ele(section_id,key_id);
				}
				bool refv_ini_item_i_value(section_id_t const & section_id, key_t const & key_id, typename section_t::iterator & i)
				{
					bool result = false;
					section_set_t::iterator m = refv_ini_section_set().find(section_id);
					if (m!=refv_ini_section_set().end())
					{
						section_t::iterator n = m->second.find(key_id);
						if (n!=m->second.end())
							i = n,result = true;
					}
					return result;
				}
				item_i_raw_t & refv_ini_item_i_value(section_t & section, key_t const & key_id)
				{
					return refv_value_ele(section,key_id);
				}
				bool refv_ini_item_i_value(section_t & section, key_t const & key_id, typename section_t::iterator & i)
				{
					bool result = false;
					section_t::iterator n = section.find(key_id);
					if (n!=section.end())
						i = n,result = true;
					return result;
				}
				section_t & refv_ini_section(section_id_t const & section_id)
				{
					return refv_basic_units_map(section_id);
				}
				section_set_t & refv_ini_section_set()
				{
					return refv_basic_application_set();
				}

				void add_one_section(section_id_t const & section_id)
				{
					section_t empty;
					refv_ini_section_set()[section_id] = empty;
				}

				void add_one_item(section_id_t const & section_id, key_t const & key, item_i_raw_t const & item_i)
				{
					refv_ini_item_i_value(section_id,key) = item_i;
				}

				size_t erase_one_item(section_id_t const & section_id, key_t const & key)
				{
					return refv_ini_section(section_id).erase(key);
				}

				size_t erase_one_section(section_id_t const & section_id)
				{
					return refv_ini_section_set().erase(section_id);
				}
			};

			template <typename character_tn>

			struct ini_file_raw_struct : 
				virtual public ini_file_struct_base<character_tn>,
				public std::list<typename ini_file_struct_base<character_tn>::section_raw_t>
			{
				section_set_raw_t & refv_raw_section_set() { return *this; }
				section_set_raw_t const& refv_raw_section_set() const { return *this; }

				bool is_empty() const
				{
					return refv_raw_section_set().empty();
				}

				typename section_set_raw_t::iterator refv_raw_section(section_id_t const & section_id)
				{
					for (section_set_raw_t::iterator i=refv_raw_section_set().begin(); i!=refv_raw_section_set().end(); i++)
						if (i->section_id==section_id)
							return i;
					return refv_raw_section_set().end();
				};
				typename section_set_raw_t::iterator refv_raw_section_endi()
				{
					return refv_raw_section_set().end();
				}

				typename item_set_raw_t::iterator refv_raw_item(item_set_raw_t & item_set, key_t const & key)
				{
					for (item_set_raw_t::iterator i=item_set.begin(); i!=item_set.end(); i++)
						if (i->first==key)
							return i;
					return item_set.end();
				}

				typename section_set_raw_t::iterator add_one_section(section_id_t const & section_id)
				{
					section_set_raw_t::iterator i = refv_raw_section(section_id);
					if (i==refv_raw_section_set().end())
					{
						section_raw_t section;
						section.section_id = section_id;
						refv_raw_section_set().push_back(section);
						i = --refv_raw_section_set().end();
					}
					return i;
				}

				typename item_set_raw_t::iterator add_one_item(section_id_t const & section_id, key_t const & key, value_t const & value)
				{
					section_set_raw_t::iterator i = add_one_section(section_id);
					item_set_raw_t::iterator j = refv_raw_item(i->item_set,key);
					if (j==i->item_set.end())
					{
						i->item_set.push_back(item_raw_t(key,value));
						j= --i->item_set.end();
					}
					else
					{
						j->second = value;
					}
					return j;
				}

				size_t erase_one_item(section_id_t const & section_id, key_t const & key)
				{
					typename section_set_raw_t::iterator i = refv_raw_section(section_id);
					if (i!=refv_raw_section_set().end())
					{
						item_set_raw_t::iterator j = refv_raw_item(i->item_set,key);
						if (j!=i->item_set.end())
							return i->item_set.erase(j),1;
					}
					return 0;
				}
				size_t erase_one_section(section_id_t const & section_id)
				{
					typename section_set_raw_t::iterator i = refv_raw_section(section_id);
					if (i!=refv_raw_section_set().end())
						return refv_raw_section_set().erase(i),1;
					return 0;
				}
			};

			template <typename character_tn>

			class struct_file_struct : 
				virtual public ini_file_struct_base<character_tn>,
				protected ini_file_sort_struct<character_tn>,
				protected ini_file_raw_struct<character_tn>
			{
			public:
				typedef ox::def::procedure_control_c (*fn_process_one_section_line)(section_id_t const & section_id);
				typedef ox::def::procedure_control_c (*fn_process_one_item_line)(section_id_t const & section_id, key_t const & key, value_t const & value);

				fn_process_one_section_line & on_process_one_section_line() { return m_process_one_section_line_d; }
				fn_process_one_item_line & on_process_one_item_line() { return m_process_one_item_line_d; }

				ini_file_raw_struct & refv_ini_raw_struct() { return * this; }
				ini_file_sort_struct & refv_ini_sort_struct() { return * this; }
				ini_file_raw_struct const& refv_ini_raw_struct() const { return *this; }

				bool is_empty() const
				{
					return refv_ini_raw_struct().is_empty();
					return true;
				}

				void add_one_section(section_id_t const & section_id)
				{
					refv_ini_raw_struct().add_one_section(section_id);
					refv_ini_sort_struct().add_one_section(section_id);
				}

				void add_one_item(section_id_t const & section_id, key_t const & key, value_t const & value)
				{
					refv_ini_sort_struct().add_one_item(
						section_id,
						key,
						refv_ini_raw_struct().add_one_item(section_id,key,value));
				}

				void add_one_section(section_id_t const & section_id, item_set_raw_t const & items)
				{
					for (item_set_raw_t::const_iterator i=items.begin(); i!=items.end(); i++)
						add_one_item(section_id,i->first,i->second);
				}

				size_t erase_one_section(section_id_t const & section_id)
				{
					refv_ini_sort_struct().erase_one_section(section_id);
					return refv_ini_raw_struct().erase_one_section(section_id);
				}

				size_t erase_one_item(section_id_t const & section_id, key_t const & key)
				{
					refv_ini_sort_struct().erase_one_item(section_id,key);
					return refv_ini_raw_struct().erase_one_item(section_id,key);
				}

				void walkthrough()
				{
					for (ini_file_raw_struct::section_set_raw_t::iterator i=refv_ini_raw_struct().refv_raw_section_set().begin();
						i!=refv_ini_raw_struct().refv_raw_section_set().end();
						i++)
					{
						if (m_process_one_section_line_d)
							if (ox::def::__procedure_return==m_process_one_section_line_d(i->section_id))
								break;
						for (ini_file_raw_struct::item_set_raw_t::iterator j=i->item_set.begin(); j!=i->item_set.end(); j++)
							if (m_process_one_item_line_d)
								if (ox::def::__procedure_return==m_process_one_item_line_d(i->section_id,j->first,j->second))
									return ;
					}
				}
			private:
				fn_process_one_item_line m_process_one_item_line_d;
				fn_process_one_section_line m_process_one_section_line_d;
			};
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
		class inifile :
			public ___::ini_file_struct_base<character_tn>,
			protected ox::io::file<character_tn>
		{
			typedef ox::character::character_trait<character_tn> character_trait;
		public:
			typedef typename character_trait::character character;
			typedef typename strkit::std_string std_string;
		private:
			typedef ox::stream::stdstream_kit<character> stdstream_kit;
			typedef typename stdstream_kit::get_line_default_t get_line_t;
			typedef ox::shell::shell_kit<character_tn> shell;
			typedef ox::utl::localpath_kit<character_tn> pathkit;


			typedef ___::ini_file_struct_base<character> ini_file_struct_base;
			typedef ___::ini_file_struct<character> ini_file_struct;
			typedef ox::io::file<character> stdfile;
			typedef ___::ini_file_sort_struct<character> ox_ini_file_sort_struct;
			typedef typename ox_ini_file_sort_struct::section_id_t section_id_t;


			/// 在遍历整个数据时，按照事件的处理方式处理所有的数据
			typedef ox::def::procedure_control_c (*fn_traverse_line_d) (
				void const * _in_extra,
				character const * _in_dir, /// 当前节点的所在的区域
				character const * _in_key, /// 当前所指向的所在区域下的关键值
				character const * _in_value /// 当前所指向的所在区域下的关键值的所指向的数据
				);

			enum line_class_t
			{
				__line_class_section,
				__line_class_key_value,
				__line_class_comment,
				__line_class_invalid
			};

			struct get_ini_line_t : public get_line_t
			{
				typedef typename stdstream_kit::allocator allocator;
				struct allocate_t
				{
					allocate_t(allocator & _inref_allocator)
						: _m_ref_allocator(_inref_allocator)
					{}
					character * operator ()(int _in_units_count, bool _in_is_reserved = true)
					{
						return _m_ref_allocator.allocate(_in_units_count,_in_is_reserved);
					}
					allocator & _m_ref_allocator;
				};

				allocate_t get_allocate()
				{
					return allocate_t(line_allocator);
				}
			};

			const character * get_ini_open_flag()
			{
				static const character ls_ini_open_flag [] =
				{
					strkit::to_static_character<char,'a'>::character_value,
					strkit::to_static_character<char,'+'>::character_value,
					strkit::to_static_character<char,'b'>::character_value,
					strkit::null_character
				};
				return ls_ini_open_flag;
			}
		public:
			typedef typename get_ini_line_t::allocate_t allocate_t;
			/// 用户定义及相应事件
			/// 在数据向介质提交前，你可以改变所提交的内容
			typedef void (*fn_prev_set_line_d)(std_string & ref_raw_current_line_to_be_written);
			/// 在数据读取后，你可以改变所读取的内容
			typedef void (*fn_post_get_line_d)(allocate_t &, character * & ref_current_line, size_t & ref_current_line_len);
			inifile(
				fn_post_get_line_d post_get_line_d = 0,
				fn_prev_set_line_d prev_set_line_d = 0)
				: _m_extra(0)
			{
				on_prev_setline()=prev_set_line_d;
				on_post_getline()=post_get_line_d;
				memmove(_m_ini_file_mode,get_ini_open_flag(),4*sizeof(character));
			}
			inifile(const character * filename, bool const & is_load = true)
				: _m_extra(0)
			{
				open (0,0,filename,is_load);
			}
			inifile(
				fn_post_get_line_d post_get_line_d,
				fn_prev_set_line_d prev_set_line_d,
				const character * filename, bool const & is_load = true)
				: _m_extra(0)
			{
				open (prev_set_line_d,post_get_line_d,filename,is_load);
			}

			bool open (
				fn_post_get_line_d post_get_line_d,
				fn_prev_set_line_d prev_set_line_d,
				const character *filename,
				bool const & is_load = true)
			{
				on_prev_setline()=prev_set_line_d;
				on_post_getline()=post_get_line_d;
				memmove(_m_ini_file_mode,get_ini_open_flag(),4*sizeof(character));
				return open(filename,is_load);
			}
			
			bool open (const character *filename, bool const & is_load = true)
			{
				if (!shell::has_file(filename))
				{
					std_string path = filename;
					pathkit::remove_last(path);
					shell::rmkdir(path.c_str(),L'\\');
				}
				if (stdfile::open(filename,_m_ini_file_mode))
				{
					rewind();
					_m_file_kit.attach(this);
					_m_get_oneline.attach(_m_file_kit);

					if (sizeof(character)==2) // if is unicode
					{
						int c = stdfile::getc();
						if (c!=0xfeff) rewind();
					}

					if (is_load) load();
					return true;
				}
				return false;
			}
			bool reopen (const character *filename=0) { return stdfile::reopen(filename,get_ini_open_flag()); }
			bool close () { return stdfile::close(); }
			bool isopen() { return stdfile::isopen(); } /// 这个该不该要呢？
			std_string const& filepath() const { return stdfile::filepath(); }

			/// 获取一行后，添加用户处理的行为
			fn_post_get_line_d & on_post_getline() { return _m_get_post_get_line_event; }
			fn_prev_set_line_d & on_prev_setline() { return _m_get_prev_set_line_event; }
			fn_traverse_line_d & on_traverse_line() { return _m_get_when_traverse_line_event; }

		protected:
			character * get_line (size_t * line_size=0) { return _m_get_oneline(line_size); }
			void free_liner () { _m_get_oneline.free(); }

			// 如果成功返回第一部分的前缀，如果有第二部分，返回第二部分的前缀
			// 否则，失败，返回 null
			character * analyse_oneline(character const * sz_line_buffer, line_class_t * line_class, character ** latter)
			{
				character * p = strkit::find_first_in<false>(sz_line_buffer,getp_skip_characters());
				bool is_analysing_ok = false;

				if (line_class) *line_class = __line_class_invalid;

				if (p)
				{
					character * ptmp = 0;
					switch(*p)
					{
					case const_section_begin_char:
						ptmp = strkit::strch<true>(p,const_section_end_char);
						if (ptmp)
						{
							is_analysing_ok = true;
							if (latter) *latter = ptmp;
							if (line_class) *line_class = __line_class_section;
						}
						break;
					case const_comment_semicommon_begin_char: // ";"
					case const_comment_well_begin_char: // "#"
					case const_end_of_line_linefeed_char: // ""
					case const_end_of_line_carriagereturn_char: // ""
						if (line_class) *line_class = __line_class_comment;
						is_analysing_ok = true;
						break;
					case const_comment_slash_begin_char: // "//"
						if (*(p+1)==const_comment_slash_begin_char)
						{
							if (line_class) *line_class = __line_class_comment;
							is_analysing_ok = true;
						}
						break;
					default: // keyvalue
						ptmp = strkit::strch<true>(p,const_key_seperator_char);
						if (ptmp && ptmp!=p)
						{
							if (line_class) *line_class = __line_class_key_value;
							is_analysing_ok = true;
							if (latter) *latter = ptmp;
						}
					}
				}
				return p;
			}

		public:

			/**
			* @brief read_value 函数说明
			*
			* 写入一个数据到ini文件

			* @param section : char * 指出属于那个区的
			* @param item: char * 那个条目
			* @param defaultvalue: char * 如果这个数据不存在，就返回这个缺省值
			* @return char *: 读出的数据
			* @retval true : 表示写入成功
			* @retval false: 表示写入失败
			*/

			value_t & get_value(character const * section, character const * key, std_string & default_value = std_string())
			{
				section_t::iterator i;
				if (_m_file_struct.refv_ini_sort_struct().refv_ini_item_i_value(section,key,i))
					return i->second->second;
				return default_value;
			}
			
			section_t & get_value(character const * section)
			{
				return _m_file_struct.refv_ini_sort_struct().refv_ini_section(section);
			}

			bool get_raw_value(character const * section_id, section_raw_t & section)
			{
				section_set_raw_t::iterator i =  _m_file_struct.refv_ini_raw_struct().refv_raw_section(section_id);
				if (i!= _m_file_struct.refv_ini_raw_struct().refv_raw_section_endi())
					return section = *i, true;
				return false;
			}

			section_raw_t const& ref_raw_value(character const * section_id)
			{
				section_set_raw_t::iterator i =  _m_file_struct.refv_ini_raw_struct().refv_raw_section(section_id);
				if (i!= _m_file_struct.refv_ini_raw_struct().refv_raw_section_endi())
					return *i;
				return static_raw_null;
			}

			section_set_t & get_value()
			{
				return _m_file_struct.refv_ini_sort_struct();
			}

			section_set_raw_t & get_raw_value()
			{
				return _m_file_struct.refv_ini_raw_struct();
			}

			/**
			* @brief set_value 函数说明
			*
			* 写入一个数据到ini文件

			* @param section : char * 指出属于那个区的
			* @param item: char * 那个条目
			* @param value: char * 要写入的数据
			* @param defaultvalue: char * 如果这个数据不存在，就返回这个缺省值
			* @return : 表示是否写入成功
			* @retval true : 表示写入成功
			* @retval false: 表示写入失败

			* @note 文件不用打开，也不关闭

			* @par 示例:
			* @code
			// 来写入数据项的操作，例子如下：
			inifile ini("d:/myini.ini");
			bool r = ini.write_itemvalue("section_name","item_name", "first","default_value");
			* @endcode

			* @see ::read_itemvalue
			* @deprecated 由于特殊的原因，这个函数可能会在将来的版本中发生改变
			*/
			void set_value(character const * section, character const * key, character const * key_vlaue)
			{
				_m_file_struct.add_one_item(section,key,key_vlaue);
			}
			void set_value(character const * section, item_set_raw_t const & items)
			{
				_m_file_struct.add_one_section(section,items);
			}

			size_t erase_value(character const * section, character const * key)
			{
				return _m_file_struct.erase_one_item(section,key);
			}
			size_t erase_value(character const * section)
			{
				return _m_file_struct.erase_one_section(section);
			}

			bool is_empty() const
			{
				return _m_file_struct.is_empty();
			}
			
			void load()
			{
				character * line = 0;
				character * line_end = 0;
				size_t line_len = 0;

				line_class_t line_class;
				character * p_formmer = 0;
				character * p_latter = 0;
				character comment_key_id[32] = {0};
				size_t comment_next_number = 0;

				std_string current_section;

				while (line = get_line(&line_len))
				{
					line_end = strkit::find_last_in<false>(line,line+line_len-1,getv_crlf()); // 最后一个是0
					* ++ line_end  = 0;
					line_len = line_end - line;

					/// do with the load event
					if (on_post_getline())
						on_post_getline()(_m_get_oneline.get_allocate(),line,line_len);

					p_formmer = analyse_oneline(line, &line_class, &p_latter);
					if (p_formmer)
					{
						switch (line_class)
						{
						case __line_class_section :
							*p_latter = 0;
							_m_file_struct.add_one_section(++p_formmer);
							current_section.assign(p_formmer,p_latter);
							break;
						case __line_class_key_value :
							*p_latter++ = 0;
							_m_file_struct.add_one_item(current_section,p_formmer,p_latter);
							break;
						case __line_class_comment :
							strkit::sprintf(comment_key_id,getp_comment_format(),++comment_next_number);
							_m_file_struct.add_one_item(current_section,comment_key_id,line);
							break;
						}
					}
				}
				free_liner();
			}

			/// 提交数据到物理介质
			void commit()
			{
				refv_processor() = this;
				_m_file_struct.on_process_one_section_line() = process_one_section_line_event;
				_m_file_struct.on_process_one_item_line() = process_one_item_line_event;
				this->truncate();
				_m_file_struct.walkthrough();
				this->flushall();
			}

			/// 遍历所有的结构及数据
			/// 这个参数_in_extra，作为外部数据用的，如果用delegate可以省略掉的
			void traverse(void const * _in_extra)
			{
				refv_processor() = this;
				refv_extra() = _in_extra;
				_m_file_struct.on_process_one_section_line() = traverse_one_section_line_event;
				_m_file_struct.on_process_one_item_line() = traverse_one_item_line_event;
				_m_file_struct.walkthrough();
			}

			ini_file_struct & refv_file_struct() { return _m_file_struct; }
			section_raw_t const& section_raw_null() const { return static_raw_null; }

		protected:
			ini_file_struct _m_file_struct;
			character _m_ini_file_mode[8];
			section_raw_t const static_raw_null;

		private:
			void const * & refv_extra() { return _m_extra; }
			stdstream_kit _m_file_kit;
			get_ini_line_t _m_get_oneline;
			fn_prev_set_line_d _m_get_prev_set_line_event;
			fn_post_get_line_d  _m_get_post_get_line_event;
			fn_traverse_line_d  _m_get_when_traverse_line_event;
			void const * _m_extra;
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

			static character * getp_comment_format()
			{
				static const character ls_comment_format [] = 
				{
					const_end_of_line_linefeed_char,
					strkit::percent_character,
					strkit::to_static_character<char,'u'>::character_value,
					strkit::null_character
				};
				return const_cast<character*>(ls_comment_format);
			}
		private:
			/// 遍历整个数据结构
			static ox::def::procedure_control_c traverse_one_section_line_event(
				typename ox_ini_file_sort_struct::section_id_t const & section_id)
			{
				if (refv_processor() && refv_processor()->on_traverse_line())
					refv_processor()->on_traverse_line()(
						refv_processor()->refv_extra(),
						section_id.c_str(),0,0);
				return ox::def::__procedure_continue;
			}
			static ox::def::procedure_control_c traverse_one_item_line_event(
				typename ox_ini_file_sort_struct::section_id_t const & section_id,
				typename ox_ini_file_sort_struct::key_t const & item_key,
				typename ox_ini_file_sort_struct::value_t const & item_vlaue)
			{
				if (refv_processor() && refv_processor()->on_traverse_line())
					refv_processor()->on_traverse_line()(
						refv_processor()->refv_extra(),
						section_id.c_str(),item_key.c_str(),item_vlaue.c_str());
				return ox::def::__procedure_continue;
			}

			/// 提交整个数据结构
			static ox::def::procedure_control_c process_one_section_line_event(
				typename ox_ini_file_sort_struct::section_id_t const & section_id)
			{
				if (refv_processor()==0) return ox::def::__procedure_continue;

				if (refv_processor()->on_prev_setline())
				{
					/// 处理存储前的处理事件
					std_string ref_line =
						refv_processor()->const_section_begin_char +
						section_id +
						refv_processor()->const_section_end_char ;
					refv_processor()->on_prev_setline()(ref_line);
					refv_processor()->puts(ref_line.c_str());
				}
				else
				{
					// 没有事件触发的时候写入动作
					refv_processor()->putc(refv_processor()->const_section_begin_char);
					refv_processor()->puts(section_id.c_str());
					refv_processor()->putc(refv_processor()->const_section_end_char);
				}
				refv_processor()->putc(refv_processor()->const_end_of_line_carriagereturn_char);
				refv_processor()->putc(refv_processor()->const_end_of_line_linefeed_char);

				return ox::def::__procedure_continue;
			}
			static ox::def::procedure_control_c process_one_item_line_event(
				typename ox_ini_file_sort_struct::section_id_t const & section_id,
				typename ox_ini_file_sort_struct::key_t const & item_key,
				typename ox_ini_file_sort_struct::value_t const & item_vlaue)
			{
				if (refv_processor()==0) return ox::def::__procedure_continue;

				if (refv_processor()->on_prev_setline())
				{
					/// 处理存储前的处理事件
					std_string current_line;

					if (item_key[0]==refv_processor()->const_end_of_line_linefeed_char) // comment
						current_line += item_vlaue.c_str();
					else
						current_line += item_key + refv_processor()->const_key_seperator_char + item_vlaue;
					refv_processor()->on_prev_setline()(current_line);
					refv_processor()->puts(current_line.c_str());
				}
				else
				{
					// 没有事件触发的时候写入动作
					if (item_key[0]==refv_processor()->const_end_of_line_linefeed_char) // comment
						refv_processor()->puts(item_vlaue.c_str());
					else
						refv_processor()->puts(item_key.c_str()),
						refv_processor()->putc(refv_processor()->const_key_seperator_char),
						refv_processor()->puts(item_vlaue.c_str());
				}

				refv_processor()->putc(refv_processor()->const_end_of_line_carriagereturn_char);
				refv_processor()->putc(refv_processor()->const_end_of_line_linefeed_char);

				return ox::def::__procedure_continue;
			}
			static inifile * & refv_processor() { static inifile * lsp_inifile = 0; return lsp_inifile; }
		};
	} // end of io
} // end of ox

