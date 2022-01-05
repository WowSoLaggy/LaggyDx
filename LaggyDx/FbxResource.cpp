#include "stdafx.h"
#include "FbxResource.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "ofbx.h"

#include <LaggySdk/Files.h>


namespace Dx
{
  namespace
  {
    std::vector<VertexTypePosTexNorm> getVertices(const ofbx::Geometry& i_geometry)
    {
      const int vertexCount = i_geometry.getVertexCount();
      const auto* vertices = i_geometry.getVertices();
      const auto* normals = i_geometry.getNormals();
      const auto* uvs = i_geometry.getUVs();

      std::vector<VertexTypePosTexNorm> verts;
      verts.reserve(vertexCount);
      for (int i = 0; i < vertexCount; ++i)
      {
        VertexTypePosTexNorm vert;
        vert.position = { (float)vertices[i].x, (float)vertices[i].y, (float)vertices[i].z };
        vert.normal = { (float)normals[i].x, (float)normals[i].y, (float)normals[i].z };
        vert.texture = { (float)uvs[i].x, (float)uvs[i].y };
        verts.push_back(std::move(vert));
      }

      return verts;
    }

    std::vector<int> getIndices(const ofbx::Geometry& i_geometry)
    {
      const int indexCount = i_geometry.getIndexCount();
      const auto* incides = i_geometry.getFaceIndices();
      std::vector<int> inds(incides, incides + indexCount);

      // Decode indices
      // Negative ones mark end of triangle.
      // I don't need it, so I have to decode them
      for (int& index : inds)
      {
        if (index < 0)
          index = -index - 1;
      }

      return inds;
    }

    MaterialSequence getMaterialsFromMesh(const ofbx::Mesh& i_mesh)
    {
      auto toColor = [](const ofbx::Color& i_color)
      {
        return Sdk::Vector4F(i_color.r, i_color.g, i_color.b, 1.0f);
      };

      const auto* geometry = i_mesh.getGeometry();
      CONTRACT_ASSERT(geometry);

      const int* materials = geometry->getMaterials();
      if (!materials)
      {
        MaterialSpan matSpan;
        matSpan.startIndex = 0;
        matSpan.count = geometry->getIndexCount();

        matSpan.material = Material::getDefault();
        if (i_mesh.getMaterialCount() != 0)
        {
          const auto* meshMaterial = i_mesh.getMaterial(0);
          matSpan.material.ambientColor = toColor(meshMaterial->getAmbientColor());
          matSpan.material.diffuseColor = toColor(meshMaterial->getDiffuseColor());
        }

        MaterialSequence matSeq;
        matSeq.add(matSpan);
        return matSeq;
      }

      CONTRACT_ASSERT(false);
    }

  } // anonym NS


  FbxResource::FbxResource(fs::path i_meshFilePath)
    : d_filePath(std::move(i_meshFilePath))
  {
  }

  const VertexBuffer& FbxResource::getVertexBuffer() const
  {
    CONTRACT_ASSERT(d_vertexBuffer);
    return *d_vertexBuffer;
  }
  const IndexBuffer& FbxResource::getIndexBuffer() const
  {
    CONTRACT_ASSERT(d_indexBuffer);
    return *d_indexBuffer;
  }
  const IMaterialSequence& FbxResource::getMaterials() const
  {
    return d_materials;
  }

  void FbxResource::load(IRenderDevice& i_renderDevice)
  {
    const auto buffer = Sdk::readBytes(d_filePath);
    auto* scene = ofbx::load(buffer.data(), (int)buffer.size(), (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
    CONTRACT_ASSERT(scene);
    CONTRACT_ASSERT(scene->getMeshCount() == 1);

    const auto* mesh = scene->getMesh(0);
    CONTRACT_ASSERT(mesh);

    const auto* geometry = mesh->getGeometry();
    CONTRACT_ASSERT(geometry);
    
    d_vertexBuffer = std::make_unique<VertexBuffer>(i_renderDevice, getVertices(*geometry));
    d_indexBuffer = std::make_unique<IndexBuffer>(i_renderDevice, getIndices(*geometry));

    d_materials = getMaterialsFromMesh(*mesh);

    scene->destroy();
  }

  void FbxResource::unload()
  {
  }

} // ns Dx
