#include "Bird.h"
#include <math.h>
 

using namespace DirectX;
using namespace SimpleMath;
RTTI_DEFINITIONS(cBird)

cBird::cBird(cGraphics *graphics, const std::string fileName, int frameNumbersX, int frameNumbersY, float speed, bool loopable) :
cAnimatedSprite(graphics, fileName, frameNumbersX, frameNumbersY, speed, loopable)
{
	Initialize();
 
}

cBird::~cBird()
{
	m_pGraphics = nullptr;
}

void cBird::Initialize()
{
	m_Jumpspeed = 290;
	m_isDead	= false;
	m_isJumping = false;
	m_fallConst = 1260;
	m_Vel = Vector2(0, 0);
	m_Acc = Vector2(0, m_fallConst);
	cAnimatedSprite::Initialize();
}

void cBird::Draw()
{
	cAnimatedSprite::Draw();
}
bool cBird::CollideWith(RECT rect)
{
	RECT intersection;
	if (IntersectRect(&intersection, &m_BoundingRect, &rect) != 0)
		return true;
	else
		return false;
	
}
void cBird::Update(float dt)
{
	
	static float angel = 0;
 
	bool qDown = GetAsyncKeyState('C') & 0x8000;  
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
 
	m_Angle = (float)atan2(m_Vel.y, 300);
	//SetAngle(m_Angle);
	m_Vel.y += m_Acc.y*dt;
	m_Pos.y += m_Vel.y*dt;

	if(m_Pos.y > 480)
	{
		m_isDead = true;
	}
	

	m_BoundingRect.left = m_Pos.x - m_Origin.x;
	m_BoundingRect.top = m_Pos.y - m_Origin.y;
	m_BoundingRect.right = m_FrameWidth + m_BoundingRect.left;
	m_BoundingRect.bottom = m_FrameHeight + m_BoundingRect.top;

	cAnimatedSprite::Update(dt);
	 
}

