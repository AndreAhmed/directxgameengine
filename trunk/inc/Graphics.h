#pragma once

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

class  cGraphics 
{
public:
	 cGraphics ();
	~ cGraphics ();

	HRESULT Initialize(HWND hwnd, bool windowed);
	void    Render();
	void    Clear();
	void    Release();
private:
	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pd3dDevice = nullptr;
	ID3D11Device1*          m_pd3dDevice1 = nullptr;
	ID3D11DeviceContext*    m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*   m_pImmediateContext1 = nullptr;
	IDXGISwapChain*         m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;

};

