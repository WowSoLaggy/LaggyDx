#include "stdafx.h"
#include "Object3.h"

#include "FbxResource.h"


namespace Dx
{
  const Sdk::Vector3F& Object3::getPosition() const { return d_position; }
  const Sdk::Vector3F& Object3::getRotation() const { return d_rotation; }
  const Sdk::Vector3F& Object3::getScale() const { return d_scale; }
  void Object3::setPosition(Sdk::Vector3F i_position) { d_position = std::move(i_position); }
  void Object3::setRotation(Sdk::Vector3F i_rotation) { d_rotation = std::move(i_rotation); }
  void Object3::setScale(Sdk::Vector3F i_scale) { d_scale = std::move(i_scale); }


  const VertexBuffer& Object3::getVertexBuffer() const
  {
    const auto* fbxResource = dynamic_cast<const FbxResource*>(d_fbxResource);
    CONTRACT_ASSERT(fbxResource);

    return fbxResource->getVertexBuffer();
  }
  const IndexBuffer& Object3::getIndexBuffer() const
  {
    const auto* fbxResource = dynamic_cast<const FbxResource*>(d_fbxResource);
    CONTRACT_ASSERT(fbxResource);

    return fbxResource->getIndexBuffer();
  }
  const IMaterialSequence& Object3::getMaterials() const
  {
    const auto* fbxResource = dynamic_cast<const FbxResource*>(d_fbxResource);
    CONTRACT_ASSERT(fbxResource);

    return fbxResource->getMaterials();
  }
  const ITextureResource& Object3::getTextureResource() const
  {
    CONTRACT_ASSERT(d_textureResource);
    return *d_textureResource;
  }
  void Object3::setFbxResource(const IFbxResource& i_fbxResource) { d_fbxResource = &i_fbxResource; }
  void Object3::setTextureResource(const ITextureResource& i_textureResource) { d_textureResource = &i_textureResource; }

} // ns Dx
