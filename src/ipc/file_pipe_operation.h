

#include <assert.h>
#include "../../ox/ox/nsab.h"
#include "../../ox/comm/communication_const.h"
#include "../../ox/comm/communication_define.h"
#include "../../ox/mos/win_queue_thread.h"
#include "../../ox/comm/file_service_operation.h"


#pragma once


___namespace2_begin(ox,ipc)


struct file_pipe_operation
{
	typedef file_pipe_operation self;
	typedef comm::communication_define::file_handle_t file_handle_t;
	typedef ox::utl::cdata_t cdata_t;
	typedef comm::file_handle_operation file_handle_operation;

	static SECURITY_ATTRIBUTES* get_all_access_security_attributes()
	{
		static bool is_initialized = false;
		static SECURITY_ATTRIBUTES security_attributes;
		static SECURITY_DESCRIPTOR security_descriptor;

		OSVERSIONINFO ver_info;  
		ver_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&ver_info);
		if(ver_info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
			return 0;

		if(is_initialized) return &security_attributes;

		if(!InitializeSecurityDescriptor(&security_descriptor, SECURITY_DESCRIPTOR_REVISION)
			|| !SetSecurityDescriptorDacl(&security_descriptor, TRUE, NULL, FALSE))
			return 0;

		security_attributes.nLength = sizeof(security_attributes); 
		security_attributes.lpSecurityDescriptor = &security_descriptor;
		security_attributes.bInheritHandle = FALSE; 

		is_initialized = true;

		return &security_attributes;
	}
	/// creating
	static file_handle_t create(char const* name,size_t out_buffer_size,size_t in_buffer_size)
	{
		assert(name && *name);
		file_handle_t hpipe = file_handle_operation::invalid_file_handle();
		if (!name || !*name) return file_handle_operation::invalid_file_handle();
		char str_pipe_name[512] = {0};
		sprintf (str_pipe_name,"\\\\.\\pipe\\%s",name);

		DWORD open_mode = PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED; // read/write access 
		//DWORD pipe_mode = PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT; /// used past
		DWORD pipe_mode = PIPE_TYPE_MESSAGE | // message type pipe 
			PIPE_READMODE_MESSAGE | // message-read mode 
			PIPE_WAIT; // blocking mode
		DWORD instance_max = PIPE_UNLIMITED_INSTANCES; //max. instances
		//DWORD time_out = NMPWAIT_USE_DEFAULT_WAIT; // 50 milliseconds
		DWORD time_out = 5000; // 1000 milliseconds

		hpipe = CreateNamedPipeA( 
			str_pipe_name,            // pipe name 
			open_mode,                // read/write access 
			pipe_mode,                // pipe mode
			instance_max,             // max. instances  
			out_buffer_size,          // output buffer size 
			in_buffer_size,           // input buffer size 
			time_out,                 // client time-out 
			0);                       // default security attribute 
		return hpipe;
	}
	/// return error
	static size_t accept(file_handle_t handle_listen)
	{
		size_t errorid = 0;
		bool bconnected = ConnectNamedPipe(handle_listen,0)==TRUE;
		if (!bconnected) errorid=GetLastError();
		if (errorid==ERROR_PIPE_CONNECTED) errorid=0;
		return errorid;
	}
	static size_t aaccept(file_handle_t handle_listen,OVERLAPPED* overlapped)
	{
		assert (overlapped && overlapped->hEvent!=(HANDLE)0 && overlapped->hEvent!=(HANDLE)-1);
		if (overlapped==0) return ox::comm::communication_const::__error_invalid_parameter;
		size_t errorid = 0;
		bool bconnected = ConnectNamedPipe(handle_listen,overlapped)==TRUE;
		if (!bconnected) errorid=GetLastError();
		if (errorid==ERROR_PIPE_CONNECTED) errorid=0;
		else if (errorid==ERROR_IO_PENDING);
		return errorid;
	}

	static file_handle_t connect(char const* name,size_t* errorid)
	{
		return connect(cdata_t(name,strlen(name)),errorid);
	}
	static file_handle_t connect(cdata_t const& name,size_t* errorid)
	{
		bool is_successful = false;
		size_t error_code = 0;
		file_handle_t file_handle = file_handle_operation::invalid_file_handle();
		do {
			char str_pipe_name[512] = "\\\\.\\pipe\\";
			strncat(str_pipe_name,name.begin,name.size);
			file_handle = CreateFileA(
				str_pipe_name, // pipe name 
				GENERIC_READ|GENERIC_WRITE, // read and write access 
				0, // no sharing 
				0, // default security attributes
				OPEN_EXISTING, // opens existing pipe 
				FILE_FLAG_OVERLAPPED, // default attributes, for using of iocp
				0); // no template file 

			// Break if the pipe handle is valid.
			is_successful = !file_handle_operation::is_handle_invalid(file_handle);
			if (is_successful) break;

			error_code = GetLastError();
			is_successful = ERROR_PIPE_BUSY==error_code;
			if (!is_successful) break;

			// All pipe instances are busy, so wait for 2 seconds. 
			is_successful = WaitNamedPipeA(str_pipe_name,2000)==TRUE;
			if (!is_successful) break;

		} while(0);

		if (is_successful)
		{
			DWORD mode = PIPE_READMODE_MESSAGE;//PIPE_READMODE_BYTE; 
			is_successful = SetNamedPipeHandleState( 
				file_handle, // pipe handle 
				&mode, // new pipe mode 
				0, // don't set maximum bytes 
				0) == TRUE; // don't set maximum time 
			//if (!is_successful) break;
		}

		if (!is_successful)
		{
			error_code = GetLastError();
			if (file_handle_operation::is_handle_invalid(file_handle))
				CloseHandle(file_handle);
			file_handle = file_handle_operation::invalid_file_handle();
		}
		if (errorid) *errorid = error_code;
		return file_handle;
	}
	static size_t aget_overlapped_result(file_handle_t handle_listen,OVERLAPPED* overlapped,size_t* bytes_transferred=0)
	{
		DWORD transfered_byte = 0;
		BOOL bret = GetOverlappedResult( 
			handle_listen, // pipe handle 
			overlapped, // OVERLAPPED structure 
			&transfered_byte, // bytes transferred 
			FALSE); // does not wait
		if (bytes_transferred) *bytes_transferred=transfered_byte;
		return bret?0:GetLastError();
	}

	//// global operation
	static int init() {return ox::comm::communication_const::__error_null;}
	static void uninit() {}
	/// file handle operation
	static size_t close(file_handle_t file_handle)
	{
		size_t error_code = 0;
		BOOL bret = CloseHandle(file_handle);
		if (!bret)
			error_code = GetLastError();
		return error_code;
	}
	static int asend(file_handle_t file_handle,cdata_t data,char* /*duration_buf*/,OVERLAPPED* ol)
	{
		size_t error_code = 0;
		DWORD bytes_written = 0;
		BOOL isokay = WriteFile( 
			file_handle, // handle to pipe 
			data.begin, // buffer to write from 
			data.size, // number of bytes to write 
			&bytes_written, // number of bytes written 
			ol); // not overlapped I/O
		if (!isokay)
		{
			error_code = GetLastError();
			if (error_code == ERROR_IO_PENDING)
				error_code = 0;
		}
		return error_code;
	}
	static int arecv(file_handle_t file_handle,cdata_t data,char* /*duration_buf*/,OVERLAPPED* ol)
	{
		size_t error_code = 0;
		DWORD bytes_read = 0;
		BOOL isokay = ReadFile( 
			file_handle, // handle to pipe 
			(void*)data.begin, // buffer to write from 
			data.size, // number of bytes to write 
			&bytes_read, // number of bytes written 
			ol); // not overlapped I/O
		if (!isokay)
		{
			error_code = GetLastError();
			if (error_code == ERROR_IO_PENDING)
				error_code = 0;
		}
		return error_code;
	}
	static int get_last_error()
	{
		return ::GetLastError();
	}
	static file_handle_operation as_file_handle_operation()
	{
		file_handle_operation operation;
		operation.init.assign(self::init);
		operation.uninit.assign(self::uninit);
		operation.close.assign(self::close);
		operation.asend.assign(self::asend);
		operation.arecv.assign(self::arecv);
		operation.last_error.assign(self::get_last_error);
		return operation;
	}
}; /// end of ipc_file_operation


___namespace2_end()