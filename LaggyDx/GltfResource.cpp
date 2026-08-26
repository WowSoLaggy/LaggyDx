#include "stdafx.h"
#include "GltfResource.h"

#include "IndexBuffer.h"
#include "Mesh.h"
#include "Model.h"
#include "TextureUtils.h"
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
      // Warnings are informational (unused extensions, default samplers) - only errors are fatal.
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

    std::shared_ptr<MemoryTexture> decodeImage(const tinygltf::Image& i_image)
    {
      // tiny_gltf decodes embedded and external images up front, so pixels are normally already here.
      if (!i_image.image.empty())
      {
        return TextureUtils::createMemoryTexture(
          i_image.image.data(),
          static_cast<int>(i_image.width),
          static_cast<int>(i_image.height),
          static_cast<TextureUtils::GlChannels>(i_image.component));
      }

      return nullptr;
    }

    // Images are cached by index so primitives sharing an atlas share one GPU texture.
    using ImageCache = std::unordered_map<int, std::shared_ptr<MemoryTexture>>;

    std::shared_ptr<MemoryTexture> extractBaseColorTexture(
      const tinygltf::Model& i_model, const tinygltf::Material& i_material, ImageCache& a_cache)
    {
      const int textureIndex = i_material.pbrMetallicRoughness.baseColorTexture.index;
      if (textureIndex < 0 || textureIndex >= static_cast<int>(i_model.textures.size()))
        return nullptr;

      const int imageIndex = i_model.textures[textureIndex].source;
      if (imageIndex < 0 || imageIndex >= static_cast<int>(i_model.images.size()))
        return nullptr;

      const auto it = a_cache.find(imageIndex);
      if (it != a_cache.cend())
        return it->second;

      auto texture = decodeImage(i_model.images[imageIndex]);
      a_cache.emplace(imageIndex, texture);

      return texture;
    }

    Material extractMaterial(
      const tinygltf::Model& i_model, const tinygltf::Primitive& i_primitive, ImageCache& a_cache)
    {
      Material material;

      if (i_primitive.material < 0 || i_primitive.material >= static_cast<int>(i_model.materials.size()))
        return material;

      const auto& gltfMaterial = i_model.materials[i_primitive.material];
      material.name = gltfMaterial.name;

      // A material may be flat-coloured only - the base colour then carries the whole look.
      const auto& baseColor = gltfMaterial.pbrMetallicRoughness.baseColorFactor;
      if (baseColor.size() == 4)
      {
        material.diffuseColor = {
          static_cast<float>(baseColor[0]),
          static_cast<float>(baseColor[1]),
          static_cast<float>(baseColor[2]),
          static_cast<float>(baseColor[3])
        };
      }

      material.texture = extractBaseColorTexture(i_model, gltfMaterial, a_cache);

      return material;
    }

    Matrix getNodeLocalTransform(const tinygltf::Node& i_node)
    {
      if (i_node.matrix.size() == 16)
      {
        // glTF stores matrices column-major while SimpleMath is row-major, so this transposes on read.
        Matrix matrix;
        for (int col = 0; col < 4; ++col)
          for (int row = 0; row < 4; ++row)
            matrix.m[row][col] = static_cast<float>(i_node.matrix[col * 4 + row]);
        return matrix;
      }

      const auto scale = i_node.scale.size() == 3
        ? Vector3(static_cast<float>(i_node.scale[0]),
                  static_cast<float>(i_node.scale[1]),
                  static_cast<float>(i_node.scale[2]))
        : Vector3::One;

      const auto rotation = i_node.rotation.size() == 4
        ? Quaternion(static_cast<float>(i_node.rotation[0]),
                     static_cast<float>(i_node.rotation[1]),
                     static_cast<float>(i_node.rotation[2]),
                     static_cast<float>(i_node.rotation[3]))
        : Quaternion::Identity;

      const auto translation = i_node.translation.size() == 3
        ? Vector3(static_cast<float>(i_node.translation[0]),
                  static_cast<float>(i_node.translation[1]),
                  static_cast<float>(i_node.translation[2]))
        : Vector3::Zero;

      // Row-vector spelling of the spec's column-vector T * R * S.
      return Matrix::CreateScale(scale) *
             Matrix::CreateFromQuaternion(rotation) *
             Matrix::CreateTranslation(translation);
    }

    void applyTransform(std::vector<VertexPos3NormText>& a_vertices, const Matrix& i_transform)
    {
      // Normals need the inverse-transpose so they survive non-uniform scale.
      const auto normalTransform = i_transform.Invert().Transpose();

      for (auto& vertex : a_vertices)
      {
        const auto position = Vector3::Transform(
          { vertex.position.x, vertex.position.y, vertex.position.z }, i_transform);
        vertex.position = { position.x, position.y, position.z };

        auto normal = Vector3::TransformNormal(
          { vertex.normal.x, vertex.normal.y, vertex.normal.z }, normalTransform);
        normal.Normalize();
        vertex.normal = { normal.x, normal.y, normal.z };
      }
    }

    Aabb calculateAabb(const std::vector<VertexPos3NormText>& i_vertices)
    {
      if (i_vertices.empty())
        return Aabb();

      auto min = i_vertices.front().position;
      auto max = min;

      for (const auto& vertex : i_vertices)
      {
        min.x = std::min(min.x, vertex.position.x);
        min.y = std::min(min.y, vertex.position.y);
        min.z = std::min(min.z, vertex.position.z);

        max.x = std::max(max.x, vertex.position.x);
        max.y = std::max(max.y, vertex.position.y);
        max.z = std::max(max.z, vertex.position.z);
      }

      return Aabb(min.x, max.x, min.y, max.y, min.z, max.z);
    }

    std::vector<int> findRootNodes(const tinygltf::Model& i_model)
    {
      // Without a scene, roots are the nodes nobody lists as a child - visiting all would double-apply parents.
      std::vector<bool> isChild(i_model.nodes.size(), false);
      for (const auto& node : i_model.nodes)
        for (const int childIndex : node.children)
          isChild[childIndex] = true;

      std::vector<int> roots;
      for (int i = 0; i < static_cast<int>(i_model.nodes.size()); ++i)
        if (!isChild[i])
          roots.push_back(i);

      return roots;
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

  void GltfResource::collectNodeMeshes(
    const tinygltf::Model& i_gltfModel,
    const int i_nodeIndex,
    const Matrix& i_parentTransform,
    std::vector<MeshData>& o_meshData,
    std::unordered_map<int, std::shared_ptr<MemoryTexture>>& a_imageCache)
  {
    const auto& node = i_gltfModel.nodes[i_nodeIndex];
    const auto transform = getNodeLocalTransform(node) * i_parentTransform;

    if (node.mesh >= 0)
    {
      for (const auto& primitive : i_gltfModel.meshes[node.mesh].primitives)
      {
        MeshData meshData;

        const auto positions = extractPositions(i_gltfModel, primitive);
        const auto normals = extractNormals(i_gltfModel, primitive);
        const auto texcoords = extractTexcoords(i_gltfModel, primitive);

        meshData.vertices = getVertices(positions, normals, texcoords);
        applyTransform(meshData.vertices, transform);

        meshData.indices = extractIndices(i_gltfModel, primitive);
        meshData.material = extractMaterial(i_gltfModel, primitive, a_imageCache);

        o_meshData.push_back(std::move(meshData));
      }
    }

    for (const int childIndex : node.children)
      collectNodeMeshes(i_gltfModel, childIndex, transform, o_meshData, a_imageCache);
  }

  std::shared_ptr<IModel> GltfResource::buildModel(const std::vector<MeshData>& i_meshData)
  {
    auto model = std::make_shared<Model>();

    for (const auto& meshData : i_meshData)
    {
      auto mesh = std::make_shared<Mesh>();
      mesh->setVertexBuffer(std::make_shared<VertexBuffer>(meshData.vertices));

      const auto indexBuffer = std::make_shared<IndexBuffer>(meshData.indices);
      mesh->setIndexBuffer(indexBuffer);
      mesh->setAabb(calculateAabb(meshData.vertices));

      MaterialSpan matSpan;
      matSpan.startIndex = 0;
      matSpan.count = indexBuffer->getIndexCount();
      matSpan.material = meshData.material;
      mesh->getMaterials().push_back(std::move(matSpan));

      model->addMesh(std::move(mesh));
    }

    return model;
  }

  void GltfResource::load()
  {
    CONTRACT_EXPECT(d_model == nullptr, "Model already loaded");

    const auto gltfModel = loadModelFromFile(d_filePath);

    // Meshes hang off the scene graph, so walking it is what applies each node's transform.
    const bool hasScene = gltfModel.defaultScene >= 0 && !gltfModel.scenes[gltfModel.defaultScene].nodes.empty();

    std::unordered_map<int, std::shared_ptr<MemoryTexture>> imageCache;

    if (hasScene)
    {
      for (const int nodeIndex : gltfModel.scenes[gltfModel.defaultScene].nodes)
        collectNodeMeshes(gltfModel, nodeIndex, Matrix::Identity, d_meshData, imageCache);
    }
    else
    {
      for (const int nodeIndex : findRootNodes(gltfModel))
        collectNodeMeshes(gltfModel, nodeIndex, Matrix::Identity, d_meshData, imageCache);
    }

    d_model = buildModel(d_meshData);
  }

  std::shared_ptr<IModel> GltfResource::makeTransformedModel(const ModelTransform& i_transform) const
  {
    CONTRACT_EXPECT(!d_meshData.empty(), "Model is not loaded");

    // Offset first so it lands in model space, before the scale and the yaw rotate it away.
    const auto matrix =
      Matrix::CreateTranslation(i_transform.offset.x, i_transform.offset.y, i_transform.offset.z) *
      Matrix::CreateScale(i_transform.scale) *
      Matrix::CreateRotationY(i_transform.yaw);

    auto meshData = d_meshData;
    for (auto& mesh : meshData)
      applyTransform(mesh.vertices, matrix);

    return buildModel(meshData);
  }

  void GltfResource::unload()
  {
    d_model.reset();
    d_meshData.clear();
  }

} // ns Dx
