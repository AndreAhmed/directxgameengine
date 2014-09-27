#include "DrawableGameObject.h"
RTTI_DEFINITIONS(cDrawableGameObject)

 

cDrawableGameObject::cDrawableGameObject(cGraphics *graphics):cGameObject(graphics), m_Visible(true)
{

}

cDrawableGameObject::~cDrawableGameObject()
{
}

void cDrawableGameObject::Draw()
{

}

void cDrawableGameObject::SetVisible(bool visible)
{
	m_Visible = visible;
}

bool cDrawableGameObject::Visible() const
{
	return m_Visible;
}
