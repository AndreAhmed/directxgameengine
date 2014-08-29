#include "AnimatedSprite.h"



using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;
cAnimatedSprite::cAnimatedSprite()
{

}
void cAnimatedSprite::InitAnimation(cGraphics *graphics, const std::string &fileName, int frameNumbersX, int frameNumbersY)
{
	m_Graphics = graphics;
	m_FrameNumbersX = frameNumbersX;
	m_FrameNumbersY = frameNumbersY;
	std::wstring widestr = std::wstring(fileName.begin(), fileName.end());

	HRESULT hr = CreateWICTextureFromFile(m_Graphics->getDevice(), m_Graphics->getContext(), widestr.c_str(), NULL, &m_SpriteTexture, NULL);
	if (FAILED(hr))
		return;
	m_SpriteBatch = std::shared_ptr<SpriteBatch>(new SpriteBatch(m_Graphics->getContext()));

	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	m_SpriteTexture->GetResource(resource.GetAddressOf());
	D3D11_RESOURCE_DIMENSION dim;
	resource->GetType(&dim);

	if (dim != D3D11_RESOURCE_DIMENSION_TEXTURE2D)
		throw std::exception("Expects a Texture2D");

	ComPtr<ID3D11Texture2D> tex2d;
	resource.As(&tex2d);
	D3D11_TEXTURE2D_DESC desc;
	tex2d->GetDesc(&desc);
	m_TextureWidth = int(desc.Width);
	m_TextureHeight = int(desc.Height);
	m_CurrFrame = 0;
	m_TotalElapsed = 0;

}

void cAnimatedSprite::DrawAnimation()
{
	m_SpriteBatch->Begin();
	XMFLOAT2 pos(50, 200);
	RECT rect;
	rect.top = 0;
	rect.right = 177;
	rect.left = 0;
	rect.bottom = 177;
	m_SpriteBatch->Draw(m_SpriteTexture, pos, &rect);
	m_SpriteBatch->End();
}

void cAnimatedSprite::DrawFrame(float t)
{
	int frameWidth = m_TextureWidth / 8;
	int frameHeight = m_TextureHeight / 3;
	m_TotalElapsed += 0.001f;

	if (m_TotalElapsed > 1 / (float)2)
	{
		m_CurrFrame++;
		m_CurrFrame = m_CurrFrame % 8;
		m_TotalElapsed -= 1 / (float)2;
	}
	RECT sourceRect;
	sourceRect.left = frameWidth * m_CurrFrame;
	sourceRect.top = 0;
	sourceRect.right = sourceRect.left + frameWidth;
	sourceRect.bottom = m_TextureHeight;
	m_SpriteBatch->Begin();
	XMFLOAT2 pos(50, 200);

	m_SpriteBatch->Draw(m_SpriteTexture, pos, &sourceRect, Colors::White);
	m_SpriteBatch->End();
	 
}

void cAnimatedSprite::Update(float deltaT)
{

}

