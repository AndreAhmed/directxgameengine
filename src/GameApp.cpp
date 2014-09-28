#include "..\..\GameApp.h"

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
 
void GameApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		m_Camera.Pitch(dy);
		m_Camera.RotateY(dx);

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
	m_Animated->Draw();

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
	static float angle = 10.0f;
	m_Camera.Update(dt);

	angle += 0.05f*dt;
	//m_Animated->SetAngle(angle);
	m_Animated->Update(dt);
}

void GameApp::Game_CleanUp()
{
	m_Graphics.Release();
	m_Grid.Release(); 
	m_Animated->Release();
	
}

void GameApp::Game_Init(HWND handle)
{

	m_Graphics.Initialize(handle, true);
	m_Camera.Initialize();
	m_Camera.SetPerspective(0.25f*DirectX::XM_PI, m_Graphics.AspectRatio(), 0.01f, 1000.0f);
	m_Camera.SetPosition(XMFLOAT3(0, 150, -56));
	m_Camera.LookAt(XMVectorSet(0, 0, 0, 0));
	m_Graphics.SetCamera(&m_Camera);


	//HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);

	m_Graphics.CreateSpriteBatch();

	m_Grid.InitGrid(&m_Graphics);
	m_Grid.CreateGrid(360.0f, 360.0f, 20, 20);
	m_Grid.CompileFX();
 
	m_SpriteFont = std::shared_ptr<SpriteFont>(new SpriteFont(m_Graphics.getDevice(), L"Arial_14_Regular.spritefont"));

	m_RenderStateHelper = new RenderStateHelper(&m_Graphics);

	m_Animated = std::shared_ptr<cAnimatedSprite>(new cAnimatedSprite(&m_Graphics, "bird.png", 2, 1, 0.3f, true));
	m_Animated->SetPos(XMFLOAT2(240, 250));
	
	m_Timer.Initialize();
}
