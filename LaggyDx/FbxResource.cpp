#include "stdafx.h"
#include "FbxResource.h"

#include "IndexBuffer.h"
#include "MeshUtils.h"
#include "OfbxSceneWrapper.h"
#include "Shape3d.h"
#include "VertexBuffer.h"
#include "ofbx.h"


namespace Dx
{
  namespace
  {
    std::vector<VertexTypePosTexNorm> getVertices(
      const ofbx::Geometry& i_geometry,
      const float i_unitScale)
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
        if (i_unitScale != 1.0f)
          vert.position = vert.position / i_unitScale;

        if (normals)
          vert.normal = { (float)normals[i].x, (float)normals[i].y, (float)normals[i].z };
        if (uvs)
          vert.texture = { (float)uvs[i].x, 1.0f - (float)uvs[i].y }; // invert Y due to Blender FBX exporter specific :(
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

    Shape3d getShape(const ofbx::Geometry& i_geometry, const float i_scaleFactor)
    {
      auto verts = getVertices(i_geometry, i_scaleFactor);
      auto inds = getIndices(i_geometry);
      return Shape3d(std::move(verts), std::move(inds));
    }

    MaterialSequence getMaterialsFromMesh(const ofbx::Mesh& i_mesh)
    {
      auto toColor = [](const ofbx::Color& i_color)
      {
        return Sdk::Vector4F(i_color.r, i_color.g, i_color.b, 1.0f);
      };

      const auto* geometry = i_mesh.getGeometry();
      CONTRACT_ASSERT(geometry);

      MaterialSpan matSpan;
      matSpan.startIndex = 0;
      matSpan.count = geometry->getIndexCount();

      if (i_mesh.getMaterialCount() != 0)
      {
        const auto* meshMaterial = i_mesh.getMaterial(0);

        if (const auto* texture = meshMaterial->getTexture(ofbx::Texture::TextureType::DIFFUSE))
        {
          char name[128];
          texture->getFileName().toString(name);
          fs::path filename(name);
          matSpan.material.textureName = filename.filename().string();
        }

        matSpan.material.ambientColor = toColor(meshMaterial->getAmbientColor());
        matSpan.material.diffuseColor = toColor(meshMaterial->getDiffuseColor());
      }

      MaterialSequence matSeq;
      matSeq.add(matSpan);
      return matSeq;
    }

  } // anonym NS


  FbxResource::FbxResource(fs::path i_meshFilePath)
    : d_filePath(std::move(i_meshFilePath))
  {
  }

  const IModel& FbxResource::getModel() const
  {
    CONTRACT_ASSERT(d_model);
    return *d_model;
  }

  void FbxResource::load(IRenderDevice& i_renderDevice)
  {
    OfbxSceneWrapper scene(d_filePath);

    Model model;

    for (int meshIndex = 0; meshIndex < scene->getMeshCount(); ++meshIndex)
    {
      const auto* fbxMesh = scene->getMesh(meshIndex);
      CONTRACT_ASSERT(fbxMesh);

      const auto* geometry = fbxMesh->getGeometry();
      CONTRACT_ASSERT(geometry);

      const auto shape = getShape(*geometry, scene->getGlobalSettings()->UnitScaleFactor);
      auto mesh = createMeshFromShape(shape, i_renderDevice);

      mesh.setMaterials(std::make_unique<MaterialSequence>(getMaterialsFromMesh(*fbxMesh)));

      model.addMesh(std::move(mesh));
    }

    model.setAnimations(importAnimationsFromFbx(*scene));
    model.createAabbMesh(i_renderDevice);

    d_model = std::make_unique<Model>(std::move(model));
  }

  void FbxResource::unload()
  {
    d_model.reset();
  }

} // ns Dx
