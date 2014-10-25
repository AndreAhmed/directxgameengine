#pragma once

#include "Graphics.h"
#include "Globals.h"

class cGrid
{
public:

	struct ConstantBuffer
	{
		DirectX::XMMATRIX mWorld;
		DirectX::XMMATRIX mView;
		DirectX::XMMATRIX mProjection;
		DirectX::XMFLOAT4 vLightDir;
		DirectX::XMFLOAT4 vLightColor;
		DirectX::XMFLOAT4 vOutputColor;
	};

public:
	cGrid();
	~cGrid();
	void InitGrid(cGraphics *Graphics);
	HRESULT CreateGrid(float width, float depth, UINT n, UINT m);
	void    SetPosition(float x, float y, float z);
	HRESULT CompileFX();
	void Update();
	void DrawGrid();
	void Release();
	Mesh getMesh() { return m_Mesh; }
private:
	cGraphics *m_pGraphics;
	Mesh m_Mesh;
	DirectX::XMMATRIX  m_World;
	DirectX::XMFLOAT3  m_Position;
	int m_Widht;
	int m_Depth;
	UINT m_IndicesSize;

private:
	ID3D11Buffer* mVB = nullptr;
	ID3D11Buffer* mIB = nullptr;
	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11VertexShader*     m_pVertexShader = nullptr;
	ID3D11PixelShader*      m_pPixelShader = nullptr;
	ID3D11PixelShader*		m_pPixelShaderSolid = nullptr;
	ID3D11InputLayout*		m_InputLayout = nullptr;
	ID3D11Buffer*           m_pConstantBuffer = nullptr;

};

