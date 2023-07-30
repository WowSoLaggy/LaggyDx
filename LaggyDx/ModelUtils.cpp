#include "stdafx.h"
#include "ModelUtils.h"

#include "Aabb.h"
#include "IShape2d.h"
#include "IShape3d.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "Model.h"
#include "Object3.h"
#include "VertexBuffer.h"


namespace Dx
{
  std::unique_ptr<IMesh> createMeshFromShape(
    const IShape2d& i_shape, const IRenderDevice& i_renderDevice)
  {
    auto mesh = std::make_unique<Mesh>();
    mesh->setVertexBuffer(std::make_unique<VertexBuffer>(i_renderDevice, i_shape.getVerts()));
    mesh->setIndexBuffer(std::make_unique<IndexBuffer>(i_renderDevice, i_shape.getInds()));

    return mesh;
  }

  std::unique_ptr<IMesh> createMeshFromShape(
    const IShape3d& i_shape, const IRenderDevice& i_renderDevice, const bool i_addDefaultMaterial)
  {
    auto mesh = std::make_unique<Mesh>();
    mesh->setVertexBuffer(std::make_unique<VertexBuffer>(i_renderDevice, i_shape.getVerts()));
    mesh->setIndexBuffer(std::make_unique<IndexBuffer>(i_renderDevice, i_shape.getInds()));
    mesh->setAabb(i_shape.getAabb());

    if (i_addDefaultMaterial)
    {
      MaterialSpan matSpan;
      matSpan.startIndex = 0;
      matSpan.count = (int)i_shape.getInds().size();
      matSpan.material.textureName = "white.png";
      mesh->getMaterials().push_back(std::move(matSpan));
    }

    return mesh;
  }

  std::unique_ptr<IMesh> createMeshFromAabb(const Aabb& i_aabb, const IRenderDevice& i_renderDevice)
  {
    auto mesh = std::make_unique<Mesh>();

    std::vector<VertexPos3NormText> verts;
    std::vector<int> inds;

    auto add = [&](Sdk::Vector3F i_pos)
    {
      VertexPos3NormText vert;
      vert.position = std::move(i_pos);
      verts.push_back(std::move(vert));
    };

    // min Y face

    add({ i_aabb.getMinX(), i_aabb.getMinY(), i_aabb.getMinZ() });
    add({ i_aabb.getMaxX(), i_aabb.getMinY(), i_aabb.getMinZ() });
    add({ i_aabb.getMaxX(), i_aabb.getMinY(), i_aabb.getMaxZ() });
    add({ i_aabb.getMinX(), i_aabb.getMinY(), i_aabb.getMaxZ() });

    // max Y face

    add({ i_aabb.getMaxX(), i_aabb.getMaxY(), i_aabb.getMinZ() });
    add({ i_aabb.getMinX(), i_aabb.getMaxY(), i_aabb.getMinZ() });
    add({ i_aabb.getMinX(), i_aabb.getMaxY(), i_aabb.getMaxZ() });
    add({ i_aabb.getMaxX(), i_aabb.getMaxY(), i_aabb.getMaxZ() });

    // inds

    inds.insert(inds.begin(), {
      0, 1, 1, 2, 2, 3, 3, 0,
      4, 5, 5, 6, 6, 7, 7, 4,
      0, 5, 3, 6, 1, 4, 2, 7
      });

    // misc

    mesh->setVertexBuffer(std::make_unique<VertexBuffer>(i_renderDevice, verts));
    mesh->setIndexBuffer(std::make_unique<IndexBuffer>(i_renderDevice, inds));
    mesh->setAabb(i_aabb);

    MaterialSpan matSpan;
    matSpan.startIndex = 0;
    matSpan.count = (int)inds.size();
    matSpan.material.textureName = "white.png";

    mesh->getMaterials().push_back(std::move(matSpan));

    mesh->setTopology(Topology::LineList);

    return mesh;
  }


  std::unique_ptr<IModel> createModelFromMesh(std::unique_ptr<IMesh> i_mesh)
  {
    auto model = std::make_unique<Model>();
    model->addMesh(std::move(i_mesh));
    return model;
  }


  std::unique_ptr<IObject3> createObjectFromShape(
    const IShape3d& i_shape, const IRenderDevice& i_renderDevice, bool i_addDefaultMaterial)
  {
    auto mesh = createMeshFromShape(i_shape, i_renderDevice, i_addDefaultMaterial);
    return createObjectFromMesh(std::move(mesh));
  }

  std::unique_ptr<IObject3> createObjectFromModel(std::unique_ptr<IModel> i_model)
  {
    auto object = std::make_unique<Object3>();
    object->setModel(std::move(i_model));
    return object;
  }

  std::unique_ptr<IObject3> createObjectFromMesh(std::unique_ptr<IMesh> i_mesh)
  {
    auto object = std::make_unique<Object3>();
    object->setModel(createModelFromMesh(std::move(i_mesh)));
    return object;
  }


  void traverseMaterials(const IModel& i_model, MaterialsVisitor i_visitor)
  {
    for (auto& mesh : i_model.getMeshes())
    {
      for (auto& mat : mesh->getMaterials())
        i_visitor(mat.material);
    }
  }

} // ns Dx
