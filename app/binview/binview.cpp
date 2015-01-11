// binview.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <map>
#include "../../src/mth/maxmin.h"
#include "../../src/pkg/string_simple_kit.h"
#include "../../src/pkg/string_convert.h"
#include "../../src/io/file.h"
#include "../../src/io/console.h"
#include "../../src/utl/path_kit.h"
#include "../../src/cxx/smartptr.h"
#include "../../src/app/command_argument.h"


enum view_part_type_c
{
	__vp_row_count,
	__vp_column_count,
	__vp_unit_count,
};

template <typename character_tn>
struct view_kit
{
	typedef typename ox::character::character_trait<character_tn>::character character;
	typedef typename ox::character::character_trait<character_tn>::ucharacter ucharacter;
	typedef ox::string::string_simple_kit<character_tn> string_kit;
	typedef ox::io::file<character> stdfile;
	typedef ox::cxx::smartptr<character> smartptr_t;

	struct user_argments_t
	{
		character * path;
		/// size_t 每列的字符个数
		size_t view_column_unit_count;
		/// size_t 每行的字符个数
		size_t view_row_unit_count;
		/// size_t 字符的大小，字节单位
		size_t unit_size;
		/// ox::io::seek_mode_c 查看的起始点
		ox::io::file_helper::seek_mode_c oragin;
		view_part_type_c part_type;
		/// 范围大小描述
		/// size_t 范围的开始
		size_t scope_from;
		/// size_t 范围的结束
		size_t scope_to;
	};

	struct argments_t
	{
		/// stdfile 要操作的对象
		stdfile file;
		/// size_t 每列的字符个数
		size_t view_column_unit_count;
		/// size_t 每行的字符个数
		size_t view_row_unit_count;
		/// size_t 字符的大小，字节单位
		size_t unit_size;
		/// size_t 范围的开始
		size_t scope_from;
		/// size_t 范围的结束
		size_t scope_to;
		size_t getv_scope_count() const
		{
			assert(scope_to>scope_from);
			return scope_to-scope_from;
		}
	};

	/// user_argument：要保证这里的参数基本规则的正确性
	/// 1. scope_from < scope_to
	/// 返回值：是否转换成功
	static bool process_user_arguments(
		user_argments_t const & user_argument,
		argments_t & arguments)
	{
		static character open_flag[] =
		{
			ox::character::static_value<character>::get<'r'>::c_value,
			ox::character::static_value<character>::get<'b'>::c_value,
			ox::character::static_value<character>::null_character
		};
		arguments.unit_size = user_argument.unit_size;
		arguments.view_column_unit_count = user_argument.view_column_unit_count;
		arguments.view_row_unit_count = user_argument.view_row_unit_count;

		if (!arguments.file.open(user_argument.path,open_flag))
			return false;
		switch (user_argument.oragin)
		{
		case ox::io::file_helper::__seek_begin:
			arguments.scope_from = 0;
			arguments.scope_to = user_argument.scope_to-user_argument.scope_from;
			break;
		case ox::io::file_helper::__seek_currrent:
			arguments.scope_from = user_argument.scope_from;
			arguments.scope_to = user_argument.scope_to;
			break;
		case ox::io::file_helper::__seek_end:
			arguments.scope_from = arguments.file.length()-user_argument.scope_to+user_argument.scope_from;
			arguments.scope_to = arguments.scope_from+user_argument.scope_to-user_argument.scope_from;
			break;
		}
		const size_t unit_count = get_units_count(user_argument.part_type,user_argument);
		arguments.scope_from *= unit_count;
		arguments.scope_to *= unit_count;
		return true;
	}
	static size_t get_units_count(const view_part_type_c & view_part, const user_argments_t & user_argument)
	{
		switch(view_part)
		{
		case __vp_column_count: return user_argument.view_column_unit_count;
		case __vp_row_count: return user_argument.view_row_unit_count;
		default: return sizeof(character);
		}
	}
	static void printf_view_column(const character * column, const character * column_end)
	{
		const character * p = column;
		if (p<column_end)
			ox::io::console::out<char>::printf("%0*X",sizeof(character)*2,ucharacter(*p));
		for (p++;p<column_end;p++)
		{
			ox::io::console::out<char>::printf(" %0*X",sizeof(character)*2,ucharacter(*p));
		}
		if (p<column_end)
			ox::io::console::out<char>::printf("%0*X",sizeof(character)*2,ucharacter(*p));
	}

	static void printf_view_line(const character * bin, size_t bin_len, size_t column_len)
	{
		static character column_sep[] =
		{
			ox::character::static_value<character>::space_character,
			ox::character::static_value<character>::minus_character,
			ox::character::static_value<character>::space_character,
			ox::character::static_value<character>::null_character
		};

		const character * p = bin;
		const character * end = bin+bin_len;
		const character * guard = p;
		
		if (p<end)
		{
			guard = ox::mth::min(p+column_len,end);
			printf_view_column(p,guard);
		}
		
		if (guard>=end)
			return;
		for (p=guard;p<end;p=guard)
		{
			ox::io::console::out<character>::printf(column_sep);
			guard = ox::mth::min(p+column_len,end);
			printf_view_column(p,guard);
		}
	}

	static void printf_file(const user_argments_t & user_argument)
	{
		static character next_newline[] =
		{
			ox::character::static_value<character>::get<'\r'>::c_value,
			ox::character::static_value<character>::get<'\n'>::c_value,
			ox::character::static_value<character>::null_character
		};

		argments_t argument;
		if (!process_user_arguments(user_argument,argument))
			return;

		/// 定位完毕，开始读取
		smartptr_t buffer(new character [argument.view_row_unit_count]);
		for (
			size_t units_actual_readed_once=0,
			calculated_len=0,
			units_readed_accumulated=0,
			view_length=argument.getv_scope_count();
			true;)
		{
			calculated_len = units_actual_readed_once = argument.file.read((char*)buffer.getp_raw_pointer(),sizeof(character),argument.view_row_unit_count);
			units_readed_accumulated += units_actual_readed_once;
			if (units_readed_accumulated>view_length)
				calculated_len -= units_readed_accumulated-view_length;
			printf_view_line(buffer.getp_raw_pointer(),calculated_len,argument.view_column_unit_count);
			ox::io::console::out<character>::printf(next_newline);
			if (units_actual_readed_once<argument.view_row_unit_count || /// 读到了文件的结尾
				units_readed_accumulated>=view_length) /// 超出了规定的范围
				break; 
		}
	}
};

template <typename character_tn>
struct preprocess_commandline
{
	typedef typename ox::character::character_trait<character_tn>::character character;
	typedef typename view_kit<character_tn>::user_argments_t user_argments_t;
	typedef ox::character::static_value<character> static_t;

	/// 返回是否参数合法，分析命令行是否成功
	static void fill_with_default(user_argments_t & user_argument)
	{
		user_argument.unit_size = 1;
		user_argument.view_column_unit_count = 8;
		user_argument.view_row_unit_count = 16;
		user_argument.oragin=ox::io::file_helper::__seek_begin;
		user_argument.part_type=__vp_row_count;
		user_argument.scope_from=0;
		user_argument.scope_to=-1;
	}
	static character get_logic_unit_of_row()
	{
		return static_t::get<'r'>::c_value;
	}
	static character get_logic_unit_of_column()
	{
		return static_t::get<'c'>::c_value;
	}
	static character get_logic_unit_of_unit()
	{
		return static_t::get<'u'>::c_value;
	}
	static const character * get_filename_key()
	{
		static const character key[2] =
		{
			static_t::space_character,
			static_t::null_character
		};
		return key;
	}
	static const character * get_unit_size_key()
	{
		static const character key[2] =
		{
			static_t::get<'u'>::c_value,
			static_t::null_character
		};
		return key;
	}	
	static const character * get_mode_key()
	{
		static const character key[2] =
		{
			static_t::get<'m'>::c_value,
			static_t::null_character
		};
		return key;
	}	
	static const character * get_head_key()
	{
		static const character key[2] =
		{
			static_t::get<'h'>::c_value,
			static_t::null_character
		};
		return key;
	}	
	static const character * get_tail_key()
	{
		static const character key[2] =
		{
			static_t::get<'t'>::c_value,
			static_t::null_character
		};
		return key;
	}	
	static const character * get_seek_key()
	{
		static const character key[2] =
		{
			static_t::get<'s'>::c_value,
			static_t::null_character
		};
		return key;
	}

	static view_part_type_c get_view_part(const character & view_part)
	{
		if (view_part==get_logic_unit_of_column())
			return __vp_column_count;
		else if (view_part==get_logic_unit_of_row())
			return __vp_row_count;
		else
			return __vp_unit_count;
	}
	static character * get_view_part(
		const character * view_part_string,
		view_part_type_c & view_part_id)
	{
		assert(view_part_string);
		if (!view_part_string) return 0;
		assert(*view_part_string);
		if (*view_part_string==0) return const_cast<character*>(view_part_string);
		if (*view_part_string>=static_t::get<'0'>::c_value&&
			*view_part_string<=static_t::get<'9'>::c_value)
			return const_cast<character*>(view_part_string);
		else
			return const_cast<character*>(view_part_string+1);
	}

	/// 把命令行转化成用户参数的输入结构
	/// 处理流程如下：
	/// 1. 把参数按照逻辑结构进行加载
	/// 2. 把逻辑结构数据转化成实际用的类型，存入到用户参数类型结构中
	static bool _do(character * argv[], int argc, user_argments_t & user_argument)
	{
		typedef ox::string::string_simple_kit<character> string_kit;
		typedef ox::app::command_parameter_tt<character> command_parameter_t;
		typedef command_parameter_t::iparameter_t iparameter_t;
		typedef ox::app::decompose_command_parameter decompose_command_parameter;

		/// 初始化用户显示默认值
		fill_with_default(user_argument);

		/// 把参数加载到特定数据结构，并进行分析
		command_parameter_t command_parameter;
		decompose_command_parameter::parse(argv,argc,command_parameter);

		/// 把特定数据结构进行转化成用户参数，并进行基本参数校验

		iparameter_t iparameter = command_parameter.get_iparameter_end();
		/// filename
		iparameter = command_parameter.get_iparameter(const_cast<character *>(get_filename_key()));
		if (iparameter!=command_parameter.get_iparameter_end())
		{
			if (command_parameter_t::refv_value(iparameter).size()>=2)
				user_argument.path = command_parameter_t::refv_value(iparameter)[1];
		}
		/// 解释如下 -h/t/s c/r/uxxx (c:column_count, r:row_count, u:unit_count);显示前面的多少
		/// -h/t/s c/r/uxxx (u:unit_count);显示前面的多少
		character part_type_c = 0;
		iparameter = command_parameter.get_iparameter(const_cast<character *>(get_head_key()));
		if (iparameter!=command_parameter.get_iparameter_end())
		{
			user_argument.oragin=ox::io::file_helper::__seek_begin;
			if (command_parameter_t::refv_value(iparameter).size()>=1)
			{
				user_argument.scope_from=0;
				user_argument.scope_to=string_kit::to_ulong(
					get_view_part(
						command_parameter_t::refv_value(iparameter)[0],
						user_argument.part_type),10);
			}
		}
		iparameter = command_parameter.get_iparameter(const_cast<character *>(get_tail_key()));
		if (iparameter!=command_parameter.get_iparameter_end())
		{
			user_argument.oragin=ox::io::file_helper::__seek_end;
			user_argument.scope_from=0;
			if (command_parameter_t::refv_value(iparameter).size()>=1)
				user_argument.scope_to=string_kit::to_ulong(
					get_view_part(
						command_parameter_t::refv_value(iparameter)[0],
						user_argument.part_type),10);
		}
		iparameter = command_parameter.get_iparameter(const_cast<character *>(get_seek_key()));
		if (iparameter!=command_parameter.get_iparameter_end())
		{
			user_argument.oragin=ox::io::file_helper::__seek_currrent;
			if (command_parameter_t::refv_value(iparameter).size()>=1)
				user_argument.scope_from=string_kit::to_ulong(
					get_view_part(
						command_parameter_t::refv_value(iparameter)[0],
						user_argument.part_type),10);
			if (command_parameter_t::refv_value(iparameter).size()>=2)
				user_argument.scope_to=string_kit::to_ulong(
					get_view_part(
						command_parameter_t::refv_value(iparameter)[1],
						user_argument.part_type),10);
		}
		
		/// -m column_char_count row_char_count
		iparameter = command_parameter.get_iparameter(const_cast<character *>(get_mode_key()));
		if (iparameter!=command_parameter.get_iparameter_end())
		{
			user_argument.view_column_unit_count = string_kit::to_ulong(
				command_parameter_t::refv_value(iparameter)[0],10);
			user_argument.view_row_unit_count = string_kit::to_ulong(
				command_parameter_t::refv_value(iparameter)[1],10);
		}

		/// -u unit_size
		iparameter = command_parameter.get_iparameter(const_cast<character *>(get_unit_size_key()));
		if (iparameter!=command_parameter.get_iparameter_end())
		{
			user_argument.unit_size = string_kit::to_ulong(
				command_parameter_t::refv_value(iparameter)[0],10);
		}
		return true;
	}
};


/**
	使用方法：command filename
		-u unit_size
		-m column_char_count row_char_count
		-h/t/s c/r/uxxx (c:column_count, r:row_count, u:unit_count);显示前面的多少
		(h: 从头开始 head， t：从尾开始 tail， s：中间开始 scope/seek)
*/
//
//unsigned long f(unsigned long n)
//{
//	if (n<=2) return 1;
//	unsigned long sum = 0;
//	for (unsigned long i=1; i<n; i++)
//		sum += f(i)*f(n-i);
//	return sum;
//}
int _tmain(int argc, _TCHAR* argv[])
{
	typedef wchar_t character;
	typedef view_kit<character> view_t;
	typedef ox::utl::localpath_kit<character> path_t;
	typedef view_t::user_argments_t user_argments_t;

	character refer_filepath[128] = {0};
	character filename[]= 
	{
		ox::character::static_value<character>::get<'t'>::c_value,
		ox::character::static_value<character>::get<'e'>::c_value,
		ox::character::static_value<character>::get<'s'>::c_value,
		ox::character::static_value<character>::get<'t'>::c_value,
		ox::character::static_value<character>::get<'.'>::c_value,
		ox::character::static_value<character>::get<'t'>::c_value,
		ox::character::static_value<character>::get<'x'>::c_value,
		ox::character::static_value<character>::get<'t'>::c_value,
		ox::character::static_value<character>::get<' '>::c_value,
		ox::character::static_value<character>::null_character,
	};
	ox::string::conv::to_string(__FILEW__,refer_filepath);
	path_t path(refer_filepath);
	std::wstring filepath = path_t::make(path.getv_ddir().c_str(),filename).c_str();

	const character * argvv []=
	{
		__FILEW__,
		filepath.c_str(),
		L"-m",
		L"5",
		L"10",
		L"-h",
		L"12"
	};

	user_argments_t user_argument;
	preprocess_commandline<character>::_do(
		(character**)(argvv),
		sizeof(argvv)/sizeof(wchar_t*),
		user_argument);
	
	view_t::printf_file(user_argument);

	ox::io::console::in<char>::getch();

	return 0;
}

