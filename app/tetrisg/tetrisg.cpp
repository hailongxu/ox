// tetrisg.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "tetrisg.h"



#include "../../src/utl/destructor.h"
#include "../../src/thread/win_queue_thread.h"
#include "../../src/app/tetris/win_gui.h"
#include "../../src/app/tetris/tetris_uig.h"
#include "../../src/app/tetris/tetris_uig_event_source.h"
#include "../../src/app/tetris/tetris_app.h"

struct on_application_start
{
	on_application_start(tetris_uig_input_event_source& tetris_event)
		: tetris_event(tetris_event)
	{}
	tetris_uig_input_event_source& tetris_event;
	void operator()() {tetris_event.start();}
};
int entry(int argc, _TCHAR* argv[])
{
	win_gui gui;
	tetris_win_gui tetris_gui(gui);
	tetris_uig_input_event_source tetris_event;
	app<tetris_win_gui,tetris_uig_input_event_source> tetris_application;
	tetris_application.init(&tetris_gui,&tetris_event);
	tetris_application.on_started.assign(&on_application_start(tetris_event));
	tetris_application.start();
	ox::win_queue_thread th;
	th.start_here();
	getchar();
	return 0;
}



win_gui gui;
tetris_win_gui tetris_gui(gui);
tetris_uig_input_event_source tetris_event;
app<tetris_win_gui,tetris_uig_input_event_source> tetris_application;

//ui_board<tetris_win_gui> a(tetris_gui);
//ui_preview<tetris_win_gui> b(tetris_gui);
//ui_information<tetris_win_gui> c(tetris_gui);


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	tetris_application.init(&tetris_gui,&tetris_event);
	tetris_application.on_started.assign(&on_application_start(tetris_event));

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TETRISG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRISG));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRISG));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TETRISG);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

	

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static int i = 0;
	char buff[32];
	RECT text_rect{10,10,50,50};

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
		{
		HDC hdc = GetDC(hWnd);
		gui._m_hdc = hdc;
		int vkey = (int)wParam;
		//tetris_uig_input_event_source event_source;
		//event_source.on_keydown_from_system(vkey);
		//sprintf(buff,"%d,%d",++i,vkey);
		//HDC hdc = GetDC(hWnd);
		//win_gui wgui(hdc);
		//tetris_win_gui tgui(wgui);
		//tgui.draw_text(rc_point_t(0,0),buff,strlen(buff));
		//
		////DrawTextA(hdc,buff,-1,&text_rect,DT_LEFT);
		//ox::utl::win_dc_defer dcdefer(hWnd,hdc);
		tetris_event.on_keydown_from_system(vkey);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		{
			//gui._m_hdc = hdc;
			//tetris_application.draw_board();
		//win_gui wgui(hdc);
			gui._m_hdc = hdc;
			tetris_application.start();
			//tetris_gui.fill_rect(rc_rect_t(0,0,2,4),tetris_define::back_char());
			//tetris_gui.draw_point(rc_point_t(2,4),tetris_define::front_char());
			//tetris_application._m_ui_preview.clear();
		//tetris_win_gui tgui(wgui);
		//tgui.fill_rect(rc_rect_t(0,0,2,4),tetris_define::back_char());
		//tgui.draw_point(rc_point_t(2,4),tetris_define::front_char());
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

