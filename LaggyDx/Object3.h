#pragma once

#include "IndexBuffer.h"
#include "IObject3.h"
#include "VertexBuffer.h"

namespace Dx
{
  class Object3 : public IObject3
  {
  public:
    virtual Sdk::Vector3F getPosition() const override;
    virtual Sdk::Vector3F getRotation() const override;
    virtual Sdk::Vector3F getScale() const override;
    virtual void setPosition(Sdk::Vector3F i_position) override;
    virtual void setRotation(Sdk::Vector3F i_rotation) override;
    virtual void setScale(Sdk::Vector3F i_scale) override;

    virtual const IModel& getModel() const override;
    virtual const ITextureResource* getTextureResource() const override;
    
    virtual void setModel(std::shared_ptr<IModel> i_model) override;
    virtual void setTextureResource(const ITextureResource& i_textureResource) override;

  private:
    Sdk::Vector3F d_position;
    Sdk::Vector3F d_rotation;
    Sdk::Vector3F d_scale = Sdk::Vector3F::identity();

    std::shared_ptr<IModel> d_model = nullptr;
    const ITextureResource* d_textureResource = nullptr;
  };

} // ns Dx
