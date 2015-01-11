/**
	时间：2008-1-14
	作者：徐海龙
	描述：分析命令行命令的　后续参数并把这些参数放进了一个特定的数据结构中

	copyright:  you can freely copy, reuse and distribute the code, 
	            but should add the head comment in front of your file.
	---------------------------------------------------------------------
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	Hobby    :  Playing Table Tenis, Playing China Chess, Speaking English, etc.
	            Programing, Linux, C++, Network, etc.
	P.S.     :  You can contract me at any time.
*/

#include "../pkg/character_trait.h"
#include "../pkg/string_simple_kit.h"

#pragma once

namespace ox
{
namespace app
{

	template <typename character_tn>
	class command_parameter_tt
	{
	public:
		typedef typename ox::character::character_trait<character_tn>::character character;
		typedef character * key_t;
		typedef character * entry_t;
		typedef std::vector<entry_t> value_t;
		struct command_less : std::less<character*>
		{
			//typedef 
			bool operator()(const character* _Left, const character* _Right) const
			{
				return ox::string::string_simple_kit<character>::strcmp<ox::character::__case_sensitive>(_Left,_Right)<0;
			}
		};
		typedef std::map<key_t, value_t,command_less> parameters_t;
		typedef typename parameters_t::iterator iparameter_t;

		value_t & set_parameter(const key_t & key, const value_t & value)
		{
			return parameters[key] = value;
		}
		void add_parameter(const key_t & key, const entry_t & entry)
		{
			parameters[key].push_back(entry);
		}
		value_t & get_parameter(const key_t & key)
		{
			return parameters[key];
		}
		iparameter_t get_iparameter(const key_t & key)
		{
			return parameters.find(key);
		}
		iparameter_t get_iparameter_end()
		{
			return parameters.end();
		}
		value_t & operator[] (const key_t & key)
		{
			return parameters[key];
		}
		static key_t & refv_key(iparameter_t const & iparameter){ return iparameter->first; }
		static value_t & refv_value(iparameter_t const & iparameter){ return iparameter->second; }
	private:
		parameters_t parameters;
	};


	/**
	分析的command line的形式如下：
	command
	argument1 argumen2
	-id value1 value2 ....
	-idx value1 value2 .....
	注：value中如果最前的字符是“-”符号的话，在前面加转义符“\”
	如果要表达转义符本身写入“\\”
	也就是说，value中的前两个字符:
	"\-" ==> "-"
	"\\" ==> "\"
	*/

	class decompose_command_parameter
	{
	public:
		//decompose_command_parameter(){} /// 空构造
		//template <typename character_tn>
		//decompose_command_parameter(character_tn * argv[], int argc, command_parameter_tt<character_tn> & command_parameter)
		//{
		//	parse(argv,argc,command_parameter);
		//}

		/// 分析
	public:
		template <typename character_tn>
		static character_tn* get_null_key()
		{
			typedef typename ox::character::character_trait<character_tn>::character character;
			static const character key [] =
			{
				ox::character::static_value<character>::space_character,
				ox::character::static_value<character>::null_character
			};
			return const_cast<character_tn*>(key);
		};

		template <typename character_tn>
		static void parse(character_tn * argv[], int argc, command_parameter_tt<character_tn> & command_parameter)
		{
			typedef typename ox::character::character_trait<character_tn>::character character;
			typedef ox::string::string_simple_kit<character> string_kit;
			typedef typename command_parameter_tt<character> parameter_t;
			typedef typename command_parameter_tt<character>::key_t key_t;
			typedef typename command_parameter_tt<character>::value_t value_t;
			typedef typename command_parameter_tt<character>::entry_t entry_t;

			struct parse_value_t
			{
				static character * get_transformed_entry(const character * entry)
				{
					assert(entry);
					if (entry[0]==ox::character::static_value<character>::backslash_character)
					{
						if (entry[1]==ox::character::static_value<character>::minus_character ||
							entry[1]==ox::character::static_value<character>::backslash_character)
							return const_cast<character*>(entry +1);
					}
					return const_cast<character *>(entry);
				}
				static character * get_transformed_key(const character * key)
				{
					assert(key);
					if (*key==ox::character::static_value<character>::minus_character)
						return const_cast<character*>(key+1);
					else
						return const_cast<character*>(key);
				}
			};

			value_t * value = 0;
			int i=0;

			if (argv[i][0]!=ox::character::static_value<character>::minus_character)
			{
				value = &command_parameter.set_parameter(get_null_key<character>(),value_t());
				value->push_back(parse_value_t::get_transformed_entry(argv[i]));
				for (i++; i<argc; i++)
					if (argv[i][0]!=ox::character::static_value<character>::minus_character)
						value->push_back(parse_value_t::get_transformed_entry(argv[i]));
					else
						break;
			}
			for (; i<argc;i++)
				if (argv[i][0]==ox::character::static_value<character>::minus_character)
					value = &command_parameter.set_parameter(parse_value_t::get_transformed_key(argv[i]),value_t());
				else
					value->push_back(parse_value_t::get_transformed_entry(argv[i]));
		}
	};

} // end of app
} // end of ox