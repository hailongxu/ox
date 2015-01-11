/*	
	类名: shell_kit
	作者: 徐海龙
	日期: 2005-6-20
	文件名: win_shell_kit.h
	版权：版权归作者所有，本文件可以自由传播
 */


//#include <windows.h>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <ShellApi.h>
//#include <winbase.h>

#include <string>
#include <vector>
#include <wchar.h> /// for tm
//#include "shell_kit.h"
#include "../utl/path_kit.h"
//
//#pragma comment(lib,"Shellwapi32.lib")


#pragma once


// struct tim
//int tm_sec;     /* seconds after the minute - [0,59] */
//int tm_min;     /* minutes after the hour - [0,59] */
//int tm_hour;    /* hours since midnight - [0,23] */
//int tm_mday;    /* day of the month - [1,31] */
//int tm_mon;     /* months since January - [0,11] */
//int tm_year;    /* years since 1900 */
//int tm_wday;    /* days since Sunday - [0,6] */
//int tm_yday;    /* days since January 1 - [0,365] */
//int tm_isdst;   /* daylight savings time flag */



namespace ox
{
namespace shell
{

	//static int win_delete(const std::wstring& filename)
	//{
	//	std::wstring strFile = std::wstring(filename) + L'\0';

	//	SHFILEOPSTRUCT so;
	//	memset(&so,0,sizeof(SHFILEOPSTRUCT));
	//	so.wFunc = FO_DELETE;
	//	so.pFrom = strFile.data();
	//	so.hwnd = 0;
	//	so.fFlags = FOF_ALLOWUNDO;
	//	return SHFileOperation(&so);
	//}

	static int win_delete(std::wstring const& strfiles,bool silent=false)
	{
		std::wstring strFile = std::wstring(strfiles) + L'\0';

		SHFILEOPSTRUCT so;
		memset(&so,0,sizeof(SHFILEOPSTRUCT));
		so.wFunc = FO_DELETE;
		so.pFrom = strFile.data();
		so.hwnd = 0;
		if (silent)
			so.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
		else
			so.fFlags = FOF_ALLOWUNDO;
		return SHFileOperation(&so);
	}

	static int win_delete(std::vector<std::wstring> const& files,bool silent=false)
	{
		std::wstring strfiles;
		typedef std::vector<std::wstring>::const_iterator I;
		for (I i=files.begin(); i!=files.end(); ++i)
		{
			strfiles += *i;
			strfiles += L'\0';
		}
		strfiles += L'\0';
		return win_delete(strfiles,silent);
	}

	static int win_copy_or_move(const wchar_t* des_path, const std::wstring& src_path, bool bcopy, bool bSameDir)
	{
		SHFILEOPSTRUCT so;
		memset(&so,0,sizeof(SHFILEOPSTRUCT));
		so.wFunc = bcopy ? FO_COPY : FO_MOVE;
		so.pFrom = src_path.data();
		so.pTo = des_path;
		so.hwnd = 0;
		so.fFlags = bSameDir && bcopy ? FOF_ALLOWUNDO | FOF_RENAMEONCOLLISION : FOF_ALLOWUNDO;
		return SHFileOperation(&so);
	}

	static int win_copy_or_move(const wchar_t* des_path,std::vector<std::wstring> const& paths, bool bcopy)
	{
		typedef ox::utl::localpath_kit<wchar_t> pathkit;

		std::wstring str;
		bool same_dir = false;
		std::wstring str_des_path(des_path);
		pathkit::add_slash(str_des_path, L'\\');

		typedef std::vector<std::wstring>::const_iterator I;
		for (I i=paths.begin(); i!=paths.end(); ++i)
		{
			if(!same_dir)
			{
				std::wstring dir = *i;
				bool b = pathkit::remove_last(dir); (void)(b);
				pathkit::add_slash(dir, L'\\');
				same_dir = (str_des_path == dir);
			}

			str += *i;
			str.push_back(L'\0');
		}
		str.push_back(L'\0');
		return win_copy_or_move(des_path,str,bcopy, same_dir);
	}

	static bool win_copy(wchar_t const* src_path,wchar_t const* des_path,bool bfailifexits=true)
	{
		BOOL b = CopyFileW(src_path,des_path,bfailifexits?TRUE:FALSE);
		return b?true:false;
	}

	static int win_rename(const wchar_t* des_path, const wchar_t* src_path, bool silent=false)
	{
		std::wstring src = std::wstring(src_path) + L'\0';

		SHFILEOPSTRUCT so;
		memset(&so,0,sizeof(SHFILEOPSTRUCT));
		so.wFunc = FO_RENAME;
		so.pFrom = src.data();
		so.pTo = des_path;
		so.hwnd = 0;
		if (silent)
			so.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_NOCONFIRMMKDIR;
		else
			so.fFlags = FOF_ALLOWUNDO;
		return SHFileOperation(&so);
	}

	static int create_new_dir(const wchar_t* parent_path, std::wstring& new_folder_name)
	{
		assert(!new_folder_name.empty());

		std::wstring temp_new_folder_name(new_folder_name);
		std::wstring str_parent_path(parent_path);
		ox::utl::localpath_kit<wchar_t>::add_slash(str_parent_path,L'\\');
		std::wstring new_folder_path = str_parent_path + temp_new_folder_name;

		int i = 0;
		while (PathFileExists(new_folder_path.c_str()))
		{
			i++;
			wchar_t szNewFolderName[MAX_PATH] = {0};
			swprintf_s(szNewFolderName, L"%s (%d)", temp_new_folder_name.c_str(), i);
			new_folder_name = szNewFolderName;
			new_folder_path = str_parent_path + new_folder_name;
		}
			
		return CreateDirectoryW(new_folder_path.c_str(), NULL);
	}

	static bool win_movefile(wchar_t const* from, wchar_t const* to)
	{
		BOOL b = MoveFileEx(
			from,
			to,
			MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH);
		return b?true:false;
	}
	static bool win_movefile(std::wstring const& from, std::wstring const& to)
	{
		return win_movefile(from.c_str(),to.c_str());
	}

	static int win_get_temp_dir(std::wstring& tempdir)
	{
		wchar_t szPath[MAX_PATH] = {0};
		HRESULT hr = SHGetFolderPathW(NULL,CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL,0,szPath);
		if (hr!=S_OK)
			return -1;
		tempdir = szPath;
		return 0;
	}
	static int win_create_temp_dir(std::wstring& tempdir,wchar_t const* subdir)
	{
		typedef ox::utl::localpath_kit<wchar_t> pathkit;
		typedef ox::shell::shell_kit<wchar_t> shkit;

		wchar_t szPath[MAX_PATH] = {0};
		HRESULT hr = SHGetFolderPathW(NULL,CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL,0,szPath);
		if (hr!=S_OK)
			return -1;

		tempdir = szPath;
		if (subdir)
			pathkit::add_rpath(tempdir,subdir,L'\\');
		return shkit::rmkdir(tempdir.c_str());
	}

	static std::wstring win_get_user_appdata()
	{
		wchar_t szPath[MAX_PATH] = {0};
		HRESULT hr = SHGetFolderPathW(NULL,CSIDL_APPDATA|CSIDL_FLAG_CREATE, NULL,0,szPath);
		if (hr!=S_OK)
			return std::wstring();
		return std::wstring(szPath);
	}

	static std::wstring get_user_startup()
	{
		wchar_t szPath[MAX_PATH] = {0};
		HRESULT hr = SHGetFolderPathW(NULL,CSIDL_STARTUP|CSIDL_FLAG_CREATE, NULL,0,szPath);
		if (hr!=S_OK)
			return std::wstring();
		return std::wstring(szPath);
	}

	static HMODULE get_current_module(bool bref)
	{
		HMODULE hModule = 0;
		DWORD flag = bref ? GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS : (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT);
		if (GetModuleHandleExW(flag, (LPCWSTR)get_current_module, &hModule))
			return hModule;
		return 0;
	}

	static std::wstring get_current_module_path()
	{
		HMODULE h = get_current_module(false);
		if (h==0) return L"";
		wchar_t buffer [1024];
		::GetModuleFileNameW(h,buffer,1024);
		return buffer;
	}

	static wchar_t get_sysdisk()
	{
		wchar_t dir [1024];
		::GetSystemDirectoryW(dir,1024);
		return dir[0];
	}

	static HANDLE win_open_existing_file(wchar_t const* filename, bool writbale = false)
	{
		DWORD dwDesiredAccess = GENERIC_READ;
		if (writbale) dwDesiredAccess |= GENERIC_WRITE;
		return CreateFileW(
			filename,	//_In_      LPCTSTR lpFileName,
			dwDesiredAccess,	//_In_      DWORD dwDesiredAccess,
			0,	//_In_      DWORD dwShareMode,
			0,	//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,	//_In_      DWORD dwCreationDisposition,
			0,	//_In_      DWORD dwFlagsAndAttributes,
			0	//_In_opt_  HANDLE hTemplateFile
			);
	}
	static HANDLE win_open_existing_file(char const* filename, bool writbale = false, bool sharbale=false)
	{
		DWORD dwDesiredAccess = GENERIC_READ;
		if (writbale) dwDesiredAccess |= GENERIC_WRITE;
		WORD dwShareMode = 0;
		dwShareMode |= FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE;
		HANDLE h = CreateFileA(
			filename,	//_In_      LPCTSTR lpFileName,
			dwDesiredAccess,	//_In_      DWORD dwDesiredAccess,
			dwShareMode,	//_In_      DWORD dwShareMode,
			0,	//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,	//_In_      DWORD dwCreationDisposition,
			0,	//_In_      DWORD dwFlagsAndAttributes,
			0	//_In_opt_  HANDLE hTemplateFile
			);
		int err = 0;
		if (h==INVALID_HANDLE_VALUE) err = GetLastError();
		return h;
	}
	static HANDLE win_open_existing_directory(wchar_t const* filename, bool writbale = false)
	{
		DWORD dwDesiredAccess = GENERIC_READ;
		if (writbale) dwDesiredAccess |= GENERIC_WRITE;
		return CreateFileW(
			filename,	//_In_      LPCTSTR lpFileName,
			dwDesiredAccess,	//_In_      DWORD dwDesiredAccess,
			FILE_SHARE_READ|FILE_SHARE_DELETE,	//_In_      DWORD dwShareMode,
			0,	//_In_opt_  LPSECURITY_ATTRIBUTES lpSecurityAttributes,
			OPEN_EXISTING,	//_In_      DWORD dwCreationDisposition,
			FILE_FLAG_BACKUP_SEMANTICS,	//_In_      DWORD dwFlagsAndAttributes,
			0	//_In_opt_  HANDLE hTemplateFile
			);
	}
	static size_t win_get_file_size(HANDLE handle)
	{
		DWORD size = 0;
		return GetFileSize(handle,&size);
	}

	static void system_time_to_structtm(struct tm& time,SYSTEMTIME const& st)
	{
		time.tm_year = st.wYear;
		time.tm_mon = st.wMonth;
		//st.wDayOfWeek = ;
		time.tm_mday = st.wDay;
		time.tm_hour = st.wHour;
		time.tm_min = st.wMinute;
		time.tm_sec = st.wSecond;
	}
	static int compare(FILETIME const& a,FILETIME const& b)
	{
		return CompareFileTime(&a,&b);
	}
	static bool win_get_filetime(HANDLE hfile,FILETIME* createtime,FILETIME* writetime,FILETIME* accesstime=0)
	{
		if (hfile==INVALID_HANDLE_VALUE)
			return false;
		HANDLE h = hfile;
		BOOL b2 = ::GetFileTime(h,createtime,accesstime,writetime);
		return b2?true:false;
	}
	static void file_time_to_local_system(FILETIME filetime,SYSTEMTIME& systemtime)
	{
		BOOL b3 = FileTimeToLocalFileTime(&filetime,&filetime);
		if (!b3) return;

		SYSTEMTIME st;
		BOOL b4 = FileTimeToSystemTime(&filetime,&st);
		if (!b4) return;
		systemtime = st;
	}
	static bool win_get_filetime(HANDLE hfile,SYSTEMTIME* createtime,SYSTEMTIME* writetime,SYSTEMTIME* accesstime=0)
	{
		if (hfile==INVALID_HANDLE_VALUE)
			return false;
		HANDLE h = hfile;

		FILETIME CreationTime;
		FILETIME LastAccessTime;
		FILETIME LastWriteTime;

		BOOL b2 = ::GetFileTime(h,&CreationTime,&LastAccessTime,&LastWriteTime);
		if (!b2) return false;

		FILETIME* all_filetime [3] = {&CreationTime,&LastWriteTime,&LastWriteTime};
		SYSTEMTIME* all_systemtime [3] = {createtime,writetime,accesstime};

		for (int i=0; i<3; ++i)
		{
			FILETIME* filetime = all_filetime[i];
			SYSTEMTIME* systemtime = all_systemtime[i];

			if (!systemtime) continue;

			BOOL b3 = FileTimeToLocalFileTime(filetime,filetime);
			if (!b3) return false;

			SYSTEMTIME st;
			BOOL b4 = FileTimeToSystemTime(filetime,&st);
			if (!b4) return false;
			*systemtime = st;
		}

		return true;
	}
	static bool win_get_filetime(HANDLE hfile,struct tm* createtime,struct tm* writetime,struct tm* accesstime=0)
	{
		SYSTEMTIME t1,t2,t3;
		if (!win_get_filetime(hfile,&t1,&t2,&t3))
			return false;

		if (createtime) system_time_to_structtm(*createtime,t1);
		if (writetime) system_time_to_structtm(*writetime,t2);
		if (accesstime) system_time_to_structtm(*accesstime,t3);

		return true;
	}
	static size_t win_get_file_size(char const* filename)
	{
		HANDLE h = win_open_existing_file(filename,false);
		if (h==INVALID_HANDLE_VALUE)
			return false;
		size_t size = win_get_file_size(h);
		CloseHandle(h);
		return size;
	}
	static bool win_get_filetime(wchar_t const* filename,struct tm* createtime,struct tm* writetime,struct tm* accesstime=0)
	{
		HANDLE h = win_open_existing_file(filename,false);
		if (h==INVALID_HANDLE_VALUE)
			return false;
		bool b = win_get_filetime(h,createtime,writetime,accesstime);
		CloseHandle(h);
		return b;
	}
	static bool win_get_filetime(char const* filename,struct tm* createtime,struct tm* writetime,struct tm* accesstime=0)
	{
		HANDLE h = win_open_existing_file(filename,false);
		if (h==INVALID_HANDLE_VALUE)
			return false;
		bool b = win_get_filetime(h,createtime,writetime,accesstime);
		CloseHandle(h);
		return b;
	}
	static bool win_get_filetime(char const* filename,FILETIME* createtime,FILETIME* writetime,FILETIME* accesstime=0)
	{
		HANDLE h = win_open_existing_file(filename,false);
		if (h==INVALID_HANDLE_VALUE)
			return false;
		bool b = win_get_filetime(h,createtime,writetime,accesstime);
		CloseHandle(h);
		return b;
	}
	static bool win_modify_filetime(HANDLE hfile,struct tm const* createtime,struct tm const* writetime,struct tm const* accesstime=0)
	{
		if (hfile==INVALID_HANDLE_VALUE)
			return false;
		HANDLE h = hfile;

		FILETIME CreationTime;
		FILETIME LastAccessTime;
		FILETIME LastWriteTime;

		BOOL b2 = ::GetFileTime(h,&CreationTime,&LastAccessTime,&LastWriteTime);
		if (!b2) return false;


		struct tm const* all_srctime[3] = {createtime,writetime,accesstime};
		FILETIME* all_filetime [3] = {&CreationTime,&LastWriteTime,&LastAccessTime};

		for (int i=0; i<3; ++i)
		{
			struct tm const* srctime = all_srctime[i];
			FILETIME* filetime = all_filetime[i];
			if (!srctime) continue;

			//FILETIME LastWriteTime;
			BOOL b3 = FileTimeToLocalFileTime(filetime,filetime);
			if (!b3) return false;

			SYSTEMTIME st;
			BOOL b4 = FileTimeToSystemTime(filetime,&st);
			if (!b4) return false;
			
			st.wYear = (WORD)srctime->tm_year;
			st.wMonth = (WORD)srctime->tm_mon;
			//st.wDayOfWeek = ;
			st.wDay = (WORD)srctime->tm_mday;
			st.wHour = (WORD)srctime->tm_hour;
			st.wMinute = (WORD)srctime->tm_min;
			st.wSecond = (WORD)srctime->tm_sec;
			st.wMilliseconds = 0;

			//int tm_sec;     /* seconds after the minute - [0,59] */
			//int tm_min;     /* minutes after the hour - [0,59] */
			//int tm_hour;    /* hours since midnight - [0,23] */
			//int tm_mday;    /* day of the month - [1,31] */
			//int tm_mon;     /* months since January - [0,11] */
			//int tm_year;    /* years since 1900 */
			//int tm_wday;    /* days since Sunday - [0,6] */
			//int tm_yday;    /* days since January 1 - [0,365] */
			//int tm_isdst;   /* daylight savings time flag */


			BOOL b5 = SystemTimeToFileTime(&st,filetime);
			if (!b5) return false;

			BOOL b6 = LocalFileTimeToFileTime(filetime,filetime);
			if (!b6) return false;
		}

		BOOL b7 = ::SetFileTime(h,&CreationTime,&LastAccessTime,&LastWriteTime);

		return b7?true:false;
	}

	static bool win_modify_filetime(wchar_t const* filename,struct tm const* createtime,struct tm const* writetime,struct tm const* accesstime=0)
	{
		HANDLE h = win_open_existing_file(filename,true);
		if (h==INVALID_HANDLE_VALUE)
			return false;
		bool b = win_modify_filetime(h,createtime,writetime,accesstime);
		CloseHandle(h);
		return b;
	}
	static bool win_modify_filetime(std::wstring const& filename,struct tm const* createtime,struct tm const* writetime,struct tm const* accesstime=0)
	{
		return win_modify_filetime(filename.c_str(),createtime,writetime,accesstime);
	}

	static struct tm filetime2tm(FILETIME const& _filetime)
	{
		FILETIME filetime = _filetime;
		//FILETIME LastWriteTime;
		BOOL b3 = FileTimeToLocalFileTime(&filetime,&filetime);
		if (!b3) return tm();

		SYSTEMTIME st;
		BOOL b4 = FileTimeToSystemTime(&filetime,&st);
		if (!b4) return tm();

		struct tm tm2;
        tm2.tm_sec = st.wSecond;     /* seconds after the minute - [0,59] */
        tm2.tm_min = st.wMinute;     /* minutes after the hour - [0,59] */
        tm2.tm_hour = st.wHour;    /* hours since midnight - [0,23] */
        tm2.tm_mday = st.wDay;    /* day of the month - [1,31] */
        tm2.tm_mon = st.wMonth;     /* months since January - [0,11] */
        tm2.tm_year = st.wYear;    /* years since 1900 */
        //tm2.tm_wday = st.;    /* days since Sunday - [0,6] */
        //tm2.tm_yday = st.;    /* days since January 1 - [0,365] */
        //tm2.tm_isdst = st.;   /* daylight savings time flag */

		return tm2;
	}

} // end of shell
} // end of ox

/// 修正历史
/// 修改了has_file特性，使之能正确判断，包括文件和目录
/// 2007-12-13，增加了attr方法，在定义“C”的类型时，要加struct关键字的，否则会出错的