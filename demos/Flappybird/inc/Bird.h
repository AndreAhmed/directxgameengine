#include "AnimatedSprite.h"
#include "RTTI.h"
#include "SimpleMath.h"

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
	bool m_isJumping;
	bool m_prevJumping;
	float m_fallConst;
	float m_Jumpspeed;
	DirectX::SimpleMath::Vector2 m_Vel;
	DirectX::SimpleMath::Vector2 m_Acc;

};