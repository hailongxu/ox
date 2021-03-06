
/*
	author: xuhailong
	date: 2011-4-18
	msn: hailongxu@msn.com
	Hi: hailonghsu@baidu.com

	this is a free and open code
	you can use it freely, but you should include the author declaration
 */



#pragma once

namespace ox
{
namespace utl
{

	struct c_free;
	struct cpp_delete;
	struct win_close_handle;
	struct win_release_dc;
	template <typename t,typename f=void>
	struct destructor;

	template <typename t>
	struct destructor<t*,cpp_delete>
	{
		typedef destructor self;
		self(t* p): _m_p(p) {}
		~destructor() {delete _m_p;}
		t* _m_p;
	};

	template <>
	struct destructor<HANDLE,win_close_handle>
	{
		typedef destructor self;
		self(HANDLE h): _m_handle(h) {}
		~destructor() {::CloseHandle(_m_handle);}
		HANDLE _m_handle;
	};

	template <>
	struct destructor<HDC,win_release_dc>
	{
		typedef destructor self;
		self(HWND hwnd,HDC h): _m_hwnd(hwnd),_m_hdc(h) {}
		~destructor() {::ReleaseDC(_m_hwnd,_m_hdc);}
		HWND _m_hwnd;
		HDC _m_hdc;
	};

	template <typename t>
	struct cxx_obj_defer
	{
		typedef cxx_obj_defer self;
		typedef destructor<t,cpp_delete> defer_t;
		self(t* p): _m_destructor(p) {}
		defer_t _m_destructor;
	};

	struct win_handle_defer
	{
		typedef win_handle_defer self;
		typedef destructor<HANDLE,win_close_handle> defer_t;
		self(HANDLE h): _m_destructor(h) {}
		defer_t _m_destructor;
	};
	
	struct win_dc_defer
	{
		typedef win_dc_defer self;
		typedef destructor<HDC,win_release_dc> defer_t;
		self(HWND hwnd,HDC h): _m_destructor(hwnd,h) {}
		defer_t _m_destructor;
	};

}
}