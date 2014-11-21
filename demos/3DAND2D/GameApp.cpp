#include "GameApp.h"

using namespace std;
using namespace DirectX;

GameApp::GameApp()
{
	m_Angle = 0;
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
	 
	Graphics_2D();

	m_Graphics.Render();
}
  
// all your 2D drawings call are called here
void GameApp::Graphics_2D()
{
	// 2D Rendering; Disable Z-Buffer, Stencil,..etc
	m_RenderStateHelper->SaveAll();

	m_Graphics.getSpriteBatch()->Begin();
	
	m_Animated->Draw();
	Draw_Info();

	TwDraw();

	m_Graphics.getSpriteBatch()->End();

	m_RenderStateHelper->RestoreAll();
}

// Game Loop Update
void GameApp::Game_Update()
{
 	float dt = m_Timer.GetTime();
    m_Camera.Update(dt);
	m_Angle = (int)m_Angle%360;
 	m_Animated->SetAngle(m_Angle);
	m_Animated->Update(dt);
}

void GameApp::Game_CleanUp()
{
	m_Graphics.Release();
	m_Grid.Release(); 
	m_Animated->Release();
	TwTerminate();

}
 
void GameApp::Game_Init(HWND handle)
{

	m_Graphics.Initialize(handle, true);
	m_Camera.Initialize();
	m_Camera.SetPerspective(0.25f*DirectX::XM_PI, m_Graphics.AspectRatio(), 0.01f, 1000.0f);
	m_Camera.SetPosition(XMFLOAT3(-140.0f, 18.0f, -8.0f));
	m_Camera.RotateY(-30.0f);
	m_Graphics.SetCamera(&m_Camera);
	//HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);

	m_Graphics.CreateSpriteBatch();

	m_Grid.InitGrid(&m_Graphics);
	m_Grid.CompileFX();
	m_Grid.CreateGrid(360.0f, 360.0f, 50, 50);
	m_Grid.SetPosition(1, -90, 1);
	
 
	m_SpriteFont = std::shared_ptr<SpriteFont>(new SpriteFont(m_Graphics.getDevice(), L"Arial_14_Regular.spritefont"));

	m_RenderStateHelper = new RenderStateHelper(&m_Graphics);

	m_Animated = std::shared_ptr<cAnimatedSprite>(new cAnimatedSprite(&m_Graphics, "bird.png", 2, 1, 0.3f, true));
	m_Animated->SetPos(XMFLOAT2(240, 250));
 
	TwInit(TW_DIRECT3D11, m_Graphics.getDevice());
	TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	TwBar *bar = TwNewBar("Mixing 2D and 3D");
	TwDefine("Mixing 2D/3D Graphics Demo, Keys WASD, and Mouse LB (FPS Camera) ");  
	// Add rotation and scale vars to bar
	TwAddVarRW(bar, "Rotation Degrees", TW_TYPE_INT16, &m_Angle, "Rotates Sprite ");
	m_Timer.Initialize();
}

void GameApp::Draw_Info()
{
	int fps = m_Timer.GetFps();
	ostringstream ss;
	ss << fps;
	string fpsString = "FPS " + ss.str();
	wstring widestrFPS = wstring(fpsString.begin(), fpsString.end());
 	m_SpriteFont->DrawString(m_Graphics.getSpriteBatch().get(), widestrFPS.c_str(), XMFLOAT2(WINDOW_WIDTH-100, 30), Colors::Red);
}

void GameApp::UpdateTimers()
{
	m_Timer.UpdateFPS();
	m_Timer.UpdateTimer();
}
