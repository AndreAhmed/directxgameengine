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
	void OnMouseDown(WPARAM btnState, int x, int y, HWND handle);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	float Clamp(float x, float low, float high);
public:
	cGraphics m_Graphics;
 	cTimer	  m_Timer;
	cCamera   m_Camera;
	std::shared_ptr<cBird> m_FlappyBird;
	std::shared_ptr<DirectX::SpriteFont> m_SpriteFont;
	RenderStateHelper* m_RenderStateHelper;
 	POINT mLastMousePos;
};