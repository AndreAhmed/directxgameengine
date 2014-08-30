#pragma once
#include "Graphics.h"
#include "Globals.h"

class RenderStateHelper
{
public:
	RenderStateHelper(cGraphics *graphics);
	~RenderStateHelper();
	
	static void ResetAll(ID3D11DeviceContext* deviceContext);

	ID3D11RasterizerState* RasterizerState();
	ID3D11BlendState* BlendState();
	ID3D11DepthStencilState* DepthStencilState();

	void SaveRasterizerState();
	void RestoreRasterizerState() const;

	void SaveBlendState();
	void RestoreBlendState() const;

	void SaveDepthStencilState();
	void RestoreDepthStencilState() const;

	void SaveAll();
	void RestoreAll() const;
	void Release();
private:
	RenderStateHelper(const RenderStateHelper& rhs);
	RenderStateHelper& operator=(const RenderStateHelper& rhs);

	cGraphics *m_Graphics;

	ID3D11RasterizerState* m_RasterizerState;
	ID3D11BlendState* m_BlendState;
	FLOAT* m_BlendFactor;
	UINT m_SampleMask;
	ID3D11DepthStencilState* m_DepthStencilState;
	UINT m_StencilRef;
};

