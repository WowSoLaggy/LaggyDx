#include "stdafx.h"
#include "AnimationController.h"

#include "MeshResourceCmo.h"
#include "ResourceController.h"


AnimationController::AnimationController(const IResourceController& i_resourceController, ResourceId i_resourceId)
  : d_model(
      dynamic_cast<const ResourceController&>(i_resourceController).getMeshResourceCmo(i_resourceId).getModel())
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


void AnimationController::setAnimation(std::wstring i_animationName)
{
  d_currentAnimationName = std::move(i_animationName);
  d_animationTime = 0;

  resetAnimationXfms();
}


int AnimationController::getBoneXfmsCount() const
{
  return (int)d_meshesBoneCombinedXfms.front().size();
}

const XMMATRIX* AnimationController::getBoneXfms() const
{
  return d_meshesBoneCombinedXfms.front().data();
}


void AnimationController::update(double i_dt)
{
  d_animationTime += i_dt;

  updateAnimationXfms();
  updateCombinedXfms();
}


void AnimationController::resetAnimationXfms()
{
  for (int meshIndex = 0; meshIndex < d_model.meshes.size(); ++meshIndex)
  {
    const auto& mesh = d_model.meshes.at(meshIndex);
    for (int boneIndex = 0; boneIndex < (int)mesh->bones.size(); ++boneIndex)
      d_meshesBoneAnimationXfms.at(meshIndex).at(boneIndex) = XMMatrixIdentity();
  }
}


void AnimationController::updateAnimationXfms()
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

void AnimationController::updateCombinedXfms()
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
