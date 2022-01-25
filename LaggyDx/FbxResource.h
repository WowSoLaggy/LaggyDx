#pragma once

#include "Animation.h"
#include "IFbxResource.h"
#include "IndexBuffer.h"
#include "LaggyDxFwd.h"
#include "MaterialSequence.h"
#include "VertexBuffer.h"


namespace Dx
{
  class FbxResource : public IFbxResource
  {
  public:
    FbxResource(fs::path i_filePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    const VertexBuffer& getVertexBuffer() const;
    const IndexBuffer& getIndexBuffer() const;
    const IMaterialSequence& getMaterials() const;
    const AnimationsMap& getAnimations() const;

  private:
    const fs::path d_filePath;

    std::unique_ptr<VertexBuffer> d_vertexBuffer;
    std::unique_ptr<IndexBuffer> d_indexBuffer;

    MaterialSequence d_materials;
    AnimationsMap d_animations;
  };
} // ns Dx
