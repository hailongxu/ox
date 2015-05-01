


//#include "winapi.h"
//#include "winsys.h"
#include "windows.h"
#include <string>



#pragma once




namespace sys
{
	static HMODULE get_current_module(bool bref)
	{
		HMODULE hModule = 0;
		DWORD flag = bref ? GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS : (GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT);
		if (GetModuleHandleExW(flag, (LPCWSTR)get_current_module, &hModule))
			return hModule;
		return 0;
	}

	template <typename char_tn>
	static std::basic_string<char_tn> get_current_module_path();
	template <>
	static std::wstring get_current_module_path<wchar_t>()
	{
		HMODULE h = get_current_module(false);
		if (h==0) return L"";
		wchar_t buffer [1024];
		::GetModuleFileNameW(h,buffer,1024);
		return buffer;
	}
	template <>
	static std::string get_current_module_path<char>()
	{
		HMODULE h = get_current_module(false);
		if (h==0) return "";
		char buffer [1024];
		::GetModuleFileNameA(h,buffer,1024);
		return buffer;
	}

	//static __inline std::string __GetDllName()
	//{
	//	char Filename[MAX_PATH];
	//	HMODULE mod = NULL;
	//	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR) GetEip(), &mod);
	//	GetModuleFileNameA(mod, Filename, MAX_PATH);
	//	char *pFind = strrchr(Filename, '\\');
	//	std::string dll(pFind + 1);
	//	return dll;
	//}
	template<typename char_tn>
	static std::basic_string<char_tn> get_current_exe_path();
	template<>
	static std::string get_current_exe_path<char>()
	{
		char path_buffer[1024] = {0};
		path_buffer[1023] = 0;
		::GetModuleFileNameA(::GetModuleHandle(0),path_buffer,1023);
		return std::string(path_buffer);
	}
	template<>
	static std::wstring get_current_exe_path<wchar_t>()
	{
		wchar_t path_buffer[1024] = {0};
		path_buffer[1023] = 0;
		::GetModuleFileNameW(::GetModuleHandle(0),path_buffer,1023);
		return std::wstring(path_buffer);
	}

	struct dll
	{
		dll() :hmodule(0) {}
		dll(char const* dllname) {init(dllname);}
		dll(wchar_t const* dllname) {init(dllname);}
		~dll() {lize();}

		bool init(wchar_t const* dllname)
		{
 			hmodule = LoadLibraryW(dllname);
			return hmodule?true:false;
		}
		bool init(char const* dllname)
		{
			hmodule = LoadLibraryA(dllname);
			return hmodule?true:false;
		}
		bool lize()
		{
			if (!hmodule) return false;
 			BOOL b = FreeLibrary(hmodule);
			if (b) hmodule=0;
			return b?true:false;
		}
		template <typename func_tn>
		func_tn* function(char const* func_name)
		{
 			if (hmodule == 0)
				return 0;
			func_tn* func = (func_tn*)GetProcAddress(hmodule, func_name);
			return func;
		}

		HMODULE hmodule;
	};
	static HRESULT register_com(wchar_t const* dllname)
	{
		typedef HRESULT(__stdcall DllRegisterServer_func)() ;
		sys::dll dll(dllname);
		DllRegisterServer_func* DllRegisterServer = dll.function<DllRegisterServer_func>("DllRegisterServer");
		if (!DllRegisterServer) return S_FALSE;
		return DllRegisterServer();
	}

	static HRESULT unregister_com(wchar_t const* dllname)
	{
		typedef HRESULT(__stdcall DllUnregisterServer_func)() ;
		sys::dll dll(dllname);
		DllUnregisterServer_func* DllUnregisterServer = dll.function<DllUnregisterServer_func>("DllUnregisterServer");
		if (!DllUnregisterServer) return S_FALSE;
		return DllUnregisterServer();
	}



	struct single_process
	{
		single_process() : _m_hmutex(0) {}
		~single_process() {if (_m_hmutex) CloseHandle(_m_hmutex);}
		bool isfirst()
		{
			if (_m_hmutex!=0)
				return false;
			_m_hmutex = CreateMutexW( 
				NULL, // default security attributes
				FALSE, // initially not owned
				L"sp:52C05961-4FA5-4125-B390-56FB9EE61C11" /// sp: single process
				); // unnamed mutex
			if (_m_hmutex==0)
				return false;
			DWORD error = GetLastError();
			if (error==ERROR_ALREADY_EXISTS)
				return false;
			return true;
		}
		HANDLE _m_hmutex;
	};

	static bool is_process_alive(size_t processid)
	{
		DWORD ver = GetProcessVersion(processid);
		return ver?true:false;
	}
}

//
////#include <shguid.h>
//#include <shtypes.h>
//#include <shobjidl.h>
//#include <shlobj.h>
//#include <string>
////#include <atlcomcli.h>
//
//namespace sys
//{
//	static bool create_desktop_shortcut(
//		wchar_t const* strSourcePath, wchar_t const* strName, wchar_t const* param=0,
//		wchar_t const* iconrespath=0, size_t index=0)
//	{
//		if(FAILED(CoInitialize(0)))
//			return false;
//
//		//CComPtr<ITEMIDLIST> pidl = 0;
//		LPITEMIDLIST pidl = 0;
//		LPMALLOC pShell = 0;
//		IShellLink* psl = 0;
//		IPersistFile* ppf = 0;
//		bool bret = false;
//
//		do {
//			wchar_t Path[MAX_PATH+1] = {0};
//
//			if(!SUCCEEDED(SHGetMalloc(&pShell)))
//				break;
//			if(!SUCCEEDED(SHGetSpecialFolderLocation(NULL,CSIDL_DESKTOPDIRECTORY,&pidl)))
//				break;
//			if(!SHGetPathFromIDList(pidl,Path))
//				break;
//
//			std::wstring strDestDir = Path;
//			strDestDir.append(L"\\").append(strName).append(L".lnk");
//// 			
//// 			if(::PathFileExists(strDestDir.c_str()))
//// 			{
//// 				wchar_t* strExt = ::PathFindExtension(strDestDir.c_str());
//// 				if( wcslen(strExt) > 0 && wcsstr(strExt,L"lnk") == 0 )
//// 					DeleteFileW(strDestDir.c_str());
//// 			}
//
//			if(!SUCCEEDED(CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID*)&psl)))
//				break;
//			psl->SetPath(strSourcePath);
//			psl->SetArguments(param?param:L"");
//			if (iconrespath && *iconrespath)
//				psl->SetIconLocation(iconrespath,(int)index);
//			if(!SUCCEEDED(psl->QueryInterface(IID_IPersistFile,(LPVOID*)&ppf)))
//				break;
//			if (SUCCEEDED(ppf->Save(strDestDir.c_str(),TRUE)))
//				bret = true;
//		}
//		while (0);
//
//		if (pShell) pShell->Release();
//		if (pShell) pShell->Free(pidl);
//		if (ppf) ppf->Release();
//		if (psl) psl->Release();
//		::CoUninitialize();
//		return bret;
//	}
//
//	static bool delete_desktop_shortcut(wchar_t const* name) 
//	{
//		if (!name || !*name) return false;
//		wchar_t path[1024];
//		HRESULT hr = SHGetSpecialFolderPath(0,path,CSIDL_DESKTOPDIRECTORY,0);
//		if (!SUCCEEDED(hr)) return false;
//		wcscat(path,L"\\");
//		wcscat(path,name);
//		wcscat(path,L".lnk");
//		return ::DeleteFileW(path)?true:false;
//	}
//
//	static bool create_startup_shortcut(wchar_t const* strSourcePath, wchar_t const* strName, wchar_t const* param=0,
//		wchar_t const* iconrespath=0, size_t index=0)
//	{
//		if(FAILED(CoInitialize(0)))
//			return false;
//
//		//CComPtr<ITEMIDLIST> pidl = 0;
//		LPITEMIDLIST pidl = 0;
//		LPMALLOC pShell = 0;
//		IShellLink* psl = 0;
//		IPersistFile* ppf = 0;
//		bool bret = false;
//
//		do {
//			wchar_t Path[MAX_PATH+1] = {0};
//
//			if(!SUCCEEDED(SHGetMalloc(&pShell)))
//				break;
//			if(!SUCCEEDED(SHGetSpecialFolderLocation(NULL,CSIDL_STARTUP,&pidl)))
//				break;
//			if(!SHGetPathFromIDList(pidl,Path))
//				break;
//
//			std::wstring strDestDir = Path;
//			strDestDir.append(L"\\").append(strName).append(L".lnk");
//
//			if(!SUCCEEDED(CoCreateInstance(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID*)&psl)))
//				break;
//			psl->SetPath(strSourcePath);
//			psl->SetArguments(param?param:L"");
//			if (iconrespath && *iconrespath)
//				psl->SetIconLocation(iconrespath,(int)index);
//			if(!SUCCEEDED(psl->QueryInterface(IID_IPersistFile,(LPVOID*)&ppf)))
//				break;
//			if (SUCCEEDED(ppf->Save(strDestDir.c_str(),TRUE)))
//				bret = true;
//		}
//		while (0);
//
//		if (pShell) pShell->Release();
//		if (pShell) pShell->Free(pidl);
//		if (ppf) ppf->Release();
//		if (psl) psl->Release();
//		::CoUninitialize();
//		return bret;
//	}
//
//	static bool delete_startup_shortcut(wchar_t const* name) 
//	{
//		if (!name || !*name) return false;
//		wchar_t path[1024];
//		HRESULT hr = SHGetSpecialFolderPath(0,path,CSIDL_STARTUP,0);
//		if (!SUCCEEDED(hr)) return false;
//		wcscat(path,L"\\");
//		wcscat(path,name);
//		wcscat(path,L".lnk");
//		return ::DeleteFileW(path)?true:false;
//	}
//
//	static bool is_exist_startup_shortcut(wchar_t const* name) 
//	{
//		if (!name || !*name) return false;
//		wchar_t path[1024];
//		HRESULT hr = SHGetSpecialFolderPath(0,path,CSIDL_STARTUP,0);
//		if (!SUCCEEDED(hr)) return false;
//		wcscat(path,L"\\");
//		wcscat(path,name);
//		wcscat(path,L".lnk");
//		return ::PathFileExists(path)?true:false;
//	}
//
//	static bool resolve_startup_shortcut(wchar_t const* linkname,wchar_t* path)
//	{
//		HWND hWnd = 0;
//		HRESULT hr;
//		IShellLink  * PShLink = 0;
//		WIN32_FIND_DATA wfd;
//		std::wstring lpszLinkName;
//		char lpszPath[MAX_PATH] = {0};
//		char lpszDescription[MAX_PATH] = {0};
//
//		lpszLinkName = f.GetFilePath();
//
//		hr = ::CoInitialize(0) ;
//		if (!SUCCEEDED(hr)) return false;
//
//		hr = CoCreateInstance(CLSID_ShellLink, 0, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&PShLink) ;
//		if (!SUCCEEDED(hres)) return false;
//
//		IPersistFile * ppf = 0;
//		hr = PShLink->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
//		if (!SUCCEEDED(hres)) return false;
//
//		WORD wsz[MAX_PATH] ;
//		MultiByteToWideChar(CP_ACP, 0, lpszLinkName, -1, wsz, MAX_PATH) ;
//		hr = ppf->Load(wsz, STGM_READ) ;
//		if (!SUCCEEDED(hr)) return false;
//
//		hr = PShLink->Resolve(hWnd, SLR_ANY_MATCH|SLR_NO_UI);
//		if(!SUCCEEDED(hr)) return false;
//
//		//这里就可以用类提供的函数得到一些快捷方式的信息了。
//		hr = PShLink->GetPath(lpszPath, MAX_PATH, &wfd, SLGP_SHORTPATH);
//		wcscpy(path,lpszPath);
//
//		if (ppf) ppf->Release();
//		if (PShLink) PShLink->Release();
//		::CoUninitialize();
//		return true;
//	}
//}