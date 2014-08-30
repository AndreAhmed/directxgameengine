#include "GameApp.h"

using namespace std;
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

// Render Update
void GameApp::Game_Render()
{
	m_Graphics.Clear();

	m_Grid.DrawGrid();

	Graphics_2D();

	m_Graphics.Render();
}

// all your 2D drawings call are called here
void GameApp::Graphics_2D()
{
	// 2D Rendering; Disable Z-Buffer, Stencil,..etc
	m_RenderStateHelper->SaveAll();
	m_Animated.DrawAnimation();

	int fps = m_Timer.GetFps();
	m_Graphics.getSpriteBatch()->Begin();

	ostringstream ss;
	ss << fps;
	string fpsString = "FPS " + ss.str();
	wstring widestrFPS = wstring(fpsString.begin(), fpsString.end());

	m_SpriteFont->DrawString(m_Graphics.getSpriteBatch().get(), L"Mixing 2D/3D Graphics Demo", XMFLOAT2(10, 10), Colors::White);
	m_SpriteFont->DrawString(m_Graphics.getSpriteBatch().get(), widestrFPS.c_str(), XMFLOAT2(10, 30), Colors::Red);
	m_Graphics.getSpriteBatch()->End();

	m_RenderStateHelper->RestoreAll();
}

// Game Loop Update
void GameApp::Game_Update()
{
	m_Timer.UpdateFPS();
	m_Timer.UpdateTimer();
	float dt = m_Timer.GetTime();

	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi)*cosf(mTheta);
	float z = mRadius * sinf(mPhi)*sinf(mTheta);
	float y = mRadius * cosf(mPhi);
	m_Graphics.LookAt(x, y, z);

	m_Animated.SetPos(XMFLOAT2(240, 250));
	m_Animated.Update(dt);
}

void GameApp::Game_CleanUp()
{
	m_Graphics.Release();
	m_Grid.Release(); 
	m_Animated.Release();
	
}

void GameApp::Game_Init(HWND handle)
{

	HRESULT result = m_Graphics.Initialize(handle, true);
	m_Graphics.LookAt(0, 0, -5.0f);
	m_Graphics.SetPerspective(0.25f*DirectX::XM_PI, 0.01f, 100.0f);
	//HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);

	m_Graphics.CreateSpriteBatch();

	m_Grid.InitGrid(&m_Graphics);
	m_Grid.CreateGrid(160.0f, 160.0f, 20, 20);
	m_Grid.CompileFX();
 
	m_SpriteFont = std::shared_ptr<SpriteFont>(new SpriteFont(m_Graphics.getDevice(), L"Arial_14_Regular.spritefont"));

	m_RenderStateHelper = new RenderStateHelper(&m_Graphics);

	m_Animated.InitAnimation(&m_Graphics, "wraithb.png", 8, 4, 0.9f);
	
	m_Timer.Initialize();
}
