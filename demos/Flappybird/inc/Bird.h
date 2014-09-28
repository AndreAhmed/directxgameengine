#include "AnimatedSprite.h"
#include "RTTI.h"

class cBird : public cAnimatedSprite
{
	RTTI_DECLARATIONS(cBird, cAnimatedSprite)

public:
	cBird(cGraphics *graphics, const std::string fileName, int frameNumbersX, int frameNumbersY, float speed, bool loopable);
	virtual ~cBird();
	virtual void Initialize() override;
	virtual void Draw() override;
	virtual void Update(float dt) override;
private:

	DirectX::XMFLOAT2 m_Vel; 

};