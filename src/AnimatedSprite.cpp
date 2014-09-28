#include "AnimatedSprite.h"

using namespace DirectX;
using namespace std;
using namespace Microsoft::WRL;

cAnimatedSprite::cAnimatedSprite(cGraphics *graphics, const std::string fileName, int frameNumbersX, int frameNumbersY, float speed, bool loopable) :
cDrawableGameObject(graphics), m_FileName(fileName), m_FrameNumbersX(frameNumbersX), m_FrameNumbersY(frameNumbersY), m_Speed(speed), m_isLoopable(loopable)
{
	Initialize();
}
void cAnimatedSprite::Initialize()
{
	InitAnimation();
}
void cAnimatedSprite::InitAnimation()
{
	m_CurrFrameX = 0;
	m_CurrFrameY = 0;
	m_TotalElapsed = 0;
	m_isAnimComplete = false;
	m_isPaused = false;
	m_Pos = XMFLOAT2(0, 0);
	m_Angle = 0;
	std::wstring widestr = std::wstring(m_FileName.begin(), m_FileName.end());

	HRESULT hr = CreateWICTextureFromFile(m_pGraphics->getDevice(), m_pGraphics->getContext(), widestr.c_str(), NULL, &m_SpriteTexture, NULL);
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
	m_Origin.x = (float)m_FrameWidth / 2;
	m_Origin.y = (float)m_FrameHeight / 2;
}
void cAnimatedSprite::Draw()
{
	if (!m_Visible)
		return;
	RECT sourceRect;
	sourceRect.left = m_CurrFrameX*m_FrameWidth;
	sourceRect.top = m_CurrFrameY*m_FrameHeight;
	sourceRect.right = sourceRect.left + m_FrameWidth;
	sourceRect.bottom = sourceRect.top + m_FrameHeight;

	m_pGraphics->getSpriteBatch()->Begin();
	m_pGraphics->getSpriteBatch()->Draw(m_SpriteTexture, m_Pos, &sourceRect, Colors::White, m_Angle, m_Origin);
	m_pGraphics->getSpriteBatch()->End();
}

void cAnimatedSprite::Update(float deltaT)
{
	if (m_isPaused || !m_Enabled)
		return;

	m_TotalElapsed += deltaT;
	if (m_TotalElapsed > m_Speed)
	{
		m_TotalElapsed -= m_Speed;
		if(m_isAnimComplete &&!m_isLoopable)
		{
			m_CurrFrameX = m_FrameNumbersX - 1;
			m_CurrFrameY = m_FrameNumbersY - 1;
		}
		else
		{
			m_CurrFrameX++;
		}

		if (m_CurrFrameX >= m_FrameNumbersX)
		{
			m_CurrFrameX = 0;
			m_CurrFrameY++;

			if (m_CurrFrameY >= m_FrameNumbersY)
			{
				m_isAnimComplete = TRUE;
			    m_CurrFrameY = 0;
			}
		}
	}
}

void cAnimatedSprite::Release()
{
	ReleaseCOM(m_SpriteTexture);
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
void cAnimatedSprite::SetPos(XMFLOAT2 &pos)
{
	m_Pos = pos;
}

XMFLOAT2 cAnimatedSprite::Position() const
{
	return m_Pos;
}

void cAnimatedSprite::SetAngle(float angle)
{
	m_Angle = angle *180/XM_PI;
}

float cAnimatedSprite::Angle() const
{
	return m_Angle;
}

void cAnimatedSprite::SetCurrentFrame(int frameX, int frameY)
{
	m_CurrFrameX = frameX;
	m_CurrFrameY = frameY;
}

bool cAnimatedSprite::Loop() const
{
	return m_isLoopable;
}


