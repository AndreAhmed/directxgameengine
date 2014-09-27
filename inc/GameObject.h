#pragma  once
#include "Globals.h"

class cGraphics;

class cGameObject : public RTTI
{
	RTTI_DECLARATIONS(cGameObject, RTTI);
public:
	cGameObject();
	cGameObject(cGraphics *graphics);
	virtual ~cGameObject();
	
	void SetGraphics(cGraphics *graphics);
	void SetEnable(bool enabled);
	bool Enabled() const;
	
	virtual void Initialize();
	virtual void Update(float dt);
	protected:
	cGraphics *m_pGraphics;
	bool	   m_Enabled;
};
