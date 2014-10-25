#include "GameApp.h"
#include "DDSTextureLoader.h"
#include <math.h>
#include <ctime>

using namespace std;
using namespace DirectX;
using namespace SimpleMath;

GameApp::GameApp()
{
	m_obstacleTimer = 0;
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
	XMMATRIX m = m_Graphics.getViewMatrix();
	m_Graphics.getSpriteBatch()->Begin(SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, m);

	m_Graphics.getSpriteBatch()->Draw(m_BackGround.Get(), XMFLOAT2(0, 0), nullptr, Colors::White, 0.f, XMFLOAT2(0, 0), 1.0f,
		SpriteEffects_None);
	
	for (list<std::shared_ptr<cObstacle> >::const_iterator iter = m_Obstacle.begin(); iter != m_Obstacle.end(); ++iter)
	{
		cObstacle *obstacle = (*iter).get();

		if (obstacle != NULL)
		{
			obstacle->Draw();
		}
	}

	m_FlappyBird->Draw();



	Draw_Info();
	m_Graphics.getSpriteBatch()->End();

	
	m_RenderStateHelper->RestoreAll();
}

// Game Loop Update
void GameApp::Game_Update()
{
	float dt = m_Timer.GetTime();

	m_Camera.Update(dt);
	m_FlappyBird->Update(dt);

	m_obstacleTimer += dt;

	if (m_obstacleTimer > 1.1)
	{
		m_Obstacle.push_back(std::shared_ptr<cObstacle>(new cObstacle(&m_Graphics, L"bar.png", Vector2(WINDOW_WIDTH - 10, 0))));
		m_obstacleTimer = 0;
	}
	
	for (list<std::shared_ptr<cObstacle> >::const_iterator iter = m_Obstacle.begin(); iter != m_Obstacle.end();)
	{
		cObstacle *obstacle = (*iter).get();
		
		if (obstacle != NULL)
		{
			if(obstacle->m_Pos.x < 0 )
			{
				iter = m_Obstacle.erase(iter);
			}
			else
			{
				obstacle->Update(dt);
				++iter;
			}
		}
	}

 
	 
}

void GameApp::Game_CleanUp()
{
 	m_FlappyBird->Release();
	m_RenderStateHelper->Release();
	m_BackGround = nullptr;
	m_SpriteFont = nullptr;
}

void GameApp::Game_Init(HWND handle)
{

	m_Graphics.Initialize(handle, true);
	m_Camera.Initialize();
	//m_Camera.SetPerspective(0.25f*DirectX::XM_PI, m_Graphics.AspectRatio(), 0.01f, 1000.0f);
	m_Camera.SetOrthographic(WINDOW_WIDTH, WINDOW_HEIGHT,0.1f, 100.0f);
	m_Camera.SetPosition(XMFLOAT3(1.0f, 0, 0));
	m_Graphics.SetCamera(&m_Camera);
	//HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);
	m_Graphics.CreateSpriteBatch();
 
	m_SpriteFont = std::shared_ptr<SpriteFont>(new SpriteFont(m_Graphics.getDevice(), L"Arial_14_Regular.spritefont"));
	m_RenderStateHelper = new RenderStateHelper(&m_Graphics);

	m_FlappyBird = std::shared_ptr<cBird>(new cBird(&m_Graphics, "bird.png", 2, 1, 0.3f, true));
	m_FlappyBird->SetPos(Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

	m_Obstacle.push_back(std::shared_ptr<cObstacle>(new cObstacle(&m_Graphics, L"bar.png", Vector2(WINDOW_WIDTH - 70, 0))));

	HRESULT hr = DirectX::CreateWICTextureFromFile(m_Graphics.getDevice(), L"background.png", nullptr, &m_BackGround);
	if (FAILED(hr))
	{
		throw(cGameException(gameErrorNS::FATAL_ERROR, "Couldn't load Background texture."));
	}
	m_Timer.Initialize();

	srand(time(NULL));

 
}

void GameApp::Draw_Info()
{
	int fps = m_Timer.GetFps();
	ostringstream ss;
	ss << fps;
	string fpsString = "FPS " + ss.str();
	wstring widestrFPS = wstring(fpsString.begin(), fpsString.end());

	ostringstream angleStream;
	float angle = m_FlappyBird->Angle();
	 
	angleStream << angle;
	string angleString = "Bird Angle " + angleStream.str();
	wstring angleWideStr = wstring(angleString.begin(), angleString.end());
	
//	m_SpriteFont->DrawString(m_Graphics.getSpriteBatch().get(), L"Flappy bird clone", XMFLOAT2(10, 10), Colors::White);
 	m_SpriteFont->DrawString(m_Graphics.getSpriteBatch().get(), widestrFPS.c_str(), XMFLOAT2(0, 40), Colors::Red);
//	m_SpriteFont->DrawString(m_Graphics.getSpriteBatch().get(), angleWideStr.c_str(), XMFLOAT2(10, 70), Colors::White);
}

void GameApp::UpdateTimers()
{
	m_Timer.UpdateFPS();
	m_Timer.UpdateTimer();
}
