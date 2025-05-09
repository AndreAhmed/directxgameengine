#pragma once
#include "Graphics.h"
#include "SpriteBatch.h"
#include "AnimatedSprite.h"
#include "RenderStateHelper.h"
#include "SpriteFont.h"
#include "Timer.h"
#include "Grid.h"
#include "Camera.h"
#include "Bird.h"
#include "Obstacle.h"
#include <list>


#define WINDOW_WIDTH			320	 
#define WINDOW_HEIGHT			480

class GameApp
{
public:
	GameApp();
	~GameApp();
	void Game_Init(HWND handle);
	void Game_Render();
	void Game_Update();
	void Game_CleanUp();
	void Graphics_2D();
	void Draw_Info();
	void UpdateTimers();
	void ResetGame();
	void OnMouseDown(WPARAM btnState, int x, int y, HWND handle);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	float Clamp(float x, float low, float high);
	
public:
	cGraphics m_Graphics;
 	cTimer	  m_Timer;
	cCamera   m_Camera;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_BackGround;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_DebugRect;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_DebugRectBird;
	std::shared_ptr<cBird> m_FlappyBird;
	std::vector<std::shared_ptr<cObstacle>> m_Obstacle;
	std::shared_ptr<DirectX::SpriteFont> m_SpriteFont;
	RenderStateHelper* m_RenderStateHelper;
 	POINT mLastMousePos;
	float m_obstacleTimer;
	int   m_Score;
};