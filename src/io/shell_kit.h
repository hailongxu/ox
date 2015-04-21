/*	
	类名: shell_kit
	作者: 徐海龙
	日期: 2005-6-20
	文件名: directory.h
	版权：版权归作者所有，本文件可以自由传播
 */

#include <io.h>
#include <direct.h>
#include <sys/stat.h>

#include "../pkg/character_static_value.h"
#include "./file.h"
//#include "../utl/debug.h"
#include "../pkg/string_chartype_def.h"
#include "../pkg/string_chartype_pod.h"
#include "../pkg/string_chartype_stl.h"
#include "../pkg/string_simple_kit.h"
#include "../utl/path_kit.h"


#pragma once


namespace ox
{
namespace shell
{

struct shell_kit_type
{
	enum cp_flag
	{
		__cp_overwrite = 1,
		__cp_write_under_nofile = 2
	};

	enum shell_return
	{
		__ret_success = 0,
		__ret_parameter_invalid,
		__ret_source_file_not_exist,
		__ret_destion_file_cannot_new,
		__ret_destion_file_has_exist,
		__ret_file_write_error,
		__ret_cannot_create_dir,
	};

	typedef struct _stat file_attr_t;
};

template <typename character_tn> struct shell_kit_trait;

template <> struct shell_kit_trait<char> : virtual public shell_kit_type
{
	typedef ox::character::character_trait<char>::character character;
	typedef ox::character::character_trait<char>::ucharacter ucharacter;
	static int access(const character * file_name, int access_mode) { return _access(file_name,access_mode); }
	static int mkdir(const character * path) { return _mkdir(path); }
	static int unlink(const character * filename) { return _unlink(filename); }
	static int rmdir(const character * filename) { return _rmdir(filename); }
	static int stat(const character* path, file_attr_t *buffer ) { return _stat(path,buffer); }
	static character* getcwd(character* path,int size) { return _getcwd(path,size); }
};
template <> struct shell_kit_trait<wchar_t> : virtual public shell_kit_type
{
	typedef ox::character::character_trait<wchar_t>::character character;
	typedef ox::character::character_trait<wchar_t>::ucharacter ucharacter;
	static int access(const character * file_name, int access_mode) { return _waccess(file_name,access_mode); }
	static int mkdir(const character * path) { return _wmkdir(path); }
	static int unlink(const character * filename) { return _wunlink(filename); }
	static int rmdir(const character * filename) { return _wrmdir(filename); }
	static int stat(const character *path, file_attr_t *buffer ) { return _wstat(path,buffer); }
	static character* getcwd(character* path,int size) { return _wgetcwd(path,size); }
};

template <typename character_tn>
class shell_kit : protected shell_kit_trait<character_tn>
{
public:
	typedef typename shell_kit_trait<character_tn>::file_attr_t file_attr_t;
	typedef typename shell_kit_trait<character_tn>::character character;
	typedef typename shell_kit_trait<character_tn>::ucharacter ucharacter;
	typedef ox::str::string_simple_kit<character_tn> strkit;
	typedef typename strkit::std_string std_string;
	typedef ox::utl::localpath_kit<character_tn> pathkit;

public:


	//template <typename string_tn>
	//static bool has(string_tn filename)
	//{
	//	character const* id = ox::str::c_string<string_tn>::get(filename);
	//	file_attr_t att;
	//	return 0==attr(id,att);
	//}

	//template <typename string_tn>
	//static bool has_dir(string_tn filename)
	//{
	//	character const* id = ox::str::c_string<string_tn>::get(filename);
	//	file_attr_t att;
	//	if (0!=attr(id,att)) return false;
	//	return att.st_mode&_S_IFDIR ? true:false;
	//}


	static bool has(character const* begin,character const* end,file_attr_t*attri=0)
	{
		character const* id = begin;
		std_string str;
		do {
			if (pathkit::is_drive_root(begin,end))
			{
				if (pathkit::get_last_slash(begin,end))
					break;
				str.assign(begin,end-begin);
				pathkit::add_slash(str,L'\\');
				id = str.c_str();
				break;
			}
			if (0!=pathkit::get_last_slash(begin,end))
			{
				str.assign(begin,end-begin-1);
				id = str.c_str();
			}
		} while(0);
		file_attr_t att;
		if (0!=attr(id,att)) return false;
		if (attri) *attri = att;
		return true;
	}

	static bool has_dir(character const* path)
	{
		file_attr_t attr;
		bool b = has(path,path+strkit::length(path),&attr);
		if (!b) return false;
		return attr.st_mode&_S_IFDIR ? true:false;
	}
	static bool has_dir(std_string const& path)
	{
		file_attr_t attr;
		bool b = has(path.c_str(),path.c_str()+path.length(),&attr);
		if (!b) return false;
		return attr.st_mode&_S_IFDIR ? true:false;
	}

	static bool has_file(character const* path)
	{
		file_attr_t attr;
		bool b = has(path,path+strkit::length(path),&attr);
		if (!b) return false;
		return attr.st_mode&_S_IFMT ? true:false;
	}
	static bool has_file(std_string const& path)
	{
		file_attr_t attr;
		bool b = has(path.c_str(),path.c_str()+path.length(),&attr);
		if (!b) return false;
		return attr.st_mode&_S_IFREG ? true:false;
	}
	static unsigned short get_type(std_string const& path)
	{
		file_attr_t attr;
		bool b = has(path.c_str(),path.c_str()+path.length(),&attr);
		if (!b) return 0;
		return attr.st_mode;
	}
	static bool is_dir(unsigned short const& filetype)
	{
		return filetype&_S_IFDIR ? true:false;
	}
	static bool is_file(unsigned short const& filetype)
	{
		return filetype&_S_IFREG ? true:false;
	}

	static bool has(character const* path)
	{
		return has(path,path+strkit::length(path));
	}

	static bool has(std_string const& path)
	{
		return has(path.c_str(),path.c_str()+path.length());
	}

	//template <typename string_tn>
	//static bool has_file(string_tn filename)
	//{
	//	character const* id = ox::str::c_string<string_tn>::get(filename);
	//	file_attr_t att;
	//	if (0!=attr(id,att)) return false;
	//	return att.st_mode&_S_IFDIR ? false:true;
	//}

	static bool is_dir(const character* filepath,bool& ret)
	{
		if (!filepath || !*filepath)
			return ret = false;
		file_attr_t att;
		size_t len = strkit::length(filepath);

		character c = pathkit::get_last_slash(filepath,filepath+len);
		character const* file = filepath;
		if (c)
		{
			/// drive
			if (len>=3)
			{
				if (file[len-2]==strkit::colon_character)
				{
					if (0!=attr(file,att)) return false;
					ret = att.st_mode&_S_IFDIR ? true:false;
					return true;
				}
			}

			if (len>1024)
			{
				std_string str(file);
				pathkit::remove_slash(str);
				if (0!=attr(str.c_str(),att)) return false;
			}
			else
			{
				character buff[1024] = {0};
				character* p = strkit::strcat(buff,file);
				buff[len-1] = 0;
				if (0!=attr(buff,att)) return false;
			}
		}
		else
		{
			/// drive
			if (len>=2)
			{
				if (file[len-1]==strkit::colon_character)
				{
					if (len>5) return ret=false;
					character buf[8] = {0};
					strkit::strcat(buf,file);
					buf[len] = strkit::backslash_character;
					if (0!=attr(buf,att)) return false;
					ret = att.st_mode&_S_IFDIR ? true:false;
					return true;
				}
			}
			if (0!=attr(file,att)) return false;
		}

		ret = att.st_mode&_S_IFDIR ? true:false;
		return true;
	}

	/**
		拷贝文件，从一个完整路径到一个完整的路径
		模板参数：const_block_byte_size，表示，外存一次行读取的大小的单位，一般是以族为单位，族一半是
			散区的2的指数幂。
	*/
	template <size_t const_block_byte_size>
	static int cp(const character * from_filename, const character * to_filename, unsigned long flag)
	{
		if (from_filename==0 || to_filename==0)
			return shell_kit_type::__ret_parameter_invalid;

		/// no difference between all characer's size,
		/// so we select the minimized bin data mode
		/// that is, byte binary
		typedef unsigned char bin_byte_t;
		bin_byte_t buffer[const_block_byte_size] = {0};

		ox::io::file<character> from_file;
		ox::io::file<character> to_file;

		typedef ox::character::static_value<character_tn> static_value_t;

		character rb_flag[3] =
		{
			static_value_t::to_static_character<char,'r'>::character_value,
			static_value_t::to_static_character<char,'b'>::character_value,
			static_value_t::null_character
		};
		character wb_flag[3] =
		{
			static_value_t::to_static_character<char,'w'>::character_value,
			static_value_t::to_static_character<char,'b'>::character_value,
			ox::character::static_value<char>::null_character
		};
		if (!from_file.open(from_filename,rb_flag))
			return shell_kit_type::__ret_source_file_not_exist;
		switch (flag)
		{
		case shell_kit_type::__cp_overwrite:
			break;
		case shell_kit_type::__cp_write_under_nofile:
			if (has_file(to_filename))
				return shell_kit_type::__ret_destion_file_has_exist;
		}
		if (!to_file.open(to_filename,wb_flag))
			return shell_kit_type::__ret_destion_file_cannot_new;

		for (size_t once_read_size=0, once_write_size=0; !from_file.eof();)
		{
			once_read_size = from_file.read(buffer,sizeof(bin_byte_t),sizeof(buffer)/sizeof(bin_byte_t));
			//dprintf1("%ld\n",from_file.tell()/1024);
			once_write_size = to_file.write(buffer,sizeof(bin_byte_t),once_read_size);
			if (once_write_size!=once_read_size)
				return shell_kit_type::__ret_file_write_error;
		}

		to_file.flushall();
		return shell_kit_type::__ret_success;
	}
	static int cp(const character * from_filename, const character * to_filename, unsigned long flag)
	{
		return cp<1024*8>(from_filename,to_filename,flag);
	}

	static int mkdir(std_string const& dir)
	{
		return shell_kit_trait::mkdir(dir.c_str());
	}
	static int mkdir(character const* dir)
	{
		return shell_kit_trait::mkdir(dir);
	}
	/**
		递归创建所有路径
	*/
	static int rmkdir(std_string const& dir)
	{
		return rmkdir(dir.c_str());
	}
	static int rmkdir(const character * dir)
	{
		using namespace ox::str;
		typedef string_simple_kit<character>::std_string std_string;

		int ret = -1;

		if (dir==0)
			return ret=shell_kit_type::__ret_parameter_invalid;

		character const seperator= '\\';
		character const seps [] = {'/','\\',0};
		string_simple_kit<character>::strings items;
		string_simple_kit<character>::spliter(dir,seps)(items);

		string_simple_kit<character>::strings::iterator i = items.begin();
		std_string tmp_dir;
		/// 寻找起始不存在的路径
		for (;i!=items.end();i++)
		{
			tmp_dir += *i;
			if(!has_dir(tmp_dir))
				break;
			tmp_dir += seperator;
		}

		/// 如果全部路径都存在，那么就返回真
		if (i==items.end())
			return ret=shell_kit_type::__ret_success;

		tmp_dir += seperator;
		/// 从不存在的路径开始，进行逐个创建，直到完成
		for (ret=shell_kit_type::__ret_success;true;)
		{
			if (shell_kit_trait::mkdir(tmp_dir.c_str())!=0)
			{
				ret=shell_kit_type::__ret_cannot_create_dir;
				break;
			}
			if (++i==items.end())
				break;
			tmp_dir.append(*i) += seperator;
		}
		return ret;
	}

	static int rm(const character * dir)
	{
		return shell_kit_trait::unlink(dir);
	}

	static int rmdir(const character * dir)
	{
		return shell_kit_trait::rmdir(dir);
	}

	/// @ret: ==0: success
	///       !=0(-1): error
	static int attr(const character * path, file_attr_t & file_attr)
	{
		return shell_kit_trait::stat(path,&file_attr);
	}

	static character const* getcwd(character* beff,size_t len)
	{
		return shell_kit_trait::getcwd(beff,len);
	}

	//st_gid - Numeric identifier of group that owns the file (UNIX-specific) This field will always be zero on Windows systems. A redirected file is classified as a Windows file.
	//st_atime - Time of last access of file. Valid on NTFS but not on FAT formatted disk drives.
	//st_ctime - Time of creation of file. Valid on NTFS but not on FAT formatted disk drives.
	//st_dev - Drive number of the disk containing the file (same as st_rdev).
	//st_ino - Number of the information node (the inode) for the file (UNIX-specific). On UNIX file systems, the inode describes the file date and time stamps, permissions, and content. When files are hard-linked to one another, they share the same inode. The inode, and therefore st_ino, has no meaning in the FAT, HPFS, or NTFS file systems.
	//st_mode - Bit mask for file-mode information. The _S_IFDIR bit is set if path specifies a directory; the _S_IFREG bit is set if path specifies an ordinary file or a device. User read/write bits are set according to the file's permission mode; user execute bits are set according to the filename extension.
	//st_mtime - Time of last modification of file.
	//st_nlink - Always 1 on non-NTFS file systems.
	//st_rdev - Drive number of the disk containing the file (same as st_dev).
	//st_size - Size of the file in bytes; a 64-bit integer for variations with the i64 suffix.
	//st_uid - Numeric identifier of user who owns file (UNIX-specific). This field will always be zero on Windows systems. A redirected file is classified as a Windows file.
	//If path refers to a device, the st_size, various time fields, st_dev, and st_rdev fields in the _stat structure are meaningless. Because STAT.H uses the _dev_t type that is defined in TYPES.H, you must include TYPES.H before STAT.H in your code.

};


} // end of shell
} // end of ox

/// 修正历史
/// 修改了has_file特性，使之能正确判断，包括文件和目录
/// 2007-12-13，增加了attr方法，在定义“C”的类型时，要加struct关键字的，否则会出错的