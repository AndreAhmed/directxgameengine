#include "GameObject.h"

RTTI_DEFINITIONS(cGameObject)

cGameObject::cGameObject(cGraphics *graphics) :m_pGraphics(graphics)
{
}

cGameObject::cGameObject()
{

}

cGameObject::~cGameObject()
{

}

void cGameObject::SetGraphics(cGraphics *graphics)
{
	if (graphics != NULL)
	{
		m_pGraphics = graphics;
	}
}

void cGameObject::Initialize()
{

}

void cGameObject::Update(float dt)
{

}

bool cGameObject::Enabled() const
{
	return m_Enabled;
}
void cGameObject::SetEnable(bool enabled)
{
	m_Enabled = enabled;
}
