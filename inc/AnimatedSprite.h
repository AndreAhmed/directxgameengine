#pragma  once

#include "Globals.h"
#include "DrawableGameObject.h"
#include "SpriteBatch.h"
#include "WICTextureLoader.h"
#include "Graphics.h"

class cAnimatedSprite : cDrawableGameObject
{
	 
public:
	cAnimatedSprite(cGraphics *graphics, const std::string fileName, int frameNumbersX, int frameNumbersY, float speed);
	virtual void Initialize() override;
	virtual void Draw() override;
	virtual void Update(float deltaT) override;
	
	void InitAnimation();
	void SetPos(DirectX::XMFLOAT2 &pos);
	DirectX::XMFLOAT2 Position() const;
	void SetAngle(float angle);
	float Angle() const;
	void Release();
	void Pause();
	void Resume();
private:
	std::shared_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	ID3D11ShaderResourceView* m_SpriteTexture;
	std::string m_FileName;

	DirectX::XMFLOAT2  m_Pos;
	float			   m_Angle;
	bool	  m_isPaused;
	int m_TextureWidth;
	int m_TextureHeight;
	int m_FrameNumbersX;
	int m_FrameNumbersY;
	int m_CurrFrameX;
	int m_CurrFrameY;
	int m_FrameWidth;
	int m_FrameHeight;
	float m_TotalElapsed;
	float m_Speed;
};