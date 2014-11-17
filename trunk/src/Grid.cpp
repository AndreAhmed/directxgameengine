#include "Grid.h"
#include "WICTextureLoader.h"

using namespace DirectX;


HRESULT cGrid::CreateGrid(float width, float depth, UINT n, UINT m)
{
	HRESULT hr;
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
//			m_Mesh.m_Vertices[i*n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
//			m_Mesh.m_Vertices[i*n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

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
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(MeshVertex)* vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &(m_Mesh.m_Vertices[0]);
	m_pGraphics->getDevice()->CreateBuffer(&vbd, &vinitData, &mVB);

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

	// Create the constant buffer
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(ConstantBuffer);
	ibd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ibd.CPUAccessFlags = 0;
	hr = m_pGraphics->getDevice()->CreateBuffer(&ibd, nullptr, &m_pConstantBuffer);
	if (FAILED(hr))
		return hr;

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
void cGrid::SetPosition(float x, float y, float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;

	XMMATRIX translation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_World = translation;
}

void cGrid::DrawGrid()
{
	XMFLOAT4 vLightDir = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	XMFLOAT4 vLightColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_pGraphics->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// Set the input layout
	m_pGraphics->getContext()->IASetInputLayout(m_InputLayout);

	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;
	m_pGraphics->getContext()->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	m_pGraphics->getContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(m_World);
	cb1.mView = XMMatrixTranspose(m_pGraphics->getViewMatrix());
	cb1.mProjection = XMMatrixTranspose(m_pGraphics->getProjectionMatrix());
	cb1.vLightDir = vLightDir;
	cb1.vLightColor = vLightColor;

	m_pGraphics->getContext()->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb1, 0, 0);
	
	m_pGraphics->getContext()->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pGraphics->getContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pGraphics->getContext()->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pGraphics->getContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pGraphics->getContext()->PSSetShaderResources(0, 1, &m_pGridTexture);
	m_pGraphics->getContext()->PSSetSamplers(0, 1, &m_pGridTextureSampler);
	m_pGraphics->getContext()->DrawIndexed(m_IndicesSize, 0, 0);

}

HRESULT cGrid::CompileFX()
{
	HRESULT hr;

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = m_pGraphics->CompileShaderFromFile(L"FX/Color.fx", "VS", "vs_4_0", &pVSBlob);
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


	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	//,
	//{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	//
	UINT numElements = ARRAYSIZE(vertexDesc);

	// Create the input layout
	hr = m_pGraphics->getDevice()->CreateInputLayout(vertexDesc, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_InputLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;


    hr = DirectX::CreateWICTextureFromFile(m_pGraphics->getDevice(), L"grass.jpg", nullptr, &m_pGridTexture);
	if (FAILED(hr))
	{
		throw(cGameException(gameErrorNS::FATAL_ERROR, "Couldn't load  Grid texture."));
	}



	// Describe the Sample State
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//Create the Sample State
	hr = m_pGraphics->getDevice()->CreateSamplerState(&sampDesc, &m_pGridTextureSampler);

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

DirectX::XMMATRIX cGrid::GetWorldMatrix()
{
	return m_World;
}

void cGrid::SetWorldMatrix(DirectX::CXMMATRIX worldMatrix)
{
	m_World = worldMatrix;
}
