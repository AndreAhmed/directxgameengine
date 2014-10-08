#include "Bird.h"

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


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
	m_Jumpspeed = 340;
	m_isJumping = false;
	m_fallConst = 860;
	m_Vel = XMFLOAT2(0, 0);
	m_Acc = XMFLOAT2(0, m_fallConst);
	cAnimatedSprite::Initialize();
}

void cBird::Draw()
{

	cAnimatedSprite::Draw();
}

void cBird::Update(float dt)
{
 
	bool qDown = GetAsyncKeyState('Z') & 0x8000;  
	if (!qDown &&m_isJumping)
	{
		m_isJumping = false;
	 
	}
	else if (qDown && !m_isJumping)
	{
		m_isJumping = true;
		m_Vel.y = -m_Jumpspeed;
 	}
 
	if (m_Vel.y > 300)
		m_Vel.y = 300;

	m_Vel.y += m_Acc.y*dt;
	m_Pos.y += m_Vel.y*dt;
 
	cAnimatedSprite::Update(dt);
}

