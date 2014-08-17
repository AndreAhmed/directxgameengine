//--------------------------------------------------------------------------------------
// Graphics.cpp
//
// Author: Ahmed Saleh Tolba
// xgameprogrammer@hotmail.com 
//--------------------------------------------------------------------------------------
#include "Graphics.h"

using namespace DirectX;
using namespace std;


HRESULT cGraphics::Initialize(HWND hwnd, bool windowed)
{

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	m_Width = width;
	m_Height = height;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = windowed;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDeviceAndSwapChain(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain the Direct3D 11.1 versions if available
	hr = m_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pd3dDevice1));
	if (SUCCEEDED(hr))
	{
		(void)m_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1));
	}

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencil);
	if (FAILED(hr))
		return hr;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);



	return hr;
}

void cGraphics::Clear()
{
	// Clear the back buffer
	//
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

}

void cGraphics::Render()
{
 

	m_pSwapChain->Present(0, 0);
}

void cGraphics::Release()
{
	ReleaseCOM(m_pImmediateContext);
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pImmediateContext);
	ReleaseCOM(m_pImmediateContext1);
	ReleaseCOM(m_pImmediateContext);
	ReleaseCOM(m_pd3dDevice1);
	ReleaseCOM(m_pd3dDevice);
	ReleaseCOM(m_pRasterizerState);
}
cGraphics::cGraphics()
{
}

cGraphics ::~cGraphics()
{
}


HRESULT cGraphics::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

void cGraphics::SetPerspective(float fov, float near, float far)
{
	m_Proj = XMMatrixPerspectiveFovLH(fov, AspectRatio(), 1.0f, 1000.0f);
}

void cGraphics::LookAt(float x, float y, float z)
{
	// Build the view matrix.
	XMVECTOR Eye = XMVectorSet(x, y, z, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_View = XMMatrixLookAtLH(Eye, At, Up);
}
float cGraphics::AspectRatio()
{
	return static_cast<float>(m_Width) / m_Height;
}

ID3D11DeviceContext * cGraphics::getContext()
{
	return m_pImmediateContext;
}

XMMATRIX cGraphics::getViewMatrix()
{
	return m_View;
}

XMMATRIX cGraphics::getProjMatrix()
{
	return m_Proj;
}

ID3D11Device* cGraphics::getDevice()
{
	return m_pd3dDevice;
}
 
HRESULT cGraphics::SetWireFrameMode(BOOL enable)
{
	HRESULT hr;
	if (enable)
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		wfdesc.CullMode = D3D11_CULL_NONE;
		hr  = m_pd3dDevice->CreateRasterizerState(&wfdesc, &m_pRasterizerState);

		m_pImmediateContext->RSSetState(m_pRasterizerState);
	}
	else
	{
		D3D11_RASTERIZER_DESC wfdesc;
		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_SOLID;
		wfdesc.CullMode = D3D11_CULL_NONE;
		 hr = m_pd3dDevice->CreateRasterizerState(&wfdesc, &m_pRasterizerState);

		m_pImmediateContext->RSSetState(m_pRasterizerState);
	}
	return hr;
}


HRESULT cGrid::CreateGrid(float width, float depth, UINT n, UINT m)
{
	int vertexCount = m*n;
	UINT faceCount = (m - 1)*(n - 1) * 2; // each quad consists of two triangles

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	// project the grid into xz plane 
	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1); // texture co-ordinates
	float dv = 1.0f / (m - 1);


	m_Mesh.m_Vertices.resize(vertexCount);

	// build the vertices of the grid, including the normals and the tangent,
	//you can build then the bitanget by cross product for normal maps -_- 

	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz; // reset for the next cell 
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			m_Mesh.m_Vertices[i*n + j].Position = XMFLOAT3(x, 0.0f, z);
			m_Mesh.m_Vertices[i*n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			m_Mesh.m_Vertices[i*n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			m_Mesh.m_Vertices[i*n + j].TextureCords.x = j*du;
			m_Mesh.m_Vertices[i*n + j].TextureCords.y = i*dv;
		}
	}

	m_Mesh.m_Indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			m_Mesh.m_Indices[k] = i*n + j;
			m_Mesh.m_Indices[k + 1] = i*n + j + 1;
			m_Mesh.m_Indices[k + 2] = (i + 1)*n + j;

			m_Mesh.m_Indices[k + 3] = (i + 1)*n + j;
			m_Mesh.m_Indices[k + 4] = i*n + j + 1;
			m_Mesh.m_Indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}

	m_IndicesSize = m_Mesh.m_Indices.size();
	 
	// Pack all the vertices into vertex buffer
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(MeshVertex)* vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &(m_Mesh.m_Vertices[0]);
	m_pGraphics->getDevice()->CreateBuffer(&vbd, &vinitData, &mVB);

	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;
	m_pGraphics->getContext()->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);


	// Pack the indices of all the meshes into one index buffer.

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT)* m_IndicesSize;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_Mesh.m_Indices[0];
	m_pGraphics->getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);

	m_pGraphics->getContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	m_pGraphics->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(ConstantBuffer);
	ibd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ibd.CPUAccessFlags = 0;
	HRESULT hr = m_pGraphics->getDevice()->CreateBuffer(&ibd, nullptr, &m_pConstantBuffer);
	if (FAILED(hr))
		return hr;
	 
}

void cGrid::Update()
{

}

void cGrid::InitGrid(cGraphics *Graphics)
{
	m_pGraphics = Graphics;
	m_World = XMMatrixIdentity();
}

void cGrid::DrawGrid()
{
	XMFLOAT4 vLightDir = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	XMFLOAT4 vLightColor = XMFLOAT4(1.0f, 1, 1, 1.0f);

	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(m_World);
	cb1.mView = XMMatrixTranspose(m_pGraphics->getViewMatrix());
	cb1.mProjection = XMMatrixTranspose(m_pGraphics->getProjMatrix());
	cb1.vLightDir = vLightDir;
	cb1.vLightColor = vLightColor;

	m_pGraphics->getContext()->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

	m_pGraphics->getContext()->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pGraphics->getContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pGraphics->getContext()->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pGraphics->getContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pGraphics->getContext()->DrawIndexed(m_IndicesSize, 0, 0);
	 

}

HRESULT cGrid::CompileFX()
{

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr = m_pGraphics->CompileShaderFromFile(L"FX/Color.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = m_pGraphics->getDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }

	};

	UINT numElements = ARRAYSIZE(vertexDesc);

	// Create the input layout
	hr = m_pGraphics->getDevice()->CreateInputLayout(vertexDesc, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_InputLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	m_pGraphics->getContext()->IASetInputLayout(m_InputLayout);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = m_pGraphics->CompileShaderFromFile(L"FX/Color.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = m_pGraphics->getDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;
	 
}

cGrid::cGrid()
{

}

cGrid::~cGrid()
{

}

void cGrid::Release()
{
 	ReleaseCOM(m_pVSBlob);
	ReleaseCOM(m_pVertexShader);
	ReleaseCOM(m_pPixelShader);
	ReleaseCOM(m_InputLayout);
	ReleaseCOM(m_pConstantBuffer);
	ReleaseCOM(mIB);
	ReleaseCOM(mVB);
}
