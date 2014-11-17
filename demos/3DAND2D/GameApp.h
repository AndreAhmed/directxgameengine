#pragma once
#include "Graphics.h"
#include "SpriteBatch.h"
#include "AnimatedSprite.h"
#include "RenderStateHelper.h"
#include "SpriteFont.h"
#include "Timer.h"
#include "Grid.h"
#include "Camera.h"
#include "GeometricPrimitive.h"
#include <DirectXCollision.h>
#include "PrimitiveBatch.h"
#include "VertexTypes.h"
#include "Effects.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"
#include "btBulletDynamicsCommon.h"

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
	void UpdateTimers();
	void Draw_Info();
	void OnMouseDown(WPARAM btnState, int x, int y, HWND handle);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void DrawAabb(const DirectX::BoundingBox& box, DirectX::FXMVECTOR color);
	void DrawCube(DirectX::CXMMATRIX mWorld, DirectX::FXMVECTOR color);
	float Clamp(float x, float low, float high);
	DirectX::XMMATRIX btTransform_to_XMMATRIX(btTransform const & trans);
public:
	cGraphics m_Graphics;
	cCamera   m_Camera;
	cGrid     m_Grid;
	cTimer	  m_Timer;
	std::shared_ptr<cAnimatedSprite> m_Animated;
	std::shared_ptr<DirectX::SpriteFont> m_SpriteFont;

	//DirectX::BoundingBox    m_Box;
	//ID3D11InputLayout*                  g_pBatchInputLayout = nullptr;
 //
	//std::unique_ptr<DirectX::BasicEffect>                            g_BatchEffect;
	//std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>    g_Batch;
	std::unique_ptr<DirectX::GeometricPrimitive> m_Cube;
	RenderStateHelper* m_RenderStateHelper;
 

	// core Bullet components
	btBroadphaseInterface* m_pBroadphase;
	btCollisionConfiguration* m_pCollisionConfiguration;
	btCollisionDispatcher* m_pDispatcher;
	btConstraintSolver* m_pSolver;
	btDynamicsWorld* m_pWorld;
	btDefaultMotionState* m_pBoxMotionState;
	btDefaultMotionState* m_pGroundMotionState;
	POINT mLastMousePos;
	
	
};