


#include <windows.h>
#include "../ox/nsab.h"




#pragma once



___namespace2_begin(ox,gui)


template <typename character_tn>
struct wingui_helper_tt;


template <>
struct wingui_helper_tt <char>
{
	typedef char character;
	typedef ::WNDCLASSA WNDCLASS;
	static HWND find_window(HWND hWndParent,HWND hWndChildAfter,character const* lpszClass,character const* lpszWindow)
	{
		return ::FindWindowExA(hWndParent,hWndChildAfter,lpszClass,lpszWindow);
	}
	static LONG_PTR get_window_long(HWND hWnd,int nIndex)
	{
		return ::GetWindowLongPtrA(hWnd,nIndex);
	}
	static LRESULT def_window_proc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		return ::DefWindowProcA(hWnd,Msg,wParam,lParam);
	}
	static LRESULT send_message(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		return ::SendMessageA(hWnd,Msg,wParam,lParam);
	}
	static BOOL get_class_info(HINSTANCE hInstance,character const* lpClassName,WNDCLASS* lpWndClass)
	{
		return ::GetClassInfoA(hInstance,lpClassName,lpWndClass);
	}
	static HMODULE get_module_handle(character const* lpModuleName)
	{
		return ::GetModuleHandleA(lpModuleName);
	}
	static HWND create_window(
		character const* lpClassName,
		character const* lpWindowName,
		DWORD dwStyle,
		int x,
		int y,
		int nWidth,
		int nHeight,
		HWND hWndParent,
		HMENU hMenu,
		HINSTANCE hInstance,
		LPVOID lpParam
	)
	{
		return ::CreateWindowA(
			lpClassName,lpWindowName,
			dwStyle,x,y,nWidth,nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);
	}
	static LONG_PTR set_window_long(HWND hWnd,int nIndex,LONG_PTR dwNewLong)
	{
		return ::SetWindowLongPtrA(hWnd,nIndex,dwNewLong);
	}
	static BOOL destroy_window(HWND hWnd)
	{
		return ::DestroyWindow(hWnd);
	}
	static BOOL unregiste_class(character const* lpClassName,HINSTANCE hInstance)
	{
		return ::UnregisterClassA(lpClassName,hInstance);
	}
};
template <>
struct wingui_helper_tt <wchar_t>
{
	typedef WNDCLASSW WNDCLASS;
	typedef wchar_t character;
	static HWND find_window(HWND hWndParent,HWND hWndChildAfter,character const* lpszClass,character const* lpszWindow)
	{
		return ::FindWindowExW(hWndParent,hWndChildAfter,lpszClass,lpszWindow);
	}
	static LONG_PTR get_window_long(HWND hWnd,int nIndex)
	{
		return ::GetWindowLongPtrW(hWnd,nIndex);
	}
	static LRESULT def_window_proc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		return ::DefWindowProcW(hWnd,Msg,wParam,lParam);
	}
	static LRESULT send_message(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
	{
		return ::SendMessageW(hWnd,Msg,wParam,lParam);
	}
	static LRESULT send_message_timeout(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam,UINT nflag,UINT ntimeout,DWORD* lpdwResult)
	{
		return ::SendMessageTimeoutW(hWnd,Msg,wParam,lParam,nflag,ntimeout,lpdwResult);
	}
	static BOOL get_class_info(HINSTANCE hInstance,character const* lpClassName,WNDCLASS* lpWndClass)
	{
		return ::GetClassInfoW(hInstance,lpClassName,lpWndClass);
	}
	static HMODULE get_module_handle(character const* lpModuleName)
	{
		return ::GetModuleHandleW(lpModuleName);
	}
	static HWND create_window(
		character const* lpClassName,
		character const* lpWindowName,
		DWORD dwStyle,
		int x,
		int y,
		int nWidth,
		int nHeight,
		HWND hWndParent,
		HMENU hMenu,
		HINSTANCE hInstance,
		LPVOID lpParam
	)
	{
		return ::CreateWindowW(
			lpClassName,lpWindowName,
			dwStyle,x,y,nWidth,nHeight,
			hWndParent,
			hMenu,
			hInstance,
			lpParam
			);
	}
	static LONG_PTR set_window_long(HWND hWnd,int nIndex,LONG_PTR dwNewLong)
	{
		return ::SetWindowLongPtrW(hWnd,nIndex,dwNewLong);
	}
	static BOOL destroy_window(HWND hWnd)
	{
		return ::DestroyWindow(hWnd);
	}
	static BOOL unregister_class(character const* lpClassName,HINSTANCE hInstance)
	{
		return ::UnregisterClassW(lpClassName,hInstance);
	}
};

___namespace2_end()