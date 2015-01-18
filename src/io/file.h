/*
	--------------------------------------------------------------
	类名: ox_stdfile
	作者: 徐海龙
	日期: 2005-6-20
	文件名: StdFile.h
	版权：版权归作者所有，本文件可以自由传播
	功能: 独立于MFC的文件操作类，用标准的c库函数封装
 */



// StdFile.h: interface for the XStdFile class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_STDFILE_H__9DB02075_4883_4E6D_B14C_2225DDC9A76A__INCLUDED_)
//#define AFX_STDFILE_H__9DB02075_4883_4E6D_B14C_2225DDC9A76A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma warning(push)
#pragma warning(disable:4099)


#include <stdio.h>
#include <string.h>
#include <string>
#include <io.h>
#include <stdarg.h>
#include <assert.h>
//#include <internal.h>

#include "../pkg/character_trait.h"
#include "../pkg/string_simple_kit.h"

//////////////////////////////////////////////////////////////////////////
// file mode : 
//		r - read only, if not exist, fail
//		w - write only, if not exist, fail; if exist, destroy the exist content
//		a - append only, if not exist, create
//		r+ - read, write; if not exist, fail
//		w+ - read, write; if not exist, create a empty; if exist, destroy the content
//		a+ - read, append; if not exist, create a empty
//		t (text mode)
//		b (binary mode)
//		c (fflush mode)
//		n ()

namespace ox
{
	namespace io
	{

		struct file_helper
		{
			enum seek_mode_c
			{
				__seek_currrent = SEEK_CUR,
				__seek_begin = SEEK_SET,
				__seek_end = SEEK_END
			};
		};


		template <typename character_tn>
		struct file_trait;

		struct file_same_trait
		{
			typedef file_helper helper;
			int fileno(FILE* pfile) const { return _fileno(pfile); }
			int fclose(FILE* pfile) { return ::fclose(pfile); }
			long filelength(int fd) const { return _filelength(fd); }

			size_t fread(void* buffer,size_t size,size_t count,FILE* pfile) { return ::fread(buffer,size,count,pfile); }
			size_t fwrite(const void *buffer, size_t size, size_t count,FILE* pfile) { return ::fwrite(buffer,size,count,pfile);}
			/// 0:successful, !0:error
			int fseek (long offset, int origin,FILE* pfile) {return ::fseek(pfile,offset,origin);}
			long ftell(FILE* pfile) const {return ::ftell(pfile);}
			void rewind(FILE* pfile) {::rewind(pfile);}
			bool feof(FILE* pfile) const { return ::feof(pfile)!=0; }
			int fflush(FILE* pfile) {return ::fflush(pfile); }
			int chsize(int fd,long size) { return ::chsize(fd, size); }
			static int flushall () { return ::_flushall(); }
			static int closeall() {return ::_fcloseall();}
			void lock(FILE* pfile) {return _lock_file(pfile);}
			void unlock(FILE*pfile) {return _unlock_file(pfile);}
		};

		template <>
		struct file_trait<char> :
			virtual public ox::character::character_trait<char>
		{
			FILE *fopen(const character* filename, const character* mode) { return ::fopen(filename,mode); }
			FILE *freopen(const character* path, const character* mode, FILE* pfile) { return ::freopen(path,mode,pfile ); }
			character *fgets (character *string, int n,FILE* pfile) {return ::fgets(string,n,pfile);}
			int fputs (const character *string,FILE* pfile) {return ::fputs(string,pfile);	}
			character fgetc (FILE* pfile) {return (character)::fgetc(pfile);}
			character fputc (character c,FILE* pfile) {return (character)::fputc(c,pfile);}
			int vfprintf (const character * format, va_list arglist, FILE* pfile) {return ::vfprintf(pfile, format, arglist);}
			//int vdfscanf (const character * format, va_list arglist, FILE* pfile) {return vfscanf(format,arglist, pfile);}
			int fgetc_nolock(FILE *pfile){return _fgetc_nolock(pfile);}

		};

		template <>
		struct file_trait<wchar_t> :
			//public file_same_trait,
			virtual public ox::character::character_trait<wchar_t>
		{
			FILE *fopen(const character* filename, const character* mode) { return _wfopen(filename,mode); }
			FILE *freopen(const character* path, const character* mode, FILE* pfile) { return _wfreopen(path,mode,pfile ); } 
			character *fgets (character *string, int n,FILE* pfile) {return fgetws(string,n,pfile);}
			int fputs (const character *string,FILE* pfile) {return fputws(string,pfile);	}
			character fgetc (FILE* pfile) {return ::fgetwc(pfile);}
			character fputc (character c,FILE* pfile) {return ::fputwc(c,pfile);}
			int vfprintf (const character * format, va_list arglist,FILE* pfile) {return vfwprintf(pfile, format, arglist);}
			//int vfscanf (const character * format, va_list arglist, FILE* pfile) {return ::vfscanf(_input_l,pfile, format, 0,arglist);}
			character fgetwc_nolock(FILE *pfile){return _fgetc_nolock(pfile);}
		};

		template <typename character_tn>
		struct logconsole;

		template <typename character_tn>
		class file :
			virtual protected file_same_trait,
			virtual protected file_trait<character_tn>
		{
			friend class ox::io::logconsole<character_tn>;
		public:
			typedef file_trait<character_tn> stdfile_trait_t;
			typedef typename stdfile_trait_t::character character;
			typedef ox::str::string_simple_kit<character> s_kit;
			typedef typename s_kit::std_string std_string;
			typedef file_same_trait stdfile_same_trait_t;

			file():pfile(0) {}
			file(const character *filename, const character *mode)
			{
				open(filename,mode);
			}
			virtual ~file(){ close();}

		public:
			std_string const& filename() const {return str_file_name;}
			bool isopen() {return pfile!=0;}
			bool open (const character *filename, const character *mode)
			{
				if(filename==0)
				{
					close();
					return false;
				}
				if(s_kit::length(filename)==0)
				{
					close();
					return false;
				}
				pfile = fopen(filename,mode);
				if (pfile)
				{
					str_file_name = filename;
					str_file_mode = mode;
				}
				//assert(pfile);
				return pfile!=0;
			}
			bool reopen (const character *filename=0, const character *mode=0)
			{
				return reopen(filename,mode,pfile);
				//pfile = freopen(
				//	filename==0?str_file_name.c_str():filename,
				//	mode==0?str_file_mode.c_str():mode,pfile);
				//if (pfile)
				//{
				//	str_file_name = filename;
				//	str_file_mode = mode;
				//}
				//return pfile!=0;
			}

			bool close ()
			{
				bool bRet=true;
				if(pfile!=0)
					bRet = (stdfile_same_trait_t::fclose(pfile)==0);
				if(bRet)
					pfile = 0;
				return bRet;
			}


			long length ()	{return stdfile_same_trait_t::filelength (stdfile_same_trait_t::fileno(pfile));}
			
			/**
				size: item size
				count: item count
				return the count of items
				return =0 : the end of the file, or some error accour
			*/
			size_t read (char* buffer, size_t size, size_t count) {return stdfile_same_trait_t::fread((void*)buffer,size,count,pfile);}
			size_t write (char* buffer, size_t size, size_t count) {return stdfile_same_trait_t::fwrite((void*)buffer,size,count,pfile);}
			int flush () { return stdfile_same_trait_t::fflush(pfile); }
			int seek (helper::seek_mode_c origin, long offset) {return stdfile_same_trait_t::fseek(offset,origin,pfile);}
			long tell()	{return stdfile_same_trait_t::ftell(pfile);}
			void rewind() { stdfile_same_trait_t::rewind(pfile);}
			bool eof() { return stdfile_same_trait_t::feof(pfile); }
			size_t read_all(std_string& content)
			{
				seek(helper::__seek_end,0);
				size_t len = tell();
				assert (len%sizeof(character)==0);
				content.resize((len+sizeof(character)/2)/sizeof(character));
				rewind();
				size_t size = read((character*)content.data(),sizeof(character),content.size());
				content.resize(size);
				return size;
			}

			// 获取一个字符串，最大为n，如果这个字符串的长度大于n，则只取n个字符，
			// 如果这个字符串的长度小于n，则只取这个字符串的长度
			character * gets (character *string, int n) {return stdfile_trait_t::fgets(string,n,pfile);}
			int puts (const character *string) {return stdfile_trait_t::fputs(string,pfile);}
			int getc () {return stdfile_trait_t::fgetc(pfile);}
			int putc (int c) {return stdfile_trait_t::fputc(c,pfile);}
			int printf (const character * format, ...)
			{
				va_list arglist;
				va_start(arglist, format);
				return vprintf (format, arglist);
			}
			int vprintf (const character * format, va_list arglist)
			{
				return stdfile_trait_t::vfprintf(format, arglist, pfile);
			}


			FILE * to_fileptr() { return pfile; }
			inline int fileno () { return stdfile_same_trait_t::fileno(pfile); }

		public:
			int chsize(size_t new_size) { return stdfile_same_trait_t::chsize(fileno(),static_cast<long>(new_size)); }
			int truncate() { return chsize(0); }

		public:
			static int flushall () { return stdfile_same_trait_t::flushall();}
			static int closeall () { return stdfile_same_trait_t::closeall();}
			static void lock() { stdfile_same_trait_t::lock(pfile);}
			static void unlock() { stdfile_same_trait_t::unlock(pfile);}
		protected:
			bool reopen (const character *filename, const character *mode, FILE* pf)
			{
				pfile = freopen(
					filename==0?str_file_name.c_str():filename,
					mode==0?str_file_mode.c_str():mode,pf);
				if (pfile)
				{
					str_file_name = filename;
					str_file_mode = mode;
				}
				return pfile!=0;
			}

		private:
			FILE * pfile;
			std_string str_file_name;
			std_string str_file_mode;
		};

	} // end of io
} // end of ox

#pragma warning(pop)
//#endif // !defined(AFX_STDFILE_H__9DB02075_4883_4E6D_B14C_2225DDC9A76A__INCLUDED_)
