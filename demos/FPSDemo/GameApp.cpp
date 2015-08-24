#include "GameApp.h"
#include "CommonStates.h"
using namespace std;
using namespace DirectX;

GameApp::GameApp()
{
	m_CubeAngle = 0;
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

 
	CommonStates states(m_Graphics.getDevice());
  
	XMMATRIX cameraInverse = XMMatrixInverse(nullptr, m_Graphics.getViewMatrix());
	XMMATRIX translate = XMMatrixTranslation( 2.0f, -3.0f, 2.0f);
 	XMMATRIX rotation = XMMatrixRotationRollPitchYaw((float)XM_PI/9.0f, (float)XM_PI/0.2f, (float)XM_PI/0.1f);
	XMMATRIX world = rotation *translate *cameraInverse;

	m_Tiny->Draw(m_Graphics.getContext(), states, world, m_Graphics.getViewMatrix(), m_Graphics.getProjectionMatrix());

	m_Graphics.getContext()->RSSetState(states.CullCounterClockwise());
	m_Grid.DrawGrid();

	Graphics_2D();


	m_Graphics.Render();
}
 

// all your 2D drawings call are called here
void GameApp::Graphics_2D()
{
	// 2D Rendering; Disable Z-Buffer, Stencil,..etc
	m_RenderStateHelper->SaveAll();

	m_Graphics.getSpriteBatch()->Begin();
 
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
 
}

void GameApp::Game_CleanUp()
{
	m_Graphics.Release();
	m_Grid.Release(); 
 	TwTerminate();
}
 
void GameApp::Game_Init(HWND handle)
{

	m_Graphics.Initialize(handle, true);
	
	m_Camera.Initialize();
	m_Camera.SetPerspective(0.25f*DirectX::XM_PI, m_Graphics.AspectRatio(), 0.01f, 1000.0f);
	m_Graphics.SetCamera(&m_Camera);
	m_Camera.LookAt(XMVectorSet(0, 0, 1, 0));
	m_Camera.SetPosition(-140.0f, -58.0f, -8.0f);
	m_Graphics.SetCamera(&m_Camera);
	//HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);

	m_Graphics.CreateSpriteBatch();

	m_Grid.InitGrid(&m_Graphics);
	m_Grid.CompileFX();
	m_Grid.CreateGrid(360.0f, 360.0f, 50, 50);
	m_Grid.SetPosition(1, -90, 1);

 
	m_SpriteFont = std::shared_ptr<SpriteFont>(new SpriteFont(m_Graphics.getDevice(), L"Arial_14_Regular.spritefont"));
	m_RenderStateHelper = new RenderStateHelper(&m_Graphics);
	m_Timer.Initialize();

	EffectFactory fx(m_Graphics.getDevice());
	m_Tiny = Model::CreateFromSDKMESH(m_Graphics.getDevice(), L"plasma.sdkmesh", fx);
 

}

void GameApp::Draw_Info()
{

	TwInit(TW_DIRECT3D11, m_Graphics.getDevice());
	TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	TwBar *bar = TwNewBar("Mixing 2D and 3D");
	TwDefine("Mixing 2D/3D Graphics Demo, Keys WASD, and Mouse LB (FPS Camera) ");
	// Add rotation and scale vars to bar
	TwAddVarRW(bar, "Rotation", TW_TYPE_FLOAT, &m_CubeAngle, "Rotates the cube ");
	

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
 