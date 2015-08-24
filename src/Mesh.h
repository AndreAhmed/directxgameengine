#pragma once

#include "Globals.h"
#include "Graphics.h"
struct aiMesh;

namespace Library
{
    class Material;
    class ModelMaterial;
	 
    class Mesh
    {
        friend class Model;

    public:
        Mesh(Model& model, ModelMaterial* material, cGraphics * graphics);
        ~Mesh();

        Model& GetModel();
        ModelMaterial* GetMaterial();
        const std::string& Name() const;

        const std::vector<DirectX::XMFLOAT3>& Vertices() const;
		const std::vector<DirectX::XMFLOAT3>& Normals() const;
		const std::vector<DirectX::XMFLOAT3>& Tangents() const;
		const std::vector<DirectX::XMFLOAT3>& BiNormals() const;
		const std::vector<std::vector<DirectX::XMFLOAT3>*>& TextureCoordinates() const;
		const std::vector<std::vector<DirectX::XMFLOAT4>*>& VertexColors() const;
        UINT FaceCount() const;
        const std::vector<UINT>& Indices() const;

        void CreateIndexBuffer(ID3D11Buffer** indexBuffer);

    private:
		Mesh(Model& model, aiMesh& mesh, cGraphics * graphics);
        Mesh(const Mesh& rhs);
        Mesh& operator=(const Mesh& rhs);

        Model& mModel;
        ModelMaterial* mMaterial;
        std::string mName;
		std::vector<DirectX::XMFLOAT3> mVertices;
		std::vector<DirectX::XMFLOAT3> mNormals;
		std::vector<DirectX::XMFLOAT3> mTangents;
		std::vector<DirectX::XMFLOAT3> mBiNormals;
		std::vector<std::vector<DirectX::XMFLOAT3>*> mTextureCoordinates;
		std::vector<std::vector<DirectX::XMFLOAT4>*> mVertexColors;
        UINT mFaceCount;
        std::vector<UINT> mIndices;
		cGraphics *mGraphics;
    };
}