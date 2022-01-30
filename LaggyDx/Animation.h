#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  struct Key
  {
    double time = 0.0;
    float value = 0.0f;
  };

  struct AnimCurve
  {
    std::vector<Key> keys;
    double length = 0.0;

    float getValue(double i_time) const;
  };

  struct AnimCurveNode
  {
    std::array<AnimCurve, 3> curves;

    Sdk::Vector3F getTransform(double i_time) const;
    double getLength() const;
  };

  struct AnimLayer
  {
    std::vector<AnimCurveNode> curveNodes;
    double getLength() const;
  };

  struct Animation
  {
    std::string name;
    std::vector<AnimLayer> layers;
    double length = 0.0;

    void updateLength();

    Sdk::Vector3F getTranslation(double i_time) const;
    Sdk::Vector3F getRotation(double i_time) const;
    Sdk::Vector3F getScale(double i_time) const;
  };

  using AnimationsMap = std::unordered_map<std::string, Animation>;

  AnimationsMap importAnimationsFromFbx(const ofbx::IScene& i_scene);

} // ns Dx
