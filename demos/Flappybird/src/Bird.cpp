#include "Bird.h"

using namespace DirectX;
RTTI_DEFINITIONS(cBird)

cBird::cBird(cGraphics *graphics, const std::string fileName, int frameNumbersX, int frameNumbersY, float speed, bool loopable) :
cAnimatedSprite(graphics, fileName, frameNumbersX, frameNumbersY, speed, loopable)
{
	Initialize();
}

cBird::~cBird()
{

}

void cBird::Initialize()
{
	m_vSpeed = 40;
	m_fallConst = 500;
	m_Jumpspeed = 150;
	m_isJumping = false;
	m_Vel = XMFLOAT2(0, m_vSpeed);
	cAnimatedSprite::Initialize();
}

void cBird::Draw()
{

	cAnimatedSprite::Draw();
}

void cBird::Update(float dt)
{
	

	XMVECTOR Pos = XMLoadFloat2(&m_Pos);
	XMVECTOR Vel = XMLoadFloat2(&m_Vel);
  
	if (GetAsyncKeyState('Z') & 0x8000)
	{
		m_isJumping = true;
	}

	if(m_isJumping)
	{
		m_isJumping = false;
		m_vSpeed = m_Jumpspeed;
	}
	Pos -= Vel*dt;
	m_vSpeed -= m_fallConst*dt;
	m_Vel.y = m_vSpeed;
	XMStoreFloat2(&m_Pos, Pos);

	cAnimatedSprite::Update(dt);
}

