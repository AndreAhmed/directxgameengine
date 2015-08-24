#include "GameApp.h"

// GLOBALS ////////////////////////////////////////////////
HWND      main_window_handle = NULL; // globally track main window
HINSTANCE hinstance_app = NULL; // globally track hinstance
// DEFINES ////////////////////////////////////////////////
// defines for windows 
#define WINDOW_CLASS_NAME "2D/3D Game Engine Course; Ahmed Saleh "
#define WINDOW_WIDTH			800 
#define WINDOW_HEIGHT			600

using namespace DirectX;

GameApp app; 

// FUNCTIONS //////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	// this is the main message handler of the system
	if (TwEventWin(hwnd, msg, wparam, lparam)) // send event message to AntTweakBar
		return 0; // event has been handled by AntTweakBar
	// what is the message 
	switch (msg)
	{

	case WM_MOUSEMOVE:
	{
	    app.OnMouseMove(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		break;
	}
	case WM_RBUTTONUP:
	{
		app.OnMouseUp(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		app.OnMouseDown(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), main_window_handle);
		break;
	}
	case WM_DESTROY:
	{
	// kill the application, this sends a WM_QUIT message 
	   PostQuitMessage(0);
    // return success
	   return(0);
	} break;

	default:break;

	} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

///////////////////////////////////////////////////////////
 
 
// WINMAIN ////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hprevinstance,
	LPSTR lpcmdline,
	int ncmdshow)
{

	WNDCLASSEX winclass; // this will hold the class we create
	HWND	   hwnd;	 // generic window handle
	MSG		   msg;		 // generic message

	// first fill in the window class stucture
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC |
		CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = L"DirectX Engine";
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// save hinstance in global
	hinstance_app = hinstance;

	// register the window class
	if (!RegisterClassEx(&winclass))
		return(0);

	// create the window
	if (!(hwnd = CreateWindowEx(NULL,                  // extended style
		L"DirectX Engine",     // class
		L"2D/3D Game Engine- A.Tolba ", // title
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		120, 100,	  // initial x,y
		WINDOW_WIDTH, WINDOW_HEIGHT,  // initial width, height
		NULL,	  // handle to parent 
		NULL,	  // handle to menu
		hinstance,// instance of this application
		NULL)))	// extra creation parms
		return(0);


	// save main window handle
	main_window_handle = hwnd;

	ShowWindow(main_window_handle, SW_NORMAL);

	UpdateWindow(main_window_handle);

	// initialize game here
	app.Game_Init(main_window_handle);

	// enter main event loop
	while (TRUE)
	{
		// test if there is a message in queue, if so get it
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if

		app.UpdateTimers();
		// main game processing goes here
		app.Game_Update();
		app.Game_Render();
	
	} // end while

	// closedown game here
	app.Game_CleanUp();

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

///////////////////////////////////////////////////////////




