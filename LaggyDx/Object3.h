#pragma once

#include "IndexBuffer.h"
#include "IObject3.h"
#include "MaterialSequence.h"
#include "VertexBuffer.h"

namespace Dx
{
  class Object3 : public IObject3
  {
  public:
    virtual Sdk::Vector3F getPosition() const override;
    virtual Sdk::Vector3F getRotation() const override;
    virtual Sdk::Vector3F getScale() const override;
    void setPosition(Sdk::Vector3F i_position);
    void setRotation(Sdk::Vector3F i_rotation);
    void setScale(Sdk::Vector3F i_scale);

    virtual const VertexBuffer& getVertexBuffer() const override;
    virtual const IndexBuffer& getIndexBuffer() const override;
    virtual const IMaterialSequence& getMaterials() const override;
    virtual const ITextureResource& getTextureResource() const override;
    void setFbxResource(const IFbxResource& i_fbxResource);
    void setTextureResource(const ITextureResource& i_textureResource);

  private:
    Sdk::Vector3F d_position;
    Sdk::Vector3F d_rotation;
    Sdk::Vector3F d_scale = Sdk::Vector3F::identity();

    const IFbxResource* d_fbxResource = nullptr;
    const ITextureResource* d_textureResource = nullptr;
  };

} // ns Dx
