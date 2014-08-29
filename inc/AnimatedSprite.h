#pragma  once

#include "Globals.h"
#include "Graphics.h"
#include "SpriteBatch.h"
#include "WICTextureLoader.h"

class cAnimatedSprite
{
	 
public:
	cAnimatedSprite();
	void InitAnimation(cGraphics *graphics, const std::string &fileName, int frameNumbersX, int frameNumbersY);
	void DrawAnimation();
	void Update(float deltaT);
	void DrawFrame(float t);
private:
	std::shared_ptr<DirectX::SpriteBatch> m_SpriteBatch;
	ID3D11ShaderResourceView* m_SpriteTexture;
	cGraphics *m_Graphics;
	int m_TextureWidth;
	int m_TextureHeight;
	int m_FrameNumbersX;
	int m_FrameNumbersY;
	int m_CurrFrame;
	float m_TotalElapsed;
};