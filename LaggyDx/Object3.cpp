#include "stdafx.h"
#include "Object3.h"

#include "TextureUtils.h"


namespace Dx
{
  Sdk::Vector3F Object3::getPosition() const { return d_position; }
  Sdk::Vector3F Object3::getRotation() const { return d_rotation; }
  Sdk::Vector3F Object3::getScale() const { return d_scale; }
  void Object3::setPosition(Sdk::Vector3F i_position) { d_position = std::move(i_position); }
  void Object3::setRotation(Sdk::Vector3F i_rotation) { d_rotation = std::move(i_rotation); }
  void Object3::setScale(Sdk::Vector3F i_scale) { d_scale = std::move(i_scale); }

  void Object3::setVisible(const bool i_visible) { d_isVisible = i_visible; }
  bool Object3::getVisible() const { return d_isVisible; }

  const IModel& Object3::getModel() const
  {
    CONTRACT_ASSERT(d_model);
    return *d_model;
  }

  const ITexture* Object3::getTexture() const
  {
    return d_texture;
  }

  void Object3::setModel(std::shared_ptr<IModel> i_model) { d_model = i_model; }
  void Object3::setTexture(const ITexture& i_texture) { d_texture = &i_texture; }
  void Object3::setTexture(const std::string& i_textureName) { d_texture = &TextureUtils::getTexture(i_textureName); }

} // ns Dx
