#pragma once

#include "Globals.h"
#include "Graphics.h"

namespace Library
{
    class Mesh;
    class ModelMaterial;

    class Model
    {
    public:
        Model(const std::string& filename, bool flipUVs = false, cGraphics *graphics = nullptr);
        ~Model();

        bool HasMeshes() const;
        bool HasMaterials() const;

        const std::vector<Mesh*>& Meshes() const;
        const std::vector<ModelMaterial*>& Materials() const;

    private:
        Model(const Model& rhs);
        Model& operator=(const Model& rhs);
		 
        std::vector<Mesh*> mMeshes;
        std::vector<ModelMaterial*> mMaterials;

		cGraphics *mGraphics;
    };
}
