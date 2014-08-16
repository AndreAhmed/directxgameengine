#pragma once

#include "Globals.h"

 
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
	void    SetPerspective(float fov, float near, float far);
	void	LookAt(float x, float y, float z);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	float	AspectRatio();
	ID3D11Device*         getDevice();
	ID3D11DeviceContext * getContext();
	DirectX::XMMATRIX	  getViewMatrix();
	DirectX::XMMATRIX     getProjMatrix();
 
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

private:
	UINT m_Width;
	UINT m_Height;
private:
	
	DirectX::XMMATRIX m_View;
	DirectX::XMMATRIX m_Proj;

};


class MeshVertex
{

public:
	MeshVertex(){}
	MeshVertex(const DirectX::XMFLOAT3 &p, const DirectX::XMFLOAT3&n, const DirectX::XMFLOAT2 &uv, const DirectX::XMFLOAT3 &t) : Position(p), Normal(n), TextureCords(uv), TangentU(t)
	{
	 // copy ctr
	}

	MeshVertex(float px, float py, float pz, float nx, float ny, float nz, float u, float v, float tx, float ty, float tz) : Position(px, py, pz),
	    Normal(nx, ny, nz),
		TextureCords(u, v), TangentU(tx, ty, tz)
	{

	}
public:
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 TextureCords;
	DirectX::XMFLOAT3 TangentU;
	DirectX::XMFLOAT4 Color;
};


struct Mesh
{
	std::vector<MeshVertex> m_Vertices;
	std::vector<UINT> m_Indices;
};

  
class cGrid
{
public:

	struct ConstantBuffer
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
	};

public:
	cGrid();
	~cGrid();
	void InitGrid(cGraphics *Graphics);
	HRESULT CreateGrid(float width, float depth, UINT n, UINT m);
	HRESULT CompileFX();
	void Update();
	void DrawGrid();
	void Release();
	Mesh getMesh() { return m_Mesh; }
private: 
	cGraphics *m_pGraphics;
	Mesh m_Mesh;
	DirectX::XMMATRIX  m_World;
	int m_Widht;
	int m_Depth;
	UINT m_IndicesSize;

private:
	ID3D11Buffer* mVB = nullptr;
	ID3D11Buffer* mIB = nullptr;
	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11VertexShader*     m_pVertexShader = nullptr;
	ID3D11PixelShader*      m_pPixelShader = nullptr;
	ID3D11InputLayout*		m_InputLayout = nullptr;
	ID3D11Buffer*           m_pConstantBuffer = nullptr;

};