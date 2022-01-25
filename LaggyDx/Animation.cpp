#include "stdafx.h"
#include "Animation.h"

#include "ofbx.h"


namespace Dx
{
  namespace
  {
    AnimCurve importCurve(const ofbx::AnimationCurve& i_curve)
    {
      AnimCurve curve;

      if (i_curve.getKeyCount() > 0)
      {
        const double start = ofbx::fbxTimeToSeconds(i_curve.getKeyTime()[0]);
        const double end = ofbx::fbxTimeToSeconds(i_curve.getKeyTime()[i_curve.getKeyCount() - 1]);
        curve.length = end - start;
      }

      for (int i = 0; i < i_curve.getKeyCount(); ++i)
      {
        auto& key = curve.keys.emplace_back();

        key.time = ofbx::fbxTimeToSeconds(i_curve.getKeyTime()[i]);
        key.value = i_curve.getKeyValue()[i];
      }

      return curve;
    }

    AnimCurveNode importCurveNode(const ofbx::AnimationCurveNode& i_curveNode)
    {
      AnimCurveNode node;

      for (int i = 0; i < 3; ++i)
      {
        if (const auto* curve = i_curveNode.getCurve(i))
          node.curves[i] = importCurve(*curve);
      }

      return node;
    }

    AnimLayer importLayer(const ofbx::AnimationLayer& i_layer)
    {
      AnimLayer layer;

      for (int i = 0; i_layer.getCurveNode(i); ++i)
      {
        const auto* curveNode = i_layer.getCurveNode(i);
        CONTRACT_ASSERT(curveNode);
        layer.curveNodes.push_back(importCurveNode(*curveNode));
      }

      return layer;
    }

    std::string getAnimationName(const char i_name[128])
    {
      std::string name = i_name;

      const auto it = std::find(name.rbegin(), name.rend(), '|');
      if (it == name.rend())
        return name;

      name = std::string(name.rbegin(), it); // crop after '|' in reverse order (e.g. "eldi")
      name = std::string(name.rbegin(), name.rend()); // reverse to normal (e.g. "idle")

      return name;
    }

    Animation importAnimation(const ofbx::AnimationStack& i_stack)
    {
      Animation anim;

      for (int i = 0; i_stack.getLayer(i); ++i)
      {
        const auto* layer = i_stack.getLayer(i);
        CONTRACT_ASSERT(layer);
        anim.layers.push_back(importLayer(*layer));
      }

      anim.name = getAnimationName(i_stack.name);
      anim.updateLength();

      return anim;
    }

    std::optional<Sdk::Vector3F> getTransform(
      const Animation& i_animation,
      const int i_curveNodeIndex,
      const double i_time)
    {
      if (!i_animation.layers.empty())
      {
        if (!i_animation.layers.front().curveNodes.empty())
          return i_animation.layers.front().curveNodes.at(i_curveNodeIndex).getTransform(i_time);
      }

      return std::nullopt;
    }

  } // anonym NS


  AnimationsMap importFromFbx(const ofbx::IScene& i_scene)
  {
    AnimationsMap animations;

    for (int i = 0; i < i_scene.getAnimationStackCount(); ++i)
    {
      const auto* stack = i_scene.getAnimationStack(i);
      CONTRACT_ASSERT(stack);

      auto animation = importAnimation(*stack);
      animations[animation.name] = std::move(animation);
    }

    return animations;
  }


  float AnimCurve::getValue(const double i_time) const
  {
    if (keys.empty())
      return 0.0f;
    if (i_time < keys[0].time)
      return keys[0].value;

    for (int i = 1; i < keys.size(); ++i)
    {
      if (i_time == keys[i].time)
        return keys[i].value;
      else if (i_time < keys[i].time)
      {
        const double part = (i_time - keys[i - 1].time) / (keys[i].time - keys[i - 1].time);
        return float(keys[i - 1].value * (1 - part) + keys[i].value * part);
      }
    }

    return keys.back().value;
  }


  Sdk::Vector3F AnimCurveNode::getTransform(const double i_time) const
  {
    return { curves[0].getValue(i_time), curves[1].getValue(i_time), curves[2].getValue(i_time) };
  }

  double AnimCurveNode::getLength() const
  {
    return std::max(curves[0].length, std::max(curves[1].length, curves[2].length));
  }


  double AnimLayer::getLength() const
  {
    if (curveNodes.empty())
      return 0;

    double maxLength = curveNodes.front().getLength();
    for (int i = 1; i < curveNodes.size(); ++i)
    {
      const double length = curveNodes[i].getLength();
      if (length > maxLength)
        maxLength = length;
    }

    return maxLength;
  }


  void Animation::updateLength()
  {
    if (layers.empty())
    {
      length = 0.0;
      return;
    }

    double maxLength = layers.front().getLength();
    for (int i = 1; i < layers.size(); ++i)
    {
      const double length = layers[i].getLength();
      if (length > maxLength)
        maxLength = length;
    }

    length = maxLength;
  }

  Sdk::Vector3F Animation::getTranslation(const double i_time) const
  {
    const auto xForm = getTransform(*this, 0, i_time);
    return xForm.value_or(Sdk::Vector3F::zero());
  }

  Sdk::Vector3F Animation::getRotation(const double i_time) const
  {
    const auto xForm = getTransform(*this, 1, i_time);
    return xForm.value_or(Sdk::Vector3F::zero());
  }

  Sdk::Vector3F Animation::getScale(const double i_time) const
  {
    const auto xForm = getTransform(*this, 2, i_time);
    return xForm.value_or(Sdk::Vector3F::identity());
  }

} // ns Dx
