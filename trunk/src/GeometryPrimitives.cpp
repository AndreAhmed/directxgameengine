 
#include "GeometryPrimitives.h"
 
using namespace DirectX;


void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData& meshData)
{
	//
	// Create the vertices.
	//

	Vertex v[24];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;

	// Fill in the front face vertex data.
	v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the back face vertex data.
	v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the top face vertex data.
	v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
	v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Fill in the left face vertex data.
	v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	// Fill in the right face vertex data.
	v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
	v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	meshData.Vertices.assign(&v[0], &v[24]);

	//
	// Create the indices.
	//

	UINT i[36];

	// Fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	// Fill in the back face index data
	i[6] = 4; i[7] = 5; i[8] = 6;
	i[9] = 4; i[10] = 6; i[11] = 7;

	// Fill in the top face index data
	i[12] = 8; i[13] = 9; i[14] = 10;
	i[15] = 8; i[16] = 10; i[17] = 11;

	// Fill in the bottom face index data
	i[18] = 12; i[19] = 13; i[20] = 14;
	i[21] = 12; i[22] = 14; i[23] = 15;

	// Fill in the left face index data
	i[24] = 16; i[25] = 17; i[26] = 18;
	i[27] = 16; i[28] = 18; i[29] = 19;

	// Fill in the right face index data
	i[30] = 20; i[31] = 21; i[32] = 22;
	i[33] = 20; i[34] = 22; i[35] = 23;

	meshData.Indices.assign(&i[0], &i[36]);
}

void GeometryGenerator::CreateSphere(float radius, UINT sliceCount, UINT stackCount, MeshData& meshData)
{
	meshData.Vertices.clear();
	meshData.Indices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

	meshData.Vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.0f*XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Position.x = radius*sinf(phi)*cosf(theta);
			v.Position.y = radius*cosf(phi);
			v.Position.z = radius*sinf(phi)*sinf(theta);

			// Partial derivative of P with respect to theta
			v.TangentU.x = -radius*sinf(phi)*sinf(theta);
			v.TangentU.y = 0.0f;
			v.TangentU.z = +radius*sinf(phi)*cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.TangentU);
			XMStoreFloat3(&v.TangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Position);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TexC.x = theta / XM_2PI;
			v.TexC.y = phi / XM_PI;

			meshData.Vertices.push_back(v);
		}
	}

	meshData.Vertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		meshData.Indices.push_back(0);
		meshData.Indices.push_back(i + 1);
		meshData.Indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);

			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			meshData.Indices.push_back(baseIndex + i*ringVertexCount + j + 1);
			meshData.Indices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)meshData.Vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex + i);
		meshData.Indices.push_back(baseIndex + i + 1);
	}
}
 
 
void GeometryGenerator::CreateGrid(float width, float depth, UINT m, UINT n, MeshData& meshData)
{
	UINT vertexCount = m*n;
	UINT faceCount = (m - 1)*(n - 1) * 2;

	//
	// Create the vertices.
	//

	float halfWidth = 0.5f*width;
	float halfDepth = 0.5f*depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.Vertices.resize(vertexCount);
	for (UINT i = 0; i < m; ++i)
	{
		float z = halfDepth - i*dz;
		for (UINT j = 0; j < n; ++j)
		{
			float x = -halfWidth + j*dx;

			meshData.Vertices[i*n + j].Position = XMFLOAT3(x, 0.0f, z);
			meshData.Vertices[i*n + j].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			meshData.Vertices[i*n + j].TangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);

			// Stretch texture over grid.
			meshData.Vertices[i*n + j].TexC.x = j*du;
			meshData.Vertices[i*n + j].TexC.y = i*dv;
		}
	}

	//
	// Create the indices.
	//

	meshData.Indices.resize(faceCount * 3); // 3 indices per face

	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			meshData.Indices[k] = i*n + j;
			meshData.Indices[k + 1] = i*n + j + 1;
			meshData.Indices[k + 2] = (i + 1)*n + j;

			meshData.Indices[k + 3] = (i + 1)*n + j;
			meshData.Indices[k + 4] = i*n + j + 1;
			meshData.Indices[k + 5] = (i + 1)*n + j + 1;

			k += 6; // next quad
		}
	}
}

void GeometryGenerator::CreateFullscreenQuad(MeshData& meshData)
{
	meshData.Vertices.resize(4);
	meshData.Indices.resize(6);

	// Position coordinates specified in NDC space.
	meshData.Vertices[0] = Vertex(
		-1.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f);

	meshData.Vertices[1] = Vertex(
		-1.0f, +1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f);

	meshData.Vertices[2] = Vertex(
		+1.0f, +1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f);

	meshData.Vertices[3] = Vertex(
		+1.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f);

	meshData.Indices[0] = 0;
	meshData.Indices[1] = 1;
	meshData.Indices[2] = 2;

	meshData.Indices[3] = 0;
	meshData.Indices[4] = 2;
	meshData.Indices[5] = 3;
}
