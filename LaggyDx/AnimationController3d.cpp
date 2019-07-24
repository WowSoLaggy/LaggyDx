#include "stdafx.h"
#include "AnimationController3d.h"

#include "MeshResourceCmo.h"
#include "ResourceController.h"


namespace Dx
{
  AnimationController3d::AnimationController3d(const IMeshResourceCmo& i_model)
    : d_model(dynamic_cast<const MeshResourceCmo&>(i_model).getModel())
    , d_currentAnimationName(L"")
    , d_animationTime(0)
  {
    for (auto& modelMeshPtr : d_model.meshes)
    {
      BoneTransformCollection boneTransformCollection(modelMeshPtr->bones.size());
      d_meshesBoneAnimationXfms.push_back(boneTransformCollection);
      d_meshesBoneCombinedXfms.push_back(boneTransformCollection);
    }

    resetAnimationXfms();
  }


  void AnimationController3d::setAnimation(std::wstring i_animationName)
  {
    d_currentAnimationName = std::move(i_animationName);
    d_animationTime = 0;

    resetAnimationXfms();
  }


  int AnimationController3d::getBoneXfmsCount() const
  {
    return (int)d_meshesBoneCombinedXfms.front().size();
  }

  const XMMATRIX* AnimationController3d::getBoneXfms() const
  {
    return d_meshesBoneCombinedXfms.front().data();
  }


  void AnimationController3d::update(double i_dt)
  {
    d_animationTime += i_dt;

    updateAnimationXfms();
    updateCombinedXfms();
  }


  void AnimationController3d::resetAnimationXfms()
  {
    for (int meshIndex = 0; meshIndex < d_model.meshes.size(); ++meshIndex)
    {
      const auto& mesh = d_model.meshes.at(meshIndex);
      for (int boneIndex = 0; boneIndex < (int)mesh->bones.size(); ++boneIndex)
        d_meshesBoneAnimationXfms.at(meshIndex).at(boneIndex) = XMMatrixIdentity();
    }
  }


  void AnimationController3d::updateAnimationXfms()
  {
    resetAnimationXfms();

    for (int meshIndex = 0; meshIndex < d_model.meshes.size(); ++meshIndex)
    {
      const auto& mesh = d_model.meshes.at(meshIndex);

      auto itAnim = mesh->animClipMap.find(d_currentAnimationName);
      if (itAnim == mesh->animClipMap.end())
        continue;

      double animLength = itAnim->second.EndTime - itAnim->second.StartTime;
      double time = d_animationTime;
      while (time > animLength)
        time -= animLength;

      for (const auto& keyframe : itAnim->second.Keyframes)
      {
        d_meshesBoneAnimationXfms.at(meshIndex).at(keyframe.BoneIndex) = keyframe.Transform;

        if (keyframe.Time >= time)
          break;
      }
    }
  }

  void AnimationController3d::updateCombinedXfms()
  {
    for (int meshIndex = 0; meshIndex < d_model.meshes.size(); ++meshIndex)
    {
      const auto& mesh = *d_model.meshes.at(meshIndex);
      for (int boneIndex = 0; boneIndex < mesh.bones.size(); ++boneIndex)
      {
        const auto& bone = mesh.bones.at(boneIndex);
        auto& animationXfm = d_meshesBoneAnimationXfms.at(meshIndex).at(boneIndex);

        auto combinedXfm = bone.InvBindPos * animationXfm;
        int parentId = bone.ParentIndex;
        while (parentId != -1)
        {
          const auto& parentAnimationXfm = d_meshesBoneAnimationXfms.at(meshIndex).at(parentId);
          combinedXfm = combinedXfm * parentAnimationXfm;
          parentId = mesh.bones.at(parentId).ParentIndex;
        }

        d_meshesBoneCombinedXfms.at(meshIndex).at(boneIndex) = combinedXfm;
      }
    }
  }

} // ns Dx
