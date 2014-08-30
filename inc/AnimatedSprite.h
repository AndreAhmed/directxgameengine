#pragma  once

#include "Globals.h"
#include "Graphics.h"
#include "SpriteBatch.h"
#include "WICTextureLoader.h"

class cAnimatedSprite
{
	 
public:
	cAnimatedSprite();
	void InitAnimation(cGraphics *graphics, const std::string &fileName, int frameNumbersX, int frameNumbersY, float speed);
	void DrawAnimation();
	void Update(float deltaT);
	void SetPos(DirectX::XMFLOAT2 &pos);
	void Release();
	void Pause();
	void Resume();
private:
	std::shared_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	ID3D11ShaderResourceView* m_SpriteTexture;
	cGraphics *m_Graphics;
	DirectX::XMFLOAT2  m_Pos;
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