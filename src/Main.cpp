#include "Globals.h"


void OnMouseDown(WPARAM btnState, int x, int y);
void OnMouseUp(WPARAM btnState, int x, int y);
void OnMouseMove(WPARAM btnState, int x, int y);

// GLOBALS ////////////////////////////////////////////////
HWND      main_window_handle = NULL; // globally track main window
HINSTANCE hinstance_app = NULL; // globally track hinstance
// DEFINES ////////////////////////////////////////////////
// defines for windows 
#define WINDOW_CLASS_NAME "2D/3D Game Engine Course; Ahmed Saleh "
#define WINDOW_WIDTH			800 
#define WINDOW_HEIGHT			600

using namespace DirectX;
cGraphics g_Graphics;
cGrid     g_Grid;

float mPhi = 0.1f*XM_PI;
float mTheta = 1.5f*XM_PI;
float mRadius = 200;
POINT mLastMousePos;

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

	case WM_MOUSEMOVE:
	{
						 OnMouseMove(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
						 break;
	}
	case WM_RBUTTONUP:
	{
						 OnMouseUp(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
						 break;
	}
	case WM_RBUTTONDOWN:
	{
						   OnMouseDown(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
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


void OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(main_window_handle);
}

void OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

float Clamp(float x, float low, float high)
{
	return x < low ? low : (x > high ? high : x);
}
void OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi =  Clamp(mPhi, 0.1f, XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		float dx = 0.2f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.2f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius =  Clamp(mRadius, 50.0f, 500.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}


int Game_Main(void *parms = NULL, int num_parms = 0)
{
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;


	g_Graphics.Clear();

	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi)*cosf(mTheta);
	float z = mRadius * sinf(mPhi)*sinf(mTheta);
	float y = mRadius * cosf(mPhi);
	g_Graphics.LookAt(x, y, z);

	g_Grid.DrawGrid();

	g_Graphics.Render();

	// return success or failure or your own return code here
	return(1);

} // end Game_Main

////////////////////////////////////////////////////////////

int Game_Init(void *parms = NULL, int num_parms = 0)
{
	HRESULT result = g_Graphics.Initialize(main_window_handle, true);
	g_Graphics.LookAt(0, 0, -5.0f);
	g_Graphics.SetPerspective(0.25f*DirectX::XM_PI, 0.01f, 100.0f);
	

	g_Grid.InitGrid(&g_Graphics);
	g_Grid.CreateGrid(160.0f, 160.0f, 20, 20);
	g_Grid.CompileFX();

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




