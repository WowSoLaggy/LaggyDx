#include "stdafx.h"
#include "Object3.h"


namespace Dx
{
  Sdk::Vector3F Object3::getPosition() const { return d_position; }
  Sdk::Vector3F Object3::getRotation() const { return d_rotation; }
  Sdk::Vector3F Object3::getScale() const { return d_scale; }
  void Object3::setPosition(Sdk::Vector3F i_position) { d_position = std::move(i_position); }
  void Object3::setRotation(Sdk::Vector3F i_rotation) { d_rotation = std::move(i_rotation); }
  void Object3::setScale(Sdk::Vector3F i_scale) { d_scale = std::move(i_scale); }


  const IModel& Object3::getModel() const
  {
    CONTRACT_ASSERT(d_model);
    return *d_model;
  }

  const ITextureResource* Object3::getTextureResource() const
  {
    return d_textureResource;
  }

  void Object3::setModel(const IModel& i_model) { d_model = &i_model; }
  void Object3::setTextureResource(const ITextureResource& i_textureResource) { d_textureResource = &i_textureResource; }

} // ns Dx
