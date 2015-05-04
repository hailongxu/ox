
#include <windows.h>
#include "../ox/nsab.h"
#include "../cxx/delegate.h"


#pragma once


___namespace2_begin(ox,gui)


typedef delegate<void()> on_before_func;

struct message_loop
{
	message_loop(HINSTANCE hinstance=0,size_t id_accletable=0)
		: _m_hinstance(hinstance)
		, _m_id_accletable(id_accletable)
	{}

	void run(size_t before_messageid)
	{
		MSG msg;
		HACCEL hAccelTable = LoadAccelerators(_m_hinstance, MAKEINTRESOURCE(_m_id_accletable));
		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.message == before_messageid && !on_before.is_empty())
			{
				on_before();
			}
			else if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	on_before_func on_before;
	HINSTANCE _m_hinstance;
	size_t _m_id_accletable;
};

struct window_proc
{
	typedef delegate<LRESULT(UINT message,WPARAM wParam,LPARAM lParam)> window_proc_d;
	window_proc_d on_proc;
	HWND _m_hwnd;
	static LRESULT proc_default(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProcA(hWnd,message,wParam,lParam);
	}
};

struct window_class
{
	static LRESULT CALLBACK def_wndproc_exit(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		window_proc* proc = reinterpret_cast<window_proc*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));
		do {
			if (!proc) break;
			assert(!proc->on_proc.is_empty());
			if (message==WM_DESTROY)
			{
				SetWindowLongPtrA(proc->_m_hwnd,GWLP_USERDATA,0);
				break;
			}
			return proc->on_proc(message,wParam,lParam);
		} while(0);
		return window_proc::proc_default(hWnd,message,wParam,lParam);
	}

	window_class()
	{
		init_zero();
		wcex.lpszClassName = "window_class_test";
		wcex.hInstance = 0;
	}
	window_class(HINSTANCE hinstance)
	{
		init_zero();
		wcex.lpszClassName = "window_class_test";
		wcex.hInstance = hinstance;
	}
	window_class(char* winclassname)
	{
		init_zero();
		wcex.lpszClassName = winclassname;
		wcex.hInstance = 0;
	}

	HINSTANCE& hinstance()
	{
		return wcex.hInstance;
	}
	
	/*
		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	*/

	//void set_window_proc(WNDPROC winproc)
	//{
	//	wcex.lpfnWndProc = defwinproc_class::def_wndproc_exit;
	//}
	void set_class_name(char const* lpszClassName)
	{
		wcex.lpszClassName = lpszClassName;
	}
	void set_instance(HINSTANCE instance)
	{
		wcex.hInstance = instance;
	}
	char const* class_name() const
	{
		return wcex.lpszClassName;
	}

	size_t create()
	{
		wcex.lpfnWndProc = def_wndproc_exit;
		ATOM r = RegisterClassExA(&wcex);
		assert (r);
		return r;
	}
	void set_menu(size_t idmenu=0)
	{
		wcex.lpszMenuName = MAKEINTRESOURCEA(idmenu);
	}

private:	
	void init_zero()
	{
		wcex.cbSize = sizeof(WNDCLASSEXA);

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= DefWindowProcA;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= 0;
		wcex.hIcon			= LoadIconA(wcex.hInstance, MAKEINTRESOURCEA(0));
		wcex.hCursor		= LoadCursorA(NULL, (char*)IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= MAKEINTRESOURCEA(0);
		wcex.lpszClassName	= 0;
		wcex.hIconSm		= LoadIconA(wcex.hInstance, MAKEINTRESOURCEA(0));
	}

public:
	WNDCLASSEXA wcex;
};


struct window
{
	typedef window self;

	static size_t const __show_just = SW_SHOW;
	static size_t const __show_default = SW_SHOWDEFAULT;
	static size_t const __show_normal = SW_SHOWNORMAL;

	typedef delegate<void(unsigned short)> on_menu_click_func;
	typedef delegate<void(HWND,HDC)> on_paint_d;
	typedef delegate<bool()> on_exit_func;

	window()
		: _m_windowclass(0)
		, _m_hwnd(0)
	{}

	window(window_class* _class)
		: _m_windowclass(_class)
		, _m_hwnd(0)
	{}

	bool create_show_update(char* wintitle)
	{
		if (!create(wintitle)) assert(false);
		show();
		if (!update()) assert(false);
		return true;
	}

	bool create(char* szTitle)
	{
		assert (_m_windowclass);
		assert (!_m_hwnd);
		char const* szWindowClass = _m_windowclass->wcex.lpszClassName;

		_m_hwnd = CreateWindowA(
			szWindowClass, /// class name
			szTitle, /// window name
			WS_OVERLAPPEDWINDOW, /// dwstyle
			CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, /// x, y, width, height
			0, /// window parent
			0, /// handle of menu
			hinstance(),/// GetModuleHandle(0),
			0 /// lpCreateParams
			);
		if (!_m_hwnd)
		{
			DWORD err = GetLastError();
		}
		_m_on_win_proc._m_hwnd = _m_hwnd;
		_m_on_win_proc.on_proc.assign(this,&self::on_window_proc);
		SetWindowLongPtrA(_m_hwnd,GWLP_USERDATA,reinterpret_cast<LPARAM>(&_m_on_win_proc));  

		return _m_hwnd!=0;
	}

	bool create_as_message(char* szTitle)
	{
		assert (_m_windowclass);
		assert (!_m_hwnd);
		char const* szWindowClass = _m_windowclass->wcex.lpszClassName;

		_m_hwnd = CreateWindowA(
			szWindowClass, /// class name
			szTitle, /// window name
			0, /// dwstyle
			0,0,0,0, /// x, y, width, height
			HWND_MESSAGE, /// window parent
			0, /// handle of menu
			hinstance(),/// GetModuleHandle(0),
			0 /// lpCreateParams
			);
		SetWindowLongPtrA(_m_hwnd,GWLP_USERDATA,reinterpret_cast<LPARAM>(this));  

		return _m_hwnd;
	}

	void show(int nCmdShow = __show_normal)
	{
		ShowWindow(_m_hwnd, nCmdShow);
	}

	bool update()
	{
		return UpdateWindow(_m_hwnd)?true:false;
	}

	HINSTANCE hinstance() const { return _m_windowclass->wcex.hInstance; }
	HWND hwnd() const { return _m_hwnd; }
	window_class*& winclass() { return _m_windowclass; }

	LRESULT on_window_proc(UINT message,WPARAM wParam,LPARAM lParam)
	{
		HDC hdc;
		PAINTSTRUCT ps;
		/*if (message == WM_COMMAND)
		{
			unsigned short wmEvent = HIWORD(wParam);
			if (wmEvent==0) /// is clicked from menu
				if (!instance()->on_menu_click.is_empty())
					instance()->on_menu_click(hWnd,LOWORD(wParam));
			return 0;
		}
		else */if (message == WM_PAINT)
		{
			hdc = BeginPaint(_m_hwnd, &ps);
			if (!on_paint.is_empty())
				on_paint(_m_hwnd,hdc);
			EndPaint(_m_hwnd, &ps);
			return 0;
		}
		return window_proc::proc_default(_m_hwnd,message,wParam,lParam);
	}

	on_paint_d on_paint;
private:
	window_proc _m_on_win_proc;
	HWND _m_hwnd;
	window_class* _m_windowclass;
};


template <typename user_dailog_tn, size_t id_tc>
struct dialog
{
	static size_t const __id = id_tc;

	typedef delegate<HWND()> on_get_hdlg_func;
	typedef delegate<bool()> on_init_func;
	typedef delegate<bool()> on_before_exit_func;

	dialog()
		: _m_hparent(0)
		, _m_exe_handle(0)
		, _m_hdlg(0)
	{}

	dialog(HWND hparent)
		: _m_hparent(hparent)
		, _m_exe_handle(0)
		, _m_hdlg(0)
	{}

	struct dialog_proc_class
	{
		on_init_func on_init_dialog;
		on_before_exit_func on_before_exit;
		
		static INT_PTR CALLBACK dialog_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
		{
			UNREFERENCED_PARAMETER(lParam);
			switch (message)
			{
			case WM_INITDIALOG:
				if (instance()->on_init_dialog.is_empty())
					return (INT_PTR)TRUE;
				return instance()->on_init_dialog()?(INT_PTR)TRUE:(INT_PTR)FALSE;

			case WM_COMMAND:
				if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
				{
					if (!instance()->on_before_exit.is_empty())
					if (!instance()->on_before_exit()) return (INT_PTR)FALSE;
					EndDialog(hDlg, LOWORD(wParam));
					return (INT_PTR)TRUE;
				}
				break;
			}
			return (INT_PTR)FALSE;
		}

		static dialog_proc_class* instance()
		{
			static dialog_proc_class* static_instance = new dialog_proc_class;
			return static_instance;
		}
	};

	void run()
	{
		//if (_m_exe_handle==0) _m_exe_handle = GetCurrentProcess();
		DialogBox(
			_m_exe_handle,
			MAKEINTRESOURCE(__id),
			_m_hparent,
			&dialog_proc_class::dialog_proc);
	}

	on_init_func& init_dialog_event()
	{
		return dialog_proc_class::instance()->on_init_dialog;
	}
	on_before_exit_func& before_exit_event()
	{
		return dialog_proc_class::instance()->on_before_exit;
	}

	//size_t get_item_int() const
	//{
	//	GetDlgItemInt(
	//}

	HINSTANCE _m_exe_handle;
	HWND _m_hparent;
	HWND _m_hdlg;
};


___namespace2_end()