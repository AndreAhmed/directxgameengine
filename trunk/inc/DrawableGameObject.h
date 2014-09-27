#include "GameObject.h"

class cDrawableGameObject : public cGameObject
{
	RTTI_DECLARATIONS(cDrawableGameObject, cGameObject)

public:
	cDrawableGameObject(cGraphics *graphics);
	virtual ~cDrawableGameObject();

	virtual void Draw();
	void SetVisible(bool visible);
	bool Visible() const;

	protected:
	bool m_Visible; 
};

