#pragma once

#include "IAnimationController.h"


namespace Dx
{
  class AnimationController : public IAnimationController
  {
  public:
    AnimationController(const IResourceController& i_resourceController, ResourceId i_resourceId);

    virtual const std::wstring& getCurrentAnimationName() const override { return d_currentAnimationName; }

    virtual void setAnimation(std::wstring i_animationName) override;
    virtual void update(double i_dt) override;

    int getBoneXfmsCount() const;
    const XMMATRIX* getBoneXfms() const;

  private:
    std::wstring d_currentAnimationName;
    double d_animationTime;

    const DirectX::Model& d_model;

    using BoneTransformCollection = std::vector<XMMATRIX>;
    std::vector<BoneTransformCollection> d_meshesBoneAnimationXfms;
    std::vector<BoneTransformCollection> d_meshesBoneCombinedXfms;

    void resetAnimationXfms();

    void updateAnimationXfms();
    void updateCombinedXfms();
  };

} // ns Dx
