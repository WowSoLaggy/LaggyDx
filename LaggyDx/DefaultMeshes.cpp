#include "stdafx.h"
#include "DefaultMeshes.h"

#include "App.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "VertexTypes.h"


namespace Dx
{
  std::unique_ptr<IMesh> DefaultMeshes::rectangle(float i_width, float i_height)
  {
    const std::vector<VertexPos2Text> points{
      VertexPos2Text::posTex({ 0, 0 }, { 0, 0 }),
      VertexPos2Text::posTex({ i_width, 0 }, { 1, 0 }),
      VertexPos2Text::posTex({ i_width, i_height }, { 1, 1 }),
      VertexPos2Text::posTex({ 0, i_height }, { 0, 1 }),
    };

    const std::vector<int> inds{ 0, 2, 1, 0, 3, 2 };

    auto mesh = std::make_unique<Mesh>();
    mesh->setVertexBuffer(std::make_unique<VertexBuffer>(App::get().getRenderDevice(), points));
    mesh->setIndexBuffer(std::make_unique<IndexBuffer>(App::get().getRenderDevice(), inds));
    return mesh;
  }

} // Dx
