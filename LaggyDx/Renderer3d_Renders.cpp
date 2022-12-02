#include "stdafx.h"
#include "Renderer3d.h"

#include "AnimationController3d.h"
#include "ICamera.h"
#include "IndexBuffer.h"
#include "MeshResourceCmo.h"
#include "RenderDevice.h"
#include "ResourceController.h"
#include "Texture.h"
#include "VertexBuffer.h"


namespace Dx
{
  void Renderer3d::renderObject(
    const ITexture& i_texture,
    const VertexBuffer& i_vertexBuffer, const IndexBuffer& i_indexBuffer,
    const std::vector<MaterialSpan>& i_materialSpans,
    const Sdk::Vector3F& i_position, const Sdk::Vector3F& i_rotation)
  {
    setBuffers(
      i_vertexBuffer.getPtr(), i_indexBuffer.getPtr(),
      unsigned int(i_vertexBuffer.getStride()));


    setShaderMatrices(i_position, i_rotation);
    setShaderTexture(i_texture.getTexturePtr());


    for (auto& materialSpan : i_materialSpans)
      drawMaterial(materialSpan);
  }


  void Renderer3d::renderObject(
    const IMeshResourceCmo& i_meshCmoResource, const ITexture* i_texture,
    std::shared_ptr<IAnimationController3d> i_animationController,
    const Sdk::Vector3F& i_position, const Sdk::Vector3F& i_rotation, const Sdk::Vector3F& i_scale,
    bool i_useLighting)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(d_renderDevice);
    const auto& meshResourceCmo = dynamic_cast<const MeshResourceCmo&>(i_meshCmoResource);

    auto animationTransform = XMMatrixIdentity();
    auto animationController = std::dynamic_pointer_cast<AnimationController3d>(i_animationController);

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

      if (i_texture)
      {
        if (auto* dgslEffect = dynamic_cast<DGSLEffect*>(io_pEffect))
        {
          dgslEffect->SetTexture(i_texture->getTexturePtr());
          dgslEffect->SetTextureEnabled(true);
        }
      }
    });

    meshResourceCmo.getModel().Draw(renderDevice.getDeviceContextPtr(), *d_commonStates,
      worldMatrix, d_camera.getViewMatrix(), d_camera.getProjectionMatrix());
  }

} // ns Dx
