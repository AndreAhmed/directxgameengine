#include "GameApp.h"

using namespace DirectX;

GameApp::GameApp()
{
}


GameApp::~GameApp()
{
}


void GameApp::OnMouseDown(WPARAM btnState, int x, int y, HWND handle)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(handle);
}

void GameApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

float GameApp::Clamp(float x, float low, float high)
{
	return x < low ? low : (x > high ? high : x);
}
void GameApp::OnMouseMove(WPARAM btnState, int x, int y)
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
		mPhi = Clamp(mPhi, 0.1f, XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		float dx = 0.2f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.2f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = Clamp(mRadius, 50.0f, 500.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}


void GameApp::Game_Render()
{

	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;


	m_Graphics.Clear();

	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi)*cosf(mTheta);
	float z = mRadius * sinf(mPhi)*sinf(mTheta);
	float y = mRadius * cosf(mPhi);
	m_Graphics.LookAt(x, y, z);

	m_Grid.DrawGrid();

	m_Graphics.Render();
}

void GameApp::Game_Update()
{

}

void GameApp::Game_CleanUp()
{
	m_Graphics.Release();
	m_Grid.Release(); 
}

void GameApp::Game_Init(HWND handle)
{

	HRESULT result = m_Graphics.Initialize(handle, true);
	m_Graphics.LookAt(0, 0, -5.0f);
	m_Graphics.SetPerspective(0.25f*DirectX::XM_PI, 0.01f, 100.0f);


	m_Grid.InitGrid(&m_Graphics);
	m_Grid.CreateGrid(160.0f, 160.0f, 20, 20);
	m_Grid.CompileFX();

	HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);
}
