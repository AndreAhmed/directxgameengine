#include "Obstacle.h"
#include "WICTextureLoader.h"
#include <Math.h>

#define RAND_RANGE(a,b) ((a)+(rand()%((b)-(a)+1)))

using namespace DirectX;
using namespace SimpleMath;

void cObstacle::Initialize()
{
	m_Pos.y = RAND_RANGE(-200, 5);
 
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

	m_topRect.left   =  m_Pos.x;
	m_topRect.top    =  m_Pos.y;
	m_topRect.right = 53;
	m_topRect.bottom = 307;
	 
}

