#pragma once

#include "Globals.h"
#include "Camera.h"
#include "SpriteBatch.h"


class  cGraphics 
{
public:
	 cGraphics ();
	~ cGraphics ();

	void    Initialize(HWND hwnd, bool windowed);
	void    Render();
	void    Clear();
	void    Release();
	void    SetCamera(cCamera *camara);
	void    CreateSpriteBatch();

	DirectX::XMMATRIX  getViewMatrix();
	DirectX::XMMATRIX  getProjectionMatrix();

	HRESULT SetWireFrameMode(BOOL enable);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	float	AspectRatio();
	ID3D11Device*         getDevice();
	ID3D11DeviceContext * getContext();
 	std::shared_ptr<DirectX::SpriteBatch> getSpriteBatch();

private:
	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pd3dDevice = nullptr;
	ID3D11Device1*          m_pd3dDevice1 = nullptr;
	ID3D11DeviceContext*    m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*   m_pImmediateContext1 = nullptr;
	IDXGISwapChain*         m_pSwapChain = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11Texture2D*        m_pDepthStencil = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	ID3D11DepthStencilState* m_pDepthStencilState = nullptr;
	ID3D11RasterizerState*  m_pRasterizerState = nullptr;

private:
	UINT m_Width;
	UINT m_Height;
	std::shared_ptr<DirectX::SpriteBatch> m_SpriteBatch; // shared sprite batch
	cCamera *m_pCamera; // current camera;

};


class MeshVertex
{

public:
	MeshVertex(){}
	//MeshVertex(const DirectX::XMFLOAT3 &p, const DirectX::XMFLOAT3&n, const DirectX::XMFLOAT2 &uv, const DirectX::XMFLOAT3 &t) : Position(p), Normal(n), TextureCords(uv), TangentU(t)
	//{
	// // copy ctr
	//}

	MeshVertex(const DirectX::XMFLOAT3 &p, const DirectX::XMFLOAT2 &uv) : Position(p),TextureCords(uv) 
	{
		// copy ctr
	}
	//MeshVertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v, float tx, float ty, float tz) : 
	//	Position(px, py, pz),
	//    //Normal(nx, ny, nz),
	//	TextureCords(u, v)/*, TangentU(tx, ty, tz*/)
	//{

	//}
	MeshVertex(float px, float py, float pz,  float u, float v) :
		Position(px, py, pz),
		//Normal(nx, ny, nz),
		TextureCords(u, v)/*, TangentU(tx, ty, tz*/
	{

	}
public:
	DirectX::XMFLOAT3 Position;
	//DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TextureCords;
//	DirectX::XMFLOAT3 TangentU;
//	DirectX::XMFLOAT4 Color;
};


struct Mesh
{
	std::vector<MeshVertex> m_Vertices;
	std::vector<UINT> m_Indices;
};

  
 