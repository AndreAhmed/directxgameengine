#include "Globals.h"

// FUNCTIONS //////////////////////////////////////////////
LRESULT CALLBACK WindowProc(HWND hwnd,
	UINT msg,
	WPARAM wparam,
	LPARAM lparam)
{
	// this is the main message handler of the system

	// what is the message 
	switch (msg)
	{
	 
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

 
int Game_Main(void *parms = NULL, int num_parms = 0)
{

	// return success or failure or your own return code here
	return(1);

} // end Game_Main

////////////////////////////////////////////////////////////

int Game_Init(void *parms = NULL, int num_parms = 0)
{

	return(1);
} // end Game_Init

/////////////////////////////////////////////////////////////

int Game_Shutdown(void *parms = NULL, int num_parms = 0)
{

	return(1);

} // end Game_Shutdown




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
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// save hinstance in global
	hinstance_app = hinstance;

	// register the window class
	if (!RegisterClassEx(&winclass))
		return(0);
	 
	// create the window
	if (!(hwnd = CreateWindowEx(NULL,                  // extended style
		WINDOW_CLASS_NAME,     // class
		"2D/3D Game Engine- A.Tolba ", // title
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
	Game_Init();

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

		// main game processing goes here
		Game_Main();

	} // end while

	// closedown game here
	Game_Shutdown();

	// return to Windows like this
	return(msg.wParam);

} // end WinMain

///////////////////////////////////////////////////////////




