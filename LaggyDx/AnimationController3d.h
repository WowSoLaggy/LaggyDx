#pragma once

#include "IAnimationController3d.h"


namespace Dx
{
  class AnimationController3d : public IAnimationController3d
  {
  public:
    AnimationController3d(const IMeshResourceCmo& i_model);

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
