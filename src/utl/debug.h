//
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*	filename :  debug.h
	---------------------------------------------------------------------
	copyright:  you can freely copy, reuse and distribute the code, 
	            but should add the head comment in front of your file.
	---------------------------------------------------------------------
	Author   :  徐海龙 xuhailong
	Email    :  Hailongxu@etang.com
	MSN      :  HaiLongXu@msn.com
	版权	 :  版权归作者所有，本文件可以自由传播
	Hobby    :  Playing Table Tenis, Playing China Chess, Speaking English, etc.
	            Programing, Linux, C++, Network, etc.
	P.S.     :  You can contract me at any time.
	---------------------------------------------------------------------
	date     :  2006.4.6
	platform :  win32,console platform
	support  :  windows API, standard clib
	---------------------------------------------------------------------
	Desp     :  encapsulate the debug function for the convient
	            这个文件的执行，如果定义了宏 " _DEBUG " 为前提的，所有的都是以此
		        为前提的。
	history: 


/************************************************************************/


#ifndef _____DEBUG_H_____
#define _____DEBUG_H_____

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>

#include "../cor/platform.h"

/************************************************************************/
/*	名称命名规则及其含义
	d-=Debug
	di-=Debug If
	die-=Debug If Else
	a=Argument
	f=Format
	ai=Argument If
	ae=Argument Else
	fi=Format If
	fe=Format Else
/************************************************************************/

// 定义打印调试函数，输出到标准设备
#	ifdef _DEBUG

#		define dprintf(f) printf(f) 
#		define dprintf0(f) printf(f) 
#		define dprintf1(f,a) printf(f,a) 
#		define dprintf2(f,a1,a2) printf(f,a1,a2) 
#		define dprintf3(f,a1,a2,a3) printf(f,a1,a2,a3) 
#		define dprintf4(f,a1,a2,a3,a4) printf(f,a1,a2,a3,a4) 
#		define dprintf5(f,a1,a2,a3,a4,a5) printf(f,a1,a2,a3,a4,a5) 
#		define dprintf6(f,a1,a2,a3,a4,a5,a6) printf(f,a1,a2,a3,a4,a5,a6) 
#		define dprintf7(f,a1,a2,a3,a4,a5,a6,a7) printf(f,a1,a2,a3,a4,a5,a6,a7) 
#		define dprintf8(f,a1,a2,a3,a4,a5,a6,a7,a8) printf(f,a1,a2,a3,a4,a5,a6,a7,a8) 
#		define dprintf9(f,a1,a2,a3,a4,a5,a6,a7,a8,a9) printf(f,a1,a2,a3,a4,a5,a6,a7,a8,a9) 

#	else

#		define dprintf(f)
#		define dprintf0(f)
#		define dprintf1(f,a)
#		define dprintf2(f,a1,a2)
#		define dprintf3(f,a1,a2,a3)
#		define dprintf4(f,a1,a2,a3,a4)
#		define dprintf5(f,a1,a2,a3,a4,a5)
#		define dprintf6(f,a1,a2,a3,a4,a5,a6)
#		define dprintf7(f,a1,a2,a3,a4,a5,a6,a7)
#		define dprintf8(f,a1,a2,a3,a4,a5,a6,a7,a8)
#		define dprintf9(f,a1,a2,a3,a4,a5,a6,a7,a8,a9)

#	endif

// 定义有条件的打印调试函数，输出到标准设备
#	ifdef _DEBUG

#		define diprintf(IF,f) if ((IF)) printf(f) ;
#		define diprintf0(IF,f) if ((IF)) printf(f) ;
#		define diprintf1(IF,f,a) if ((IF)) printf(f,a) ;
#		define diprintf2(IF,f,a1,a2) if ((IF)) printf(f,a1,a2) ;
#		define diprintf3(IF,f,a1,a2,a3) if ((IF)) printf(f,a1,a2,a3) ;
#		define diprintf4(IF,f,a1,a2,a3,a4) if ((IF)) printf(f,a1,a2,a3,a4) ;
#		define diprintf5(IF,f,a1,a2,a3,a4,a5) if ((IF)) printf(f,a1,a2,a3,a4,a5) ;
#		define diprintf6(IF,f,a1,a2,a3,a4,a5,a6) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6) ;
#		define diprintf7(IF,f,a1,a2,a3,a4,a5,a6,a7) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6,a7) ;
#		define diprintf8(IF,f,a1,a2,a3,a4,a5,a6,a7,a8) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6,a7,a8) ;
#		define diprintf9(IF,f,a1,a2,a3,a4,a5,a6,a7,a8,a9) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6,a7,a8,a9) ;

#	else

#		define diprintf(IF,f)
#		define diprintf0(IF,f)
#		define diprintf1(IF,f,a)
#		define diprintf2(IF,f,a1,a2)
#		define diprintf3(IF,f,a1,a2,a3)
#		define diprintf4(IF,f,a1,a2,a3,a4)
#		define diprintf5(IF,f,a1,a2,a3,a4,a5)
#		define diprintf6(IF,f,a1,a2,a3,a4,a5,a6)
#		define diprintf7(IF,f,a1,a2,a3,a4,a5,a6,a7)
#		define diprintf8(IF,f,a1,a2,a3,a4,a5,a6,a7,a8)
#		define diprintf9(IF,f,a1,a2,a3,a4,a5,a6,a7,a8,a9)

#	endif

// 定义if(){}else{}的打印调试函数，输出到标准设备
// {{

#	ifdef _DEBUG

#		define dieprintf(IF,fi,fe) if ((IF)) printf(fi) ; else printf(fe) ;
#		define dieprintf0(IF,fi,fe) if ((IF)) printf(fi) ; else printf(fe) ;
#		define dieprintf1(IF,fi,ai,fe,ae) if ((IF)) printf(fi,ai) ; else printf(fe,ae) ;
//#		define dieprintf2(IF,f,a1,a2) if ((IF)) printf(f,a1,a2) ;
//#		define dieprintf3(IF,f,a1,a2,a3) if ((IF)) printf(f,a1,a2,a3) ;
//#		define dieprintf4(IF,f,a1,a2,a3,a4) if ((IF)) printf(f,a1,a2,a3,a4) ;
//#		define dieprintf5(IF,f,a1,a2,a3,a4,a5) if ((IF)) printf(f,a1,a2,a3,a4,a5) ;
//#		define dieprintf6(IF,f,a1,a2,a3,a4,a5,a6) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6) ;
//#		define dieprintf7(IF,f,a1,a2,a3,a4,a5,a6,a7) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6,a7) ;
//#		define dieprintf8(IF,f,a1,a2,a3,a4,a5,a6,a7,a8) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6,a7,a8) ;
//#		define dieprintf9(IF,f,a1,a2,a3,a4,a5,a6,a7,a8,a9) if ((IF)) printf(f,a1,a2,a3,a4,a5,a6,a7,a8,a9) ;

#	else

#		define dieprintf(IF,fi,fe)
#		define dieprintf0(IF,fi,fe)
#		define dieprintf1(IF,fi,ai,fe,ae)
//#		define dieprintf2(IF,f,a1,a2)
//#		define dieprintf3(IF,f,a1,a2,a3)
//#		define dieprintf4(IF,f,a1,a2,a3,a4)
//#		define dieprintf5(IF,f,a1,a2,a3,a4,a5)
//#		define dieprintf6(IF,f,a1,a2,a3,a4,a5,a6)
//#		define dieprintf7(IF,f,a1,a2,a3,a4,a5,a6,a7)
//#		define dieprintf8(IF,f,a1,a2,a3,a4,a5,a6,a7,a8)
//#		define dieprintf9(IF,f,a1,a2,a3,a4,a5,a6,a7,a8,a9)

#	endif

// }}


// 定义打印调试函数，输出到用户可选的输出设备
#	ifdef _DEBUG

#		define dfprintf(o,f) fprintf(o,f) 
#		define dfprintf0(o,f) fprintf(o,f) 
#		define dfprintf1(o,f,a) fprintf(o,f,a) 
#		define dfprintf2(o,f,a1,a2) fprintf(o,f,a1,a2) 
#		define dfprintf3(o,f,a1,a2,a3) fprintf(o,f,a1,a2,a3) 
#		define dfprintf4(o,f,a1,a2,a3,a4) fprintf(o,f,a1,a2,a3,a4) 
#		define dfprintf5(o,f,a1,a2,a3,a4,a5) fprintf(o,f,a1,a2,a3,a4,a5) 
#		define dfprintf6(o,f,a1,a2,a3,a4,a5,a6) fprintf(o,f,a1,a2,a3,a4,a5,a6) 
#		define dfprintf7(o,f,a1,a2,a3,a4,a5,a6,a7) fprintf(o,f,a1,a2,a3,a4,a5,a6,a7) 
#		define dfprintf8(o,f,a1,a2,a3,a4,a5,a6,a7,a8) fprintf(o,f,a1,a2,a3,a4,a5,a6,a7,a8) 
#		define dfprintf9(o,f,a1,a2,a3,a4,a5,a6,a7,a8,a9) fprintf(o,f,a1,a2,a3,a4,a5,a6,a7,a8,a9) 

#	else

#		define dfprintf(o,f)
#		define dfprintf0(o,f)
#		define dfprintf1(o,f,a)
#		define dfprintf2(o,f,a1,a2)
#		define dfprintf3(o,f,a1,a2,a3)
#		define dfprintf4(o,f,a1,a2,a3,a4)
#		define dfprintf5(o,f,a1,a2,a3,a4,a5)
#		define dfprintf6(o,f,a1,a2,a3,a4,a5,a6)
#		define dfprintf7(o,f,a1,a2,a3,a4,a5,a6,a7)
#		define dfprintf8(o,f,a1,a2,a3,a4,a5,a6,a7,a8)
#		define dfprintf9(o,f,a1,a2,a3,a4,a5,a6,a7,a8,a9)

#	endif


// 定义有条件的打印调试函数，输出到用户可选的输出设备
#	ifdef _DEBUG

#		define difprintf(IF,o,f) if((IF)) fprintf(o,f) ;
#		define difprintf0(IF,o,f) if((IF)) fprintf(o,f) ;
#		define difprintf1(IF,o,f,a) if((IF)) fprintf(o,f,a) ;
#		define difprintf2(IF,o,f,a1,a2) if((IF)) fprintf(o,f,a1,a2) ;
#		define difprintf3(IF,o,f,a1,a2,a3) if((IF)) fprintf(o,f,a1,a2,a3) ;
#		define difprintf4(IF,o,f,a1,a2,a3,a4) if((IF)) fprintf(o,f,a1,a2,a3,a4) ;
#		define difprintf5(IF,o,f,a1,a2,a3,a4,a5) if((IF)) fprintf(o,f,a1,a2,a3,a4,a5) ;
#		define difprintf6(IF,o,f,a1,a2,a3,a4,a5,a6) if((IF)) fprintf(o,f,a1,a2,a3,a4,a5,a6) ;
#		define difprintf7(IF,o,f,a1,a2,a3,a4,a5,a6,a7) if((IF)) fprintf(o,f,a1,a2,a3,a4,a5,a6,a7) ;
#		define difprintf8(IF,o,f,a1,a2,a3,a4,a5,a6,a7,a8) if((IF)) fprintf(o,f,a1,a2,a3,a4,a5,a6,a7,a8) ;
#		define difprintf9(IF,o,f,a1,a2,a3,a4,a5,a6,a7,a8,a9) if((IF)) fprintf(o,f,a1,a2,a3,a4,a5,a6,a7,a8,a9) ;

#	else

#		define difprintf(IF,o,f)
#		define difprintf0(IF,o,f)
#		define difprintf1(IF,o,f,a)
#		define difprintf2(IF,o,f,a1,a2)
#		define difprintf3(IF,o,f,a1,a2,a3)
#		define difprintf4(IF,o,f,a1,a2,a3,a4)
#		define difprintf5(IF,o,f,a1,a2,a3,a4,a5)
#		define difprintf6(IF,o,f,a1,a2,a3,a4,a5,a6)
#		define difprintf7(IF,o,f,a1,a2,a3,a4,a5,a6,a7)
#		define difprintf8(IF,o,f,a1,a2,a3,a4,a5,a6,a7,a8)
#		define difprintf9(IF,o,f,a1,a2,a3,a4,a5,a6,a7,a8,a9)

#	endif



#	ifdef _DEBUG

	inline int dprint(const char * format, ...)
	{
		va_list arglist;
		va_start (arglist, format);
		return vprintf(format, arglist);
	}

	template <typename T>
	inline int dprinta(T array[], size_t array_len, bool isEachLn=false)
	{
		printf("[%d]=",0);
		std::cout<<array[0];
		for (size_t n=1; n<array_len; n++)
		{
			printf(",%c[%d]=",(isEachLn?'\0':'\n'),n);
			std::cout<<array[n];
		}
		return 0;
	}

#	else

	inline int dprint(const char * format, ...) {}

	template <typename T>
	inline int dprinta(T array[], size_t array_len) {}

#	endif


/************************************************************************/
/* 如果使用下面的，会在非_DEBUG版下，产生这个写函数的空函数
   不过，如果使用优化选项的话，编译器会把这空函数优化掉的
/************************************************************************/

#	ifdef _DEBUG

	inline void OUTPUTDEBUG_VARINT(int start, ...) 
	{ 
		va_list arg_ptr; 
		int nArgValue = start;
		int nArgCout = 0; 

		va_start(arg_ptr,start);
		do 
		{
			++nArgCout;
			printf("the %d th arg: %d\n",nArgCout,nArgValue);
			nArgValue = va_arg(arg_ptr,int);
		}	while(nArgValue != -1);               
		va_end(arg_ptr);

		return; 
	}

	inline void OUTPUTDEBUG_VARSTR(char *msg, ...)  
	{  
		va_list argp; // 定义保存函数参数的结构
		int argno = 0; // 纪录参数个数 */  
		char * para = msg; // 存放取出的字符串参数

		va_start(argp, msg);  // argp指向传入的第一个可选参数，msg是最后一个确定的参数
		printf("arg: \n");
		while (1)
		{  
			if (strcmp( para, "\0") == 0)  // 采用空串指示参数输入结束
			break;  
			printf(" #%d : %s\n", argno, para);  
			para = va_arg(argp, char *); // 取出当前的参数，类型为char * 
			argno++;  
		} 
		va_end(argp); // 将argp置为NULL

		return ;  
	 }

#	else

	inline void OUTPUTDEBUG_VARINT(int start, ...) {}
	inline void OUTPUTDEBUG_VARSTR( char *msg, ... ) {}

#	endif


	// 调试类，打印文件
	template <ox::constant::os_type_c os>
	class debug
	{
	public:
		template<typename char_t> void print();
	};

	template<>
	class debug<ox::constant::os_type_t::__os_window>
	{
	public:
		template<typename char_t>	void print (char_t * str);
		template<>					void print<char> (char * str) { /*OutputDebugStringA(str)*/; }
		template<>					void print<wchar_t> (wchar_t * str) { /*OutputDebugStringW(str)*/; }
	};



#endif // _____DEBUG_H_____