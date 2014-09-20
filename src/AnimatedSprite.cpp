#include "AnimatedSprite.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

cAnimatedSprite::cAnimatedSprite()
{

}
void cAnimatedSprite::InitAnimation(cGraphics *graphics, const std::string &fileName, int frameNumbersX, int frameNumbersY, float speed)
{
	m_Graphics = graphics;
	m_FrameNumbersX = frameNumbersX;
	m_FrameNumbersY = frameNumbersY;
	m_CurrFrameX = 0;
	m_CurrFrameY = 0;
	m_TotalElapsed = 0;
	m_Speed = speed;
	m_isPaused = false;
	m_Pos = XMFLOAT2(0, 0);

	std::wstring widestr = std::wstring(fileName.begin(), fileName.end());

	HRESULT hr = CreateWICTextureFromFile(m_Graphics->getDevice(), m_Graphics->getContext(), widestr.c_str(), NULL, &m_SpriteTexture, NULL);
	if (FAILED(hr))
	{
		throw(cGameException(gameErrorNS::FATAL_ERROR, "Failed to initialize Sprite Texture."));
	}
		

	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	m_SpriteTexture->GetResource(resource.GetAddressOf());
	D3D11_RESOURCE_DIMENSION dim;
	resource->GetType(&dim);

	if (dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		throw(cGameException(gameErrorNS::FATAL_ERROR, "Expects Texture2D."));

	ComPtr<ID3D11Texture2D> tex2d;
	resource.As(&tex2d);
	D3D11_TEXTURE2D_DESC desc;
	tex2d->GetDesc(&desc);
	m_TextureWidth = int(desc.Width);
	m_TextureHeight = int(desc.Height);
	m_FrameWidth = m_TextureWidth / m_FrameNumbersX;
	m_FrameHeight = m_TextureHeight / m_FrameNumbersY;

}

void cAnimatedSprite::DrawAnimation()
{
	if (m_isPaused)
		return;
	RECT sourceRect;
	sourceRect.left = m_CurrFrameX*m_FrameWidth;
	sourceRect.top = m_CurrFrameY*m_FrameHeight;
	sourceRect.right = sourceRect.left + m_FrameWidth;
	sourceRect.bottom = sourceRect.top + m_FrameHeight;

	m_Graphics->getSpriteBatch()->Begin();
	m_Graphics->getSpriteBatch()->Draw(m_SpriteTexture, m_Pos, &sourceRect, Colors::White);
	m_Graphics->getSpriteBatch()->End();
}
 
void cAnimatedSprite::Update(float deltaT)
{
	if (m_isPaused)
		return;
	
	m_TotalElapsed += deltaT;
	if (m_TotalElapsed > m_Speed)
	{
		m_TotalElapsed -= m_Speed;
		m_CurrFrameX++;

		if (m_CurrFrameX >= m_FrameNumbersX)
		{
			m_CurrFrameX = 0;
			m_CurrFrameY++;

			if (m_CurrFrameY >= m_FrameNumbersY)
			{
				m_CurrFrameY = 0;
			}
		}
	}

}

void cAnimatedSprite::Release()
{
	ReleaseCOM(m_SpriteTexture);
}

void cAnimatedSprite::SetPos(XMFLOAT2 &pos)
{
	m_Pos = pos;
}

void cAnimatedSprite::Resume()
{
	m_isPaused = false;
	m_TotalElapsed = 0.f;
	m_CurrFrameX = 0; 
	m_CurrFrameY = 0;
}

void cAnimatedSprite::Pause()
{
	m_isPaused = true;
}

