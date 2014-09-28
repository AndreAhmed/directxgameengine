#include "Bird.h"

using namespace DirectX;
RTTI_DEFINITIONS(cBird)

cBird::cBird(cGraphics *graphics, const std::string fileName, int frameNumbersX, int frameNumbersY, float speed, bool loopable) :
cAnimatedSprite(graphics, fileName, frameNumbersX, frameNumbersY, speed, loopable)
{

}

cBird::~cBird()
{

}

void cBird::Initialize()
{
	m_Vel = XMFLOAT2(0, 0);
	cAnimatedSprite::Initialize();
}

void cBird::Draw()
{

	cAnimatedSprite::Draw();
}

void cBird::Update(float dt)
{

	cAnimatedSprite::Update(dt);
}

