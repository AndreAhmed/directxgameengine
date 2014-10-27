#include "Obstacle.h"
#include "WICTextureLoader.h"
#include <Math.h>

#define RAND_RANGE(a,b) ((a)+(rand()%((b)-(a)+1)))

using namespace DirectX;
using namespace SimpleMath;

void cObstacle::Initialize()
{
	m_Pos.y =  RAND_RANGE(-200, 5);
	m_isScored = false;
}

cObstacle::cObstacle(cGraphics *graphics, const wchar_t *fileName, Vector2 pos)
{
	m_pGraphics = graphics;
	m_Pos = pos;


	HRESULT hr = DirectX::CreateWICTextureFromFile(graphics->getDevice(), fileName, nullptr, &m_Obstacle);
	if (FAILED(hr))
	{
		throw(cGameException(gameErrorNS::FATAL_ERROR, "Couldn't load  Obstacle texture."));
	}

	Initialize();
}
 
cObstacle::~cObstacle()
{
	m_pGraphics = nullptr;
	m_Obstacle = nullptr;
}

void cObstacle::Draw()
{

	m_pGraphics->getSpriteBatch()->Draw(m_Obstacle.Get(), m_Pos, nullptr, Colors::White, 0.f, XMFLOAT2(0, 0), 1.0f,
		SpriteEffects_None);
 
}

void cObstacle::Update(float dt)
{
	m_Pos.x -= 80.0f * dt;

	m_topRect.left = m_Pos.x;
	m_topRect.top = m_Pos.y;
	m_topRect.right = 55 + m_topRect.left;
	m_topRect.bottom = 308 + m_topRect.top;

	m_bottomRect.left = m_Pos.x;
	m_bottomRect.top = m_Pos.y + 460;
	m_bottomRect.right = 55 + m_bottomRect.left;
	m_bottomRect.bottom = 340 + m_bottomRect.top;
	

}

RECT cObstacle::GetTopRect()
{
	return m_topRect;
}

RECT cObstacle::GetBottomRect()
{
	return m_bottomRect;
}

