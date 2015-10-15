// vector_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../../src/fit/vector.h"


struct act
{
	bool operator()(int& v)
	{
		printf("%d\n",v);
		return true;
	}
	bool operator()(char& v)
	{
		printf("%c\n",v);
		return true;
	}
};

namespace vector_test
{
	void test()
	{
		typedef ox::fit::vector<int> V;
		V v;
		v.push_back(1);
		v.push_back(3);
		v.insert(1,2);
		v.insert(1,4);
		v.foreach(act());
		v.erase(1);
		//typedef ox::fit::static_vector<int[4]> SV;
		//SV sv;
		//sv.push_back(1);
		//sv.push_back(2);
		//sv.push_back(3);
		//sv.push_back(4);
		////sv.push_back(5);
		//typedef ox::fit::string<char> S;
		//S s;
		//s.push_back('1');
		//s.push_back('3');
		//s.insert(1,'2');
		//s.insert(1,'4');
		//typedef ox::fit::static_string<char[4]> SS;
		//SS ss;
		//ss.push_back('1');
		//ss.push_back('2');
		//ss.push_back('3');
		//ss.push_back('4');
	}
}



namespace string_test
{
	void test()
	{
		typedef ox::fit::string<char> V;
		V v;
		v.push_back('1');
		v.push_back('3');
		v.insert(1,'2');
		v.insert(1,'4');
		printf("%s\n",v.data());
		v.foreach(act());
		v.erase(1);
		printf("%s",v.data());
		//typedef ox::fit::static_vector<int[4]> SV;
		//SV sv;
		//sv.push_back(1);
		//sv.push_back(2);
		//sv.push_back(3);
		//sv.push_back(4);
		////sv.push_back(5);
		//typedef ox::fit::string<char> S;
		//S s;
		//s.push_back('1');
		//s.push_back('3');
		//s.insert(1,'2');
		//s.insert(1,'4');
		//typedef ox::fit::static_string<char[4]> SS;
		//SS ss;
		//ss.push_back('1');
		//ss.push_back('2');
		//ss.push_back('3');
		//ss.push_back('4');
	}
}

void test_open();
namespace open_url_test
{
	void test()
	{
		test_open();
	}
}


#include <string>
int _tmain(int argc, _TCHAR* argv[])
{

	switch(1) { default:
	string_test::test();break;
	vector_test::test();break;
	open_url_test::test();break;
	};
	return 0;
}


#include <windows.h>
#include <shlobj.h>
#include <string>

namespace Util
{
	namespace Url
	{
		inline DWORD ExcuteWait( LPCWSTR strExePath,LPCWSTR strCmdLine = NULL,BOOL bWait = FALSE ,BOOL runas = FALSE)
		{
			SHELLEXECUTEINFO sei = {0};
			//it will use runas to elevate
			sei.cbSize = sizeof(SHELLEXECUTEINFO);
			sei.fMask = SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI; 
			sei.hwnd = NULL;
			if (runas)
			{
				sei.lpVerb = L"runas";
			}
			else
			{
				sei.lpVerb = L"open";
			}
			sei.lpFile = strExePath;
			if (strCmdLine)
			{
				sei.lpParameters = strCmdLine;
			} 
			sei.nShow = SW_SHOWNORMAL; 
			if (::ShellExecuteEx(&sei) != TRUE)
			{
				::MessageBoxW(0,strExePath,L"false",0);
				return GetLastError();
			}
			else if (bWait)
			{ 
				::WaitForSingleObject(sei.hProcess,2 * 60 * 60 * 1000);
				DWORD ret_val = 0;
				if (::GetExitCodeProcess(sei.hProcess, &ret_val)) 
				{  
					return ret_val;
				}
			}
			char buff[32];
			sprintf(buff,"%u",GetLastError());
			::MessageBoxA(0,buff,0,0);
			return 0;
		}

		inline std::wstring GetIEInstallPath()
		{
			LONG    lStatus;
			HKEY    hKey = NULL;
			std::wstring iePath;

			lStatus = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
				TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\IEXPLORE.EXE"),
				0,
				KEY_READ,
				&hKey);
			WCHAR buffer[MAX_PATH] = {0};
			DWORD bufsize = sizeof(buffer);
			do {
				if (lStatus != ERROR_SUCCESS)
					break;
				lStatus = RegQueryValueEx(hKey,TEXT("Path"),NULL,NULL,(LPBYTE)buffer,&bufsize);
				if (lStatus != ERROR_SUCCESS || bufsize == 0)
					break;
				iePath = buffer;
				int pos = (int)iePath.find(TEXT(";"));
				if (pos != std::wstring::npos){
					iePath = iePath.substr(0,pos);
				}
				iePath = iePath + TEXT("\\iexplore.exe");
			} while (false);
			do {
				if (!iePath.empty()){
					break;
				}
				if (!SHGetSpecialFolderPath(NULL,buffer,CSIDL_PROGRAM_FILESX86,0)){
					break;
				}
				iePath = buffer;
				iePath = iePath + L"\\Internet Explorer\\iexplore.exe";
			} while (false);
			
			return iePath;
		}

		inline wchar_t * wcslcat(wchar_t * dst, size_t len, const wchar_t * src)
		{
			wchar_t * const dst0 = dst;
			if (!dst || !src) return dst0;
			size_t len1 = 0;
			while (*dst)
			{
				++ len1;
				++ dst;
			}
			while (len1+1 < len && *src)
			{
				*dst = *src;
				++ dst;
				++ src;
				++ len1;
			}
			if (len1 < len)
			{
				* dst = 0;
			}
			return dst0;
		}

		inline wchar_t * wcslcpy(wchar_t * dst, size_t len, const wchar_t * src)
		{
			if (!dst || len == 0) return dst;
			dst[0] = 0;
			return wcslcat(dst, len, src);
		}

		inline bool OpenUrlWithIEInternal(LPCTSTR strUrl)
		{
			std::wstring iePath = GetIEInstallPath();
			if (iePath.empty()){
				return false;
			}

			return ExcuteWait(iePath.c_str(),strUrl) == 0;
		}

        /**
        * @brief 获得百度浏览器的安装目录和全路径
        * @param[out] baiduBrowserDir 返回百度浏览器的安装目录，最后不含'\'，形如"C:\xx\yy"
        * @param[out] baiduBrowserFullPath 返回百度浏览器主程序的全路径，形如"C:\xx\yy\baidubrowser.exe"
        * @return 是否存在百度浏览器,true:存在，false：不存在
        * @note 若百度浏览器6.x与7.x共存时优先返回7.x的路径
        */
        inline bool GetBaiduBrowserFullPath(std::wstring& baiduBrowserDir, std::wstring &baiduBrowserFullPath)
        {
            bool ret = false;

            HKEY hKey = NULL;
            do
            {
                bool isBaiduBrowser7X = false;


                if (ERROR_SUCCESS != RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Baidu\\BaiduBrowser", 0, KEY_READ, &hKey))
                {
                    ret = false;
                    break;
                }


                WCHAR installX[MAX_PATH] = {0};
                DWORD installX_len = sizeof(installX);
                if (ERROR_SUCCESS != RegQueryValueExW(hKey, L"installX", NULL, REG_NONE, (LPBYTE)&installX, &installX_len))
                {
                    isBaiduBrowser7X = false;
                }
                else
                {
                    if (installX[0] == L'1')
                    {
                        isBaiduBrowser7X = true;
                    }
                    else
                    {
                        isBaiduBrowser7X =false;
                    }
                }


                WCHAR installDir[MAX_PATH] = {0};
                DWORD installDirLen = sizeof(installDir);

                if (isBaiduBrowser7X)
                {
                    if (ERROR_SUCCESS != RegQueryValueExW(hKey, L"InstallDirX", NULL, REG_NONE, (LPBYTE)&installDir, &installDirLen))
                    {
                        break;
                    }

                    WCHAR version[MAX_PATH] = {0};
                    DWORD versionLen = sizeof(version);
                    if (ERROR_SUCCESS != RegQueryValueExW(hKey, L"VersionX", NULL, REG_NONE, (LPBYTE)&version, &versionLen))
                    {
                        break;
                    }

                    swprintf(installDir, MAX_PATH, L"%s\\%s", installDir, version);
                }
                else
                {
                    if (ERROR_SUCCESS != RegQueryValueExW(hKey, L"InstallDir", NULL, REG_NONE, (LPBYTE)&installDir, &installDirLen))
                    {
                        break;
                    }

                }

                baiduBrowserDir = installDir;
                baiduBrowserFullPath = baiduBrowserDir;
                baiduBrowserFullPath += L"\\baidubrowser.exe";
                ret = true;

            } while (0);

            if (hKey)
            {
                RegCloseKey(hKey);
            }

            return ret;
        }

		inline bool OpenUrlWithBBInternal(LPCTSTR strUrl)
		{
			HINSTANCE hCode = 0;
            std::wstring bddir;
            std::wstring bdpath;
            bool bbfind = GetBaiduBrowserFullPath(bddir, bdpath);

            if (!bbfind)
            {
                return false;
            }

            hCode = ::ShellExecute(::GetDesktopWindow(), _T("open"), bdpath.c_str(), strUrl, NULL, SW_SHOW);

			return (INT_PTR)hCode > 32;
		}

		inline bool OpenUrlWithDefBrowserInternal(LPCTSTR strUrl)
		{
			if (ExcuteWait(strUrl) == 0)
			{
				return true;
			}
			return false;
		}

		inline bool OpenUrl(LPCTSTR strUrl)
		{
			::MessageBoxA(0,"OpenUrlWithBBInternal",0,0);
			bool bRet = OpenUrlWithBBInternal(strUrl);
			if (bRet)
			{
				return bRet;
			}
			else
			{
				::MessageBoxA(0,"OpenUrlWithDefBrowserInternal",0,0);
				if (OpenUrlWithDefBrowserInternal(strUrl))
				{
					return true;
				}
				else
				{
					::MessageBoxA(0,"OpenUrlWithIEInternal",0,0);
					return OpenUrlWithIEInternal(strUrl);
				}
			}
		}
	}
}

void test_open()
{
	Util::Url::OpenUrl(L"www.baidu.com");
}