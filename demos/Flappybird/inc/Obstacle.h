#include "Graphics.h"
#include "Globals.h"
#include "SimpleMath.h"

class cObstacle
{
public:
	cObstacle(cGraphics *graphics, const wchar_t *fileName, DirectX::SimpleMath::Vector2 pos);
	virtual ~cObstacle();
	void Initialize();
	void Draw();
	void Update(float dt);
public:
	DirectX::SimpleMath::Vector2 m_Pos;
	RECT m_topRect;
	RECT m_bottomRect;
	cGraphics *m_pGraphics;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    m_Obstacle;
};