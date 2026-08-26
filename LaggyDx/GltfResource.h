#pragma once

#include "Animation.h"
#include "IGltfResource.h"
#include "LaggyDxFwd.h"
#include "MaterialSpan.h"
#include "Model.h"
#include "VertexTypes.h"


namespace tinygltf
{
  class Model;
}


namespace Dx
{
  class GltfResource : public IGltfResource
  {
  public:
    GltfResource(fs::path i_filePath);

    virtual void load() override;
    virtual void unload() override;

    virtual std::shared_ptr<IModel> getModel() const override;
    virtual std::shared_ptr<IModel> makeTransformedModel(const ModelTransform& i_transform) const override;

  private:
    // The CPU-side copy kept so transformed models can be built without reading back from the GPU.
    struct MeshData
    {
      std::vector<VertexPos3NormText> vertices;
      std::vector<int> indices;
      Material material;
    };

    const fs::path d_filePath;
    std::shared_ptr<IModel> d_model;
    std::vector<MeshData> d_meshData;

    static std::shared_ptr<IModel> buildModel(const std::vector<MeshData>& i_meshData);

    static void collectNodeMeshes(
      const tinygltf::Model& i_gltfModel,
      int i_nodeIndex,
      const Matrix& i_parentTransform,
      std::vector<MeshData>& o_meshData,
      std::unordered_map<int, std::shared_ptr<MemoryTexture>>& a_imageCache);
  };
} // ns Dx
