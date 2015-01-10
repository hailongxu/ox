/*
	--------------------------------------------------------------
	类名: ox_directory
	作者: 徐海龙
	日期: 2005-6-20
	文件名: directory.h
	版权：版权归作者所有，本文件可以自由传播
	功能: 独立于MFC的文件操作类，用标准的c库函数封装
		  封装了常用的目录操作，包括遍历整个目录下的所有文件
 */



// dir.h: interface for the ox_dir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRECTORY_H__9DB02075_4883_4E6D_B14C_2225DDC9A76A__INCLUDED_)
#define AFX_DIRECTORY_H__9DB02075_4883_4E6D_B14C_2225DDC9A76A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <io.h>
#include <string.h>
#include <assert.h>

#include <string>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <stdarg.h>
#include <set>
#include <algorithm>

#include <windows.h>

#include "../../kernel/delegate.h"
#include "../pkg/character_trait.h"
#include "../utl/path_kit.h"
#include "./shell_kit.h"
#include "../pkg/string_simple_kit.h"

//////////////////////////////////////////////////////////////////////////
// file mode : 
//		r(+)
//		w(+)
//		a(+)
//		t (text mode)
//		b (binary mode)
//		c (fflush mode)
//		n ()
//template <typename t_char>

namespace ox
{
namespace io
{

struct directory_same_trait
{
	enum nodetype_c
	{
		//normal_file = _A_NORMAL+1, //0x00    /* Normal file - No read/write restrictions */
		//mod_readonly = _A_RDONLY<<1,//=0x01    /* Read only file */
		//mod_hidden = _A_HIDDEN<<1,//0x02    /* Hidden file */
		//mod_system = _A_SYSTEM<<1,//=0x04    /* System file */
		//normal_directory = _A_SUBDIR<<1,//=0x10    /* Subdirectory */
		//normal_archive_file = _A_ARCH<<1//=0x20    /* Archive file */
		attr_normal = FILE_ATTRIBUTE_NORMAL, /* Normal file - No read/write restrictions */
		attr_readonly = FILE_ATTRIBUTE_READONLY, /* Read only file */
		attr_hidden = FILE_ATTRIBUTE_HIDDEN, /* Hidden file */
		attr_system = FILE_ATTRIBUTE_SYSTEM, /* System file */
		attr_directory = FILE_ATTRIBUTE_DIRECTORY, /* Subdirectory */
		attr_archive = FILE_ATTRIBUTE_ARCHIVE /* Archive file */
	};

	enum
	{
		level_bit=1,
		depth_bit=2,
	};

protected:
	typedef unsigned long attribute_t;
	typedef unsigned __int64 filesize_t;
	typedef HANDLE handle_t;
	typedef FILETIME filetime_t;
	static int findclose(handle_t handle) { return FindClose(handle); }
	static bool isvalid(handle_t handle) { return handle!=INVALID_HANDLE_VALUE; }
};


template <typename character_tn>
struct directory_trait;

template <>
struct directory_trait<char> : directory_same_trait
{
	typedef ox::character::character_trait<char>::character character;
	struct finddata_t : WIN32_FIND_DATAA
	{
		attribute_t const& attribute() const { return dwFileAttributes; }
		attribute_t& attribute() { return dwFileAttributes; }
		character* filename() { return cFileName; }
		character const* filename() const { return cFileName; }
		filesize_t filesize() const { return (((filesize_t)nFileSizeHigh)<<32)|((filesize_t)nFileSizeLow); }
		filetime_t& file_writetime() {return ftLastWriteTime;}
		filetime_t const& file_writetime() const {return ftLastWriteTime;}
		void init() {memset(this,0,sizeof(*this));}
		size_t depth; /// from 0
		character const* filedir;
	};
protected:
	static handle_t findfirst(const character *filespec,finddata_t *fileinfo)
	{ return FindFirstFileA(filespec,fileinfo); }
	static bool findnext(handle_t handle,finddata_t *fileinfo)
	{ return FindNextFileA(handle,fileinfo)?true:false; }
};

template <>
struct directory_trait<wchar_t> : directory_same_trait
{
	typedef ox::character::character_trait<wchar_t>::character character;
	struct finddata_t : WIN32_FIND_DATAW
	{
		attribute_t const& attribute() const { return dwFileAttributes; }
		attribute_t& attribute() { return dwFileAttributes; }
		character* filename() { return cFileName; }
		character const* filename() const { return cFileName; }
		filesize_t filesize() const { return (((unsigned __int64)nFileSizeHigh)<<32)|nFileSizeLow; }
		filetime_t const& file_writetime() const {return ftLastWriteTime;}
		void init() {memset(this,0,sizeof(*this));}
		size_t depth; /// from 0
		character const* filedir;
	};
protected:
	static handle_t findfirst(const character *filespec,finddata_t *fileinfo)
	{ return FindFirstFileW(filespec,fileinfo); }
	static bool findnext(handle_t handle,finddata_t *fileinfo)
	{ return FindNextFileW(handle,fileinfo)?true:false; }
};


template <typename character_tn>
struct directory_kit : directory_trait<character_tn>
{
public:
	typedef directory_trait<character_tn> directory_trait;
	typedef typename directory_trait::finddata_t finddata_t;
	typedef typename ox::character::character_trait<character_tn>::character character;
	typedef typename ox::character::static_value<character> static_value_t;
	typedef ox::string::string_simple_kit<character> string_kit;
	typedef ox::utl::localpath_kit<character> pathkit;
	typedef typename string_kit::std_string std_string;
	typedef typename directory_trait::attribute_t attribute_t;
	typedef typename directory_trait::filesize_t filesize_t;
	typedef typename directory_trait::filetime_t filetime_t;

	struct nodeinfo
	{
		std_string id;
		attribute_t type;
		filesize_t size;
		filetime_t timewrite;
	};
	
	typedef std::vector<nodeinfo> filelist_t;
	typedef delegate<size_t(finddata_t const&, size_t)> processnode_func;

public:
	static bool attribute(character const* path_start,finddata_t& fd)
	{
		handle_t fr = findfirst(path_start,&fd);
		return isvalid(fr);
	}
	static bool attribute(std_string const& path_start,finddata_t& fd)
	{
		return attribute(path_start.c_str(),fd);
	}

public:
	directory_kit(bool _is_ignoring_dot=true,bool _is_ignoring_ddot=true)
	{
		is_ignoring_dot = _is_ignoring_dot;
		is_ignoring_ddot = _is_ignoring_ddot;
	}
	
	size_t traverse(std_string const& path_start,filelist_t& files)
	{
		return traverse(path_start.c_str(),files);
	}
	size_t traverse(character const* path_start,filelist_t& files)
	{
		add_node_to_list_handle handle(files,-1,0);
		processnode_func process(&handle);
		return traverse(path_start,process);
	}

	size_t traverse_file(std_string const& path_start,filelist_t& files)
	{
		return traverse_file(path_start.c_str(),files);
	}
	size_t traverse_file(character const* path_start,filelist_t& files)
	{
		return traverse(path_start,files,-1,attr_directory);
	}

	size_t traverse_dir(std_string const& path_start,filelist_t& files)
	{
		return traverse_dir(path_start.c_str(),files);
	}
	size_t traverse_dir(character const* path_start,filelist_t& files)
	{
		return traverse(path_start,files,attr_directory,0);
	}

	size_t traverse(std_string const& path_start,filelist_t& files,attribute_t const& caretypes,attribute_t const& ignoringtypes=0)
	{
		return traverse(path_start.c_str(),files,catetype,ignoringtypes);
	}
	size_t traverse(character const* path_start,filelist_t& files,attribute_t const& caretypes,attribute_t const& ignoringtypes=0)
	{
		add_node_to_list_handle handle(files,caretypes,ignoringtypes);
		processnode_func process(&handle);
		return traverse(path_start,process);
	}

	size_t traverse(std_string const& path_start, processnode_func& process,size_t depth=1)
	{
		return traverse(path_start.c_str(),process,depth);
	}
	size_t traverse(const character* path_start, processnode_func& process,size_t depth=1)
	{
		static ox::character::is_case_sensitive_c const casive = ox::character::__case_sensitive;
		if (path_start==0 || 0==*path_start) return 0;
		if (process.is_empty()) return 0;

		size_t index = 0;
		finddata_t fd;

		fd.depth = depth;
		fd.filedir = path_start;

		handle_t fr = findfirst(path_start,&fd);
		if (!isvalid(fr)) return 0;

		character const dotstr[2] = {'.',0};
		character const ddotstr[3] = {'.','.',0};
		character const asterisk[2] = {'*',0};

		do {
			int rcmp = string_kit::strcmp<casive>(fd.filename(),dotstr);
			bool is_dot = 0==rcmp;
			if (!is_dot || is_dot && !is_ignoring_dot)
				if (!(level_bit & process(fd,index++)))
					break;

			if (!findnext(fr,&fd)) break;

			int rcmp2 = string_kit::strcmp<casive>(fd.filename(),ddotstr);
			bool is_ddot = 0==rcmp2;
			if (!is_ddot || is_ddot && !is_ignoring_ddot)
				if (!(level_bit & process(fd,index++)))
					break;

			std_string subdir;
			while (findnext(fr,&fd))
			{
				size_t ret = process(fd,++index);
				if (!(ret&level_bit)) break;
				if (!(fd.attribute()&attr_directory)) continue;
				if (!(ret&depth_bit)) continue;
				subdir.assign(path_start);
				pathkit::remove_last(subdir);
				pathkit::add_rpath(subdir,fd.filename(),'\\');
				pathkit::add_rpath(subdir,asterisk,'\\');
				traverse(subdir.c_str(),process,depth+1); /// ignore exception
			}
		}
		while(false);

		findclose(fr);

		return index;
	}

	bool is_ignoring_dot;
	bool is_ignoring_ddot;

protected:
	struct add_node_to_list_handle
	{
		add_node_to_list_handle(filelist_t& _files,attribute_t const& caretypes=-1,attribute_t const& ignoringtypes=0)
			: files(_files)
			, attribute_care(caretypes)
			, attribute_ignoring(ignoringtypes)
		{}
		size_t operator()(finddata_t const& fd,size_t index)
		{
			if (!(fd.attribute()&attribute_care))
				return level_bit;
			if (fd.attribute()&attribute_ignoring)
				return level_bit;
			files.push_back(nodeinfo());
			files.back().id = fd.filename();
			files.back().size = fd.filesize();
			files.back().type = fd.attribute();
			files.back().timewrite = fd.file_writetime();
			return level_bit;
		}
		filelist_t& files;
		attribute_t const& attribute_care;
		attribute_t const& attribute_ignoring;
	};
};

template <typename character_tn>
static size_t get_drives(character_tn* drives)
{
	typedef ox::character::static_value<character_tn> static_value_t;

	size_t drivemask = _getdrives();
	if (drivemask == 0)
		return 0;

	int i=0, j=0;
	while (drivemask)
	{
		if (drivemask & 1)
			drives[i++] = static_value_t::get<'A'>::c_value+j;
		drivemask>>=1, ++j;
	}
	drives[i] = 0;
	return i;
}

} // end of io
} // end of ox

#endif // !defined(AFX_DIRECTORY_H__9DB02075_4883_4E6D_B14C_2225DDC9A76A__INCLUDED_)
