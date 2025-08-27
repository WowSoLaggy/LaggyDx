#include "stdafx.h"
#include "GltfResource.h"

#include "IndexBuffer.h"
#include "Mesh.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "VertexTypes.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "tiny_gltf.h"


namespace Dx
{
  namespace
  {
    tinygltf::Model loadModelFromFile(const fs::path& i_filePath)
    {
      tinygltf::Model model;
      tinygltf::TinyGLTF loader;
      std::string err;
      std::string warn;

      bool loadedOk = false;
      if (i_filePath.extension() == ".glb")
      {
        // Binary glTF
        loadedOk = loader.LoadBinaryFromFile(&model, &err, &warn, i_filePath.string());
      }
      else
      {
        // ASCII glTF.
        loadedOk = loader.LoadASCIIFromFile(&model, &err, &warn, i_filePath.string());
      }
      CONTRACT_EXPECT(warn.empty(), warn);
      CONTRACT_EXPECT(err.empty(), err);
      CONTRACT_EXPECT(loadedOk, "Failed to load GLTF resource");

      return model;
    }

    std::vector<float> extractPositions(const tinygltf::Model& i_model, const tinygltf::Primitive& i_primitive)
    {
      std::vector<float> positions;

      auto it = i_primitive.attributes.find("POSITION");
      if (it != i_primitive.attributes.end())
      {
        const auto& accessor = i_model.accessors[it->second];
        const auto& bufferView = i_model.bufferViews[accessor.bufferView];
        const auto& buffer = i_model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(
          &buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        positions.assign(data, data + accessor.count * 3);
      }

      return positions;
    }

    std::vector<float> extractNormals(const tinygltf::Model& i_model, const tinygltf::Primitive& i_primitive)
    {
      std::vector<float> normals;

      auto it = i_primitive.attributes.find("NORMAL");
      if (it != i_primitive.attributes.end())
      {
        const auto& accessor = i_model.accessors[it->second];
        const auto& bufferView = i_model.bufferViews[accessor.bufferView];
        const auto& buffer = i_model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(
          &buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        normals.assign(data, data + accessor.count * 3);
      }

      return normals;
    }

    std::vector<float> extractTexcoords(const tinygltf::Model& i_model, const tinygltf::Primitive& i_primitive)
    {
      std::vector<float> texcoords;

      auto it = i_primitive.attributes.find("TEXCOORD_0");
      if (it != i_primitive.attributes.end())
      {
        const auto& accessor = i_model.accessors[it->second];
        const auto& bufferView = i_model.bufferViews[accessor.bufferView];
        const auto& buffer = i_model.buffers[bufferView.buffer];
        const float* data = reinterpret_cast<const float*>(
          &buffer.data[bufferView.byteOffset + accessor.byteOffset]);
        texcoords.assign(data, data + accessor.count * 2);
      }

      return texcoords;
    }

    std::vector<int> extractIndices(const tinygltf::Model& i_model, const tinygltf::Primitive& i_primitive)
    {
      std::vector<int> indices;

      if (i_primitive.indices >= 0)
      {
        const auto& accessor = i_model.accessors[i_primitive.indices];
        const auto& bufferView = i_model.bufferViews[accessor.bufferView];
        const auto& buffer = i_model.buffers[bufferView.buffer];
        const uint8_t* data = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
          const uint16_t* idx = reinterpret_cast<const uint16_t*>(data);
          for (size_t i = 0; i < accessor.count; ++i)
            indices.push_back(static_cast<int>(idx[i]));
        }
        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
        {
          const uint32_t* idx = reinterpret_cast<const uint32_t*>(data);
          for (size_t i = 0; i < accessor.count; ++i)
            indices.push_back(static_cast<int>(idx[i]));
        }
        else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
        {
          const uint8_t* idx = reinterpret_cast<const uint8_t*>(data);
          for (size_t i = 0; i < accessor.count; ++i)
            indices.push_back(static_cast<int>(idx[i]));
        }
      }

      return indices;
    }

    std::vector<VertexPos3NormText> getVertices(
      const std::vector<float>& i_positions,
      const std::vector<float>& i_normals,
      const std::vector<float>& i_texcoords)
    {
      std::vector<VertexPos3NormText> vertices;
      size_t vertexCount = i_positions.size() / 3;
      for (size_t i = 0; i < vertexCount; ++i)
      {
        Sdk::Vector3F pos{
          i_positions[i * 3 + 0],
          i_positions[i * 3 + 1],
          i_positions[i * 3 + 2]
        };
        Sdk::Vector3F norm{ 0, 0, 0 };
        if (i_normals.size() == i_positions.size())
        {
          norm.x = i_normals[i * 3 + 0];
          norm.y = i_normals[i * 3 + 1];
          norm.z = i_normals[i * 3 + 2];
        }
        Sdk::Vector2F tex{ 0, 0 };
        if (i_texcoords.size() / 2 == vertexCount)
        {
          tex.x = i_texcoords[i * 2 + 0];
          tex.y = i_texcoords[i * 2 + 1];
        }
        VertexPos3NormText v;
        v.position = pos;
        v.normal = norm;
        v.texture = tex;
        vertices.push_back(v);
      }
      return vertices;
    }

  } // anon NS


  GltfResource::GltfResource(fs::path i_meshFilePath)
    : d_filePath(std::move(i_meshFilePath))
  {
  }

  std::shared_ptr<IModel> GltfResource::getModel() const
  {
    return d_model;
  }

  void GltfResource::load(IRenderDevice& i_renderDevice)
  {
    CONTRACT_EXPECT(d_model == nullptr, "Model already loaded");

    const auto gltfModel = loadModelFromFile(d_filePath);
    auto model = std::make_shared<Model>();

    for (const auto& node : gltfModel.nodes)
    {
      if (node.mesh < 0)
        continue;

      const auto& gltfMesh = gltfModel.meshes[node.mesh];
      for (const auto& primitive : gltfMesh.primitives)
      {
        // VB
        const auto positions = extractPositions(gltfModel, primitive);
        const auto normals = extractNormals(gltfModel, primitive);
        const auto texcoords = extractTexcoords(gltfModel, primitive);
        const auto vertices = getVertices(positions, normals, texcoords);
        const auto vertexBuffer = std::make_shared<VertexBuffer>(i_renderDevice, vertices);

        // IB
        const auto indices = extractIndices(gltfModel, primitive);
        const auto indexBuffer = std::make_shared<IndexBuffer>(i_renderDevice, indices);

        // --- Compute AABB (pseudo-code, adapt as needed) ---
        /*Aabb aabb;
        if (!positions.empty())
        {
          for (size_t i = 0; i < positions.size(); i += 3)
          {
            Sdk::Vector3F p{ positions[i], positions[i + 1], positions[i + 2] };
            aabb.expandToInclude(p);
          }
        }*/

        // Create mesh and add to model
        auto mesh = std::make_shared<Mesh>();
        mesh->setVertexBuffer(vertexBuffer);
        mesh->setIndexBuffer(indexBuffer);

        // Setup material
        MaterialSpan matSpan;
        matSpan.startIndex = 0;
        matSpan.count = indexBuffer->getIndexCount();
        matSpan.material.textureName = "White.png";
        mesh->getMaterials().push_back(std::move(matSpan));

        model->addMesh(mesh);
      }
    }

    d_model = model;
  }

  void GltfResource::unload()
  {
    d_model.reset();
  }

} // ns Dx
