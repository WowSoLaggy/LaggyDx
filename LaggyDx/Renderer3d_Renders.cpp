#include "stdafx.h"
#include "Renderer3d.h"

#include "AnimationController.h"
#include "Camera.h"
#include "IndexBuffer.h"
#include "MaterialSequence.h"
#include "MeshResourceCmo.h"
#include "RenderDevice.h"
#include "ResourceController.h"
#include "TextureResource.h"
#include "VertexBuffer.h"


namespace Dx
{
  void Renderer3d::renderObject(
    const ITextureResource& i_textureResource,
    const VertexBuffer& i_vertexBuffer, const IndexBuffer& i_indexBuffer,
    const std::vector<MaterialSpan>& i_materialSpans,
    const Sdk::Vector3& i_position, const Sdk::Vector3& i_rotation)
  {
    const auto& textureResource = dynamic_cast<const TextureResource&>(i_textureResource);

    setBuffers(
      i_vertexBuffer.getPtr(), i_indexBuffer.getPtr(),
      unsigned int(i_vertexBuffer.getStride()));


    setShaderMatrices(i_position, i_rotation);
    setShaderTexture(textureResource.getTexturePtr());


    for (auto& materialSpan : i_materialSpans)
      drawMaterial(materialSpan);
  }


  void Renderer3d::renderObject(
    const IMeshResourceCmo& i_meshCmoResource, const ITextureResource* i_textureResource,
    std::shared_ptr<IAnimationController> i_animationController,
    const Sdk::Vector3& i_position, const Sdk::Vector3& i_rotation, const Sdk::Vector3& i_scale,
    bool i_useLighting)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);
    const auto& meshResourceCmo = dynamic_cast<const MeshResourceCmo&>(i_meshCmoResource);

    auto& camera = dynamic_cast<const Camera&>(d_camera);

    auto animationTransform = XMMatrixIdentity();
    auto animationController = std::dynamic_pointer_cast<AnimationController>(i_animationController);

    auto worldMatrix =
      XMMatrixScaling(i_scale.x, i_scale.y, i_scale.z) *
      XMMatrixRotationRollPitchYaw(i_rotation.x, i_rotation.y, i_rotation.z) *
      XMMatrixTranslation(i_position.x, i_position.y, i_position.z);

    meshResourceCmo.getModel().UpdateEffects([&](IEffect* io_pEffect)
    {
      if (auto* pLights = dynamic_cast<IEffectLights*>(io_pEffect))
      {
        pLights->SetLightingEnabled(i_useLighting);

        if (i_useLighting)
        {
          const float ambientMultiplier = 0.3f;
          pLights->SetAmbientLightColor({ ambientMultiplier, ambientMultiplier, ambientMultiplier });

          pLights->SetLightEnabled(0, true);
          pLights->SetLightDiffuseColor(0, { 1, 1, 1 });
          pLights->SetLightDirection(0, { 1, -1, -1 });

          pLights->SetLightEnabled(1, false);
          pLights->SetLightEnabled(2, false);
        }
      }

      if (auto* pSkinning = dynamic_cast<IEffectSkinning*>(io_pEffect);
        pSkinning && animationController && animationController->getBoneXfmsCount() > 0)
      {
        pSkinning->SetBoneTransforms(animationController->getBoneXfms(), animationController->getBoneXfmsCount());
      }

      if (i_textureResource)
      {
        if (auto* dgslEffect = dynamic_cast<DGSLEffect*>(io_pEffect))
        {
          const auto& textureResource = dynamic_cast<const TextureResource&>(*i_textureResource);
          auto texture = textureResource.getTexturePtr();
          dgslEffect->SetTexture(texture);
          dgslEffect->SetTextureEnabled(true);
        }
      }
    });

    meshResourceCmo.getModel().Draw(renderDevice.getDeviceContextPtr(), *d_commonStates,
      worldMatrix, camera.getViewMatrix(), camera.getProjectionMatrix());
  }

} // ns Dx
