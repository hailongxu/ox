

#include <Windows.h>
#include <assert.h>
#include <string>
#include "error_const.h"


#pragma once


namespace ox
{
	struct shared_file
	{
		shared_file()
			: _m_handle_sharememory(INVALID_HANDLE_VALUE)
		{}
		bool is_opened() const
		{
			return _m_handle_sharememory != INVALID_HANDLE_VALUE;
		}
		size_t create(char const* name,size_t size)
		{
			size_t error_code = 0;
			char share_name[1024] = {0};
			strcat(share_name,"Global\\");
			strcat(share_name,name);
			_m_handle_sharememory = CreateFileMappingA(
				INVALID_HANDLE_VALUE, /// just in memory
				0,
				PAGE_READWRITE,
				0,
				size,
				share_name);
			if (_m_handle_sharememory==0)
				error_code = GetLastError();
			if (error_code==0) _m_file_size = size;
			return error_code;
		}
		size_t open(char const* name)
		{
			size_t error_code = 0;
			char share_name[1024] = {0};
			strcat(share_name,"Global\\");
			strcat(share_name,name);
			_m_handle_sharememory = OpenFileMappingA(
				FILE_MAP_ALL_ACCESS, // read/write access
				FALSE, // do not inherit the name
				share_name); // name of mapping object
			if (_m_handle_sharememory==0)
				error_code = GetLastError();
			return error_code;
		}
		size_t open_or_create(char const* name,size_t size)
		{
			size_t error_code = open(name);
			if (error_code==0)
				return error_code;
			error_code = create(name,size);
			return error_code;
		}
		void close()
		{
			CloseHandle(_m_handle_sharememory);
			_m_handle_sharememory = INVALID_HANDLE_VALUE;
		}
		char* load(size_t* errorcode=0)
		{
			char* pdata = (char*)MapViewOfFile(
				_m_handle_sharememory,
				FILE_MAP_READ|FILE_MAP_WRITE,0,0,0);
			if (!pdata && errorcode) *errorcode=GetLastError();
			assert (pdata);
			return pdata;
		}
		char* load_read(size_t* errorcode=0)
		{
			return load_read(0,0,errorcode);
		}
		char* load_read(size_t offset,size_t size,size_t* errorcode=0)
		{
			char* pdata = (char*)MapViewOfFile(
				_m_handle_sharememory,
				FILE_MAP_READ,0,offset,size);
			if (!pdata && errorcode) *errorcode=GetLastError();
			assert (pdata);
			return pdata;
		}
		char* load_write(size_t* errorcode=0)
		{
			char* pdata = (char*)MapViewOfFile(
				_m_handle_sharememory,
				FILE_MAP_WRITE,0,0,0);
			if (!pdata && errorcode) *errorcode=GetLastError();
			assert (pdata);
			return pdata;
		}
		char* load_write(size_t offset,size_t* errorcode=0)
		{
			//if (offset>_m_meta._m_capacity)
			//	DebugBreak();
			char* pdata = (char*)MapViewOfFile(
				_m_handle_sharememory,
				FILE_MAP_WRITE,0,offset,0);
			if (!pdata && errorcode) *errorcode=GetLastError();
			assert (pdata);
			return pdata;
		}
		char* load_write(size_t offset,size_t size,size_t* errorcode=0)
		{
			//if (offset+size>_m_meta._m_capacity)
			//	DebugBreak();
			char* pdata = (char*)MapViewOfFile(
				_m_handle_sharememory,
				FILE_MAP_WRITE,0,offset,size);
			if (!pdata && errorcode) *errorcode=GetLastError();
			assert (pdata);
			return pdata;
		}
		char* load_read_write(size_t offset,size_t size,size_t* errorcode=0)
		{
			//if (offset+size>_m_meta._m_capacity)
			//	DebugBreak();
			SYSTEM_INFO si;
			GetSystemInfo(&si); 
			si.dwAllocationGranularity;
			static size_t const __block_size = 64*1024;
			size_t file_offset = offset & 0xffff;
			char* pdata = (char*)MapViewOfFile(
				_m_handle_sharememory,
				FILE_MAP_WRITE|FILE_MAP_READ,0,offset,size);
			if (!pdata && errorcode) *errorcode=GetLastError();
			assert (pdata);
			return pdata;
		}
		template <typename data_tn>
		data_tn* load_read_write(size_t offset,size_t* errorcode=0)
		{
			char* pdata = load_read_write(offset,sizeof(data_tn),errorcode);
			return reinterpret_cast<data_tn*>(pdata);
		}
		size_t unload(char* pdata)
		{
			assert (pdata);
			BOOL b = UnmapViewOfFile(pdata);
			assert (b);
			return b?0:GetLastError();
		}
		HANDLE _m_handle_sharememory;
		size_t _m_file_size;
	}; /// share file
		
	struct shared_memory
	{
		shared_memory()
		{}
		/// the user defined file
		struct meta_t
		{
			meta_t() : _m_capacity(0), _m_begin(0), _m_size(0) {}
			__int32 _m_capacity;
			__int32 _m_begin;
			__int32 _m_size;
			bool operator==(meta_t const& o) const
			{
				return _m_capacity==o._m_capacity &&
					_m_begin==o._m_begin &&
					_m_size==o._m_size;
			}
			bool operator!=(meta_t const& o) const
			{
				return !operator==(o);
			}
		};
		size_t create(char const* name,size_t size)
		{
			size_t errorid = _m_share_file.create(name,__meta_size+size);
			if (errorid!=0) return errorid;
			meta_t meta;
			meta._m_capacity = size;
			meta._m_begin = 0;
			meta._m_size = 0;
			errorid = write_meta(meta);
			return errorid;
		}
		size_t open(char const* name)
		{
			return _m_share_file.open(name);
		}
		size_t open_or_create(char const* name,size_t size)
		{
			size_t error_code = _m_share_file.open(name);
			if (error_code==0)
				return error_code;
			error_code = create(name,size);
			return error_code;
		}
		bool is_opened() const
		{
			return _m_share_file.is_opened();
		}
		size_t meta_size() const
		{
			return __meta_size; /// [capacity][begin][size]
		}
		static const size_t __meta_size = sizeof(meta_t);
		size_t read_meta(meta_t& meta)
		{
			/// get meta data
			char* pdata = _m_share_file.load_read(0,__meta_size);
			if (!pdata)
				return GetLastError();
			__int32 memory_capacity = *(__int32*)(pdata);
			__int32 memory_begin = *(__int32*)(pdata+4);
			__int32 memory_size = *(__int32*)(pdata+8);
			_m_share_file.unload(pdata);
			do {
				if (memory_size>memory_capacity)
					DebugBreak();
				if (memory_capacity!=_m_meta._m_capacity)
					DebugBreak();
				if (memory_begin!=_m_meta._m_begin)
					DebugBreak();
				if (memory_size!=_m_meta._m_size)
					DebugBreak();
			} while(0);
			meta._m_capacity = memory_capacity;
			meta._m_begin = memory_begin;
			meta._m_size = memory_size;
			return 0;
		}
		size_t write_meta(meta_t const& meta)
		{
			/// get meta data
			char* pdata = _m_share_file.load_write(0,sizeof(meta));
			if (!pdata)
				return GetLastError();
			meta_t& meta_file = *(meta_t*)(pdata);
			do {
				if (meta._m_size>meta._m_capacity)
					DebugBreak();
				if (_m_meta!=meta_file)
					DebugBreak();
			} while(0);
			if (meta._m_capacity!=-1)
			{
				meta_file._m_capacity = meta._m_capacity;
				_m_meta._m_capacity = meta._m_capacity;
			}
			if (meta._m_begin!=-1)
			{
				meta_file._m_begin = meta._m_begin;
				_m_meta._m_begin = meta._m_begin;
			}
			if (meta._m_size!=-1)
			{
				meta_file._m_size = meta._m_size;
				_m_meta._m_size = meta._m_size;
			}
			_m_share_file.unload(pdata);
			return 0;
		}
		size_t read_all(std::string& data)
		{
			data.clear();
			char* pdata = _m_share_file.load_read();
			if (!pdata)
				return GetLastError();
			meta_t& meta = *(meta_t*)(pdata);
			if (meta._m_size>meta._m_capacity)
			{
				DebugBreak();
			}
			__try
			{
				data.append(pdata+__meta_size,meta._m_size);
			}
			__except(GetExceptionCode()==EXCEPTION_IN_PAGE_ERROR ?EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				size_t errorid = GetLastError();
				bool b = _m_share_file.unload(pdata);
				return errorid;
			}
			bool b = _m_share_file.unload(pdata);
			return 0;
		}
		size_t write_all(char const* data,size_t size)
		{
			char* pdata = _m_share_file.load_read_write(0,0);
			if (!pdata)
				return GetLastError();
			meta_t& meta = *(meta_t*)(pdata);
			__try
			{
				meta._m_begin = 0;
				meta._m_size = size;
				memmove(pdata+sizeof(meta),data,size);
				_m_meta._m_begin = meta._m_begin;
				_m_meta._m_size = meta._m_size;
			}
			__except (GetExceptionCode() == EXCEPTION_IN_PAGE_ERROR ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				size_t errorid = GetLastError();
				bool b = _m_share_file.unload(pdata);
				return errorid;
			}
			size_t errorid = _m_share_file.unload(pdata);
			assert(errorid==0);
			if (errorid!=0) DebugBreak();
			return 0;
		}
		/// return error
		size_t append(char const* data,size_t size)
		{
			size_t errorid = 0;
			
			/// map all the file
			char* pfile = _m_share_file.load_read_write(0,0,&errorid);
			if (!pfile)
			{
				assert (false);
				return errorid;
			}
			meta_t& meta = *(meta_t*)pfile;
			char* pdata = pfile+__meta_size;

			/// get data
			size_t data_size = size;
			if (meta._m_begin+meta._m_size+size>meta._m_capacity)
			{
				return error_const::__error_file_is_not_that_long; /// file is too short
			}
			size_t data_offset = meta._m_begin+meta._m_size;
			__try
			{
				meta._m_size += data_size;
				memmove(pdata+data_offset,data,data_size);
				_m_meta._m_size += data_size;
			}
			__except (GetExceptionCode() == EXCEPTION_IN_PAGE_ERROR ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				errorid = GetLastError();
			}
			size_t errorid2 = _m_share_file.unload(pfile);
			assert (errorid2==0);
			if (errorid==0 && errorid2!=0) errorid=errorid2;
			if (errorid!=0 || errorid2!=0) DebugBreak();
			return errorid;
		}
		size_t append_circle(char const* data,size_t size)
		{
			size_t errorid = 0;
			/// get meta data
			char* pfile = _m_share_file.load_read_write(0,0,&errorid);
			if (!pfile)
			{
				assert (false);
				return errorid;
			}
			meta_t& meta = *(meta_t*)pfile;
			char* pdata = pfile+__meta_size;

			__try
			{
				meta_t tmeta = meta;
				if (size<=tmeta._m_capacity-tmeta._m_begin-tmeta._m_size)
				{
					/// just copy it once
					size_t data_offset = tmeta._m_begin+tmeta._m_size;
					meta._m_size += size;
					char* p = pdata+data_offset;
					memmove(p,data,size);
					_m_meta._m_size += size;
				}
				else if (size<tmeta._m_capacity)
				{
					size_t data_offset = tmeta._m_begin+tmeta._m_size;
					size_t memory_size = tmeta._m_capacity-tmeta._m_begin-tmeta._m_size;
					char* p = pdata+data_offset;
					memmove(p,data,memory_size); /// copy former at the tail
					memmove(pdata,data+memory_size,size-memory_size); /// copy latter at the head
					if (meta._m_size>=meta._m_capacity);
					else if (meta._m_size+size>meta._m_capacity) meta._m_size=meta._m_capacity;
					else meta._m_size+=size;
				}
				else /// (size>=tmeta._m_capacity)
				{
					/// copy once
					tmeta._m_begin = 0;
					tmeta._m_size = tmeta._m_capacity;
					data += (size-tmeta._m_capacity);
					size = tmeta._m_capacity;
					meta._m_begin = 0;
					meta._m_size = meta._m_capacity;
					memmove(pdata,data,size);
				}
			}
			__except (GetExceptionCode() == EXCEPTION_IN_PAGE_ERROR ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
			{
				errorid = GetLastError();
			}
			size_t errorid2 = _m_share_file.unload(pfile);
			assert (errorid2==0);
			if (errorid==0 && errorid2!=0) errorid=errorid2;
			if (errorid!=0 || errorid2!=0) DebugBreak();
			return errorid;
		}
		meta_t _m_meta; /// just for debugging
		shared_file _m_share_file;
	}; /// end of share memory
}