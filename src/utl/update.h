#include <string>
#include <vector>
#include <time.h>
#include <algorithm>

#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#include <Wininet.h>
#pragma comment (lib, "Wininet.lib")

#include <windns.h>
#pragma comment (lib, "Dnsapi.lib")


#include "../io/win_shell_kit.h"
#include "../utl/path_kit.h"

#pragma once


namespace ox
{
	struct update
	{
		typedef ox::utl::localpath_kit<wchar_t> pathkit;

		/// true, the program will cotinue
		/// false, break the execution
		bool go(int argc, wchar_t** argv)
		{
			if (argc>=4 && 0==wcscmp(argv[1],L"clone"))
			{
				HANDLE hProcessOrig = (HANDLE) _ttoi(argv[2]);
				clone_run(hProcessOrig,argv[3]);
				return false;
			}
			else if (is_overtime() || argc>=2 && 0==wcscmp(argv[1],L"deleteme"))
			{
				::MessageBoxA(0,"该内测版本试用期已到，程序将会自动删除\n请下载新的内测版本","",0);
				delete_me();
				return false;
			}

			if (!is_online() || !is_enabled_gate())
			{
				::MessageBoxA(0,"内测版本，只能在百度内网下使用","",0);
				return false;
			}

			return true;
		}

		bool is_enabled_gate()
		{
			char* gatesway_enabled [] =
			{
				"172.22.231.2",
				"172.22.248.1",
				"172.22.244.1",
				"172.22.225.1",
				"172.31.23.1",
				"172.22.238.1"
			};
			std::vector<std::string> alist;
			get_gateway_list(alist);
			typedef std::vector<std::string>::iterator I;
			for (int i=0; i<sizeof(gatesway_enabled)/sizeof(char*); ++i)
			{
				I ig = std::find(alist.begin(),alist.end(),gatesway_enabled[i]);
				if (ig!=alist.end()) return true;
			}
			return false;
		}


		bool is_online()
		{
			DWORD state;
			size_t reserved = 0;
			BOOL b = InternetGetConnectedState(&state,(DWORD)reserved)? TRUE: FALSE;
			return b && state&INTERNET_CONNECTION_LAN;
		}

		bool is_overtime()
		{
			time_t rawtime = time(0);
			struct tm* st = localtime(&rawtime);
			st->tm_year += 1900;
			st->tm_mon += 1;

			return st->tm_year>=2011 && st->tm_mon>=9 && st->tm_mday>=5;
		}

		bool get_dns()
		{
			char sDomainName[100];
			DWORD size = 100;
			DNS_STATUS res = DnsQueryConfig(
				//DnsConfigFullHostName_A,
				//DnsConfigPrimaryDomainName_A,
				DnsConfigAdapterDomainName_A,
				FALSE,
				NULL,
				NULL,
				sDomainName,
				&size
				);
			return true;
		}

		bool get_dns2()
		{
			char name[128];
			gethostname(name,128);

			FIXED_INFO *pFixedInfo;
			ULONG ulOutBufLen;
			DWORD dwRetVal;
			//IP_ADDR_STRING *pIPAddr;

			pFixedInfo = new FIXED_INFO;
			if (pFixedInfo == NULL)
				return false;
			ulOutBufLen = sizeof (FIXED_INFO);

			if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
			{
				delete pFixedInfo;
				pFixedInfo = (FIXED_INFO*) new char [ulOutBufLen];
				if (pFixedInfo == NULL)
					return false;
			}

			if (dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR)
				return true;
			return false;
		}


		bool get_gateway_list(std::vector<std::string>& gateways)
		{
			PIP_ADAPTER_INFO pAdapterInfo;
			PIP_ADAPTER_INFO pAdapter = NULL;
			DWORD dwRetVal = 0;
			//UINT i;

			//char buffer[32];
			//errno_t error;

			ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
			pAdapterInfo = new IP_ADAPTER_INFO;
			if (pAdapterInfo == NULL) return false;

			if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
			{
				delete pAdapterInfo; pAdapterInfo=0;
				pAdapterInfo = (PIP_ADAPTER_INFO)new char [ulOutBufLen];
				if (pAdapterInfo == NULL) return false;
			}

			dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
			if (dwRetVal!=NO_ERROR) return false;

			pAdapter = pAdapterInfo;
			while (pAdapter)
			{
				gateways.push_back(pAdapter->GatewayList.IpAddress.String);
				pAdapter = pAdapter->Next;
			}

			if (pAdapterInfo)
				delete[] (char*)(pAdapterInfo), pAdapterInfo=0;

			return true;
		}

		int delete_me ()
		{
			// Original EXE: Spawn clone EXE to delete this EXE
			// Copy this EXEcutable image into the user's temp directory        　
			wchar_t szPathOrig[_MAX_PATH], szPathClone[_MAX_PATH];
			GetModuleFileNameW(NULL, szPathOrig, _MAX_PATH);
			GetTempPathW(_MAX_PATH, szPathClone);
			GetTempFileNameW(szPathClone, L"Del", 0, szPathClone);
			CopyFileW(szPathOrig, szPathClone, FALSE);
			// Open the clone EXE using FILE_FLAG_DELETE_ON_CLOSE
			HANDLE hfile = CreateFileW(szPathClone, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, NULL);
			// Spawn the clone EXE passing it our EXE's process handle
			// and the full path name to the Original EXE file.
			TCHAR szCmdLine[1024];
			HANDLE hProcessOrig = OpenProcess(SYNCHRONIZE, TRUE, GetCurrentProcessId());
			wsprintf(szCmdLine, L"%s clone %d \"%s\"", szPathClone, hProcessOrig, szPathOrig);
			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi;
			CreateProcessW(NULL, szCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
			CloseHandle(hProcessOrig);
			CloseHandle(hfile);
			// This original process can now terminate.
			return 0;
		}

		void clone_run(HANDLE hProcessOrig, wchar_t const* orginal_path)
		{
			// Clone EXE: When original EXE terminates, delete it
			//HANDLE hProcessOrig = (HANDLE) _ttoi(__targv[2]);
			WaitForSingleObject(hProcessOrig, INFINITE);
			CloseHandle(hProcessOrig);
			DeleteFileW(orginal_path);
			std::wstring path = orginal_path;
			pathkit::remove_last(path);
			std::wstring str = L"now will delete the dir:\n" + path;
			//if (IDOK==::MessageBoxW(0,str.c_str(),L"",MB_OKCANCEL))
				ox::shell::win_delete(path,true);
			// Insert code here to remove the subdirectory too (if desired).
			// The system will delete the clone EXE automatically
			// because it was opened with FILE_FLAG_DELETE_ON_CLOSE
		}
	};

} /// end of ox