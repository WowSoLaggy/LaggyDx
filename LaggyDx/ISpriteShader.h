#pragma once

#include "Shader2d.h"
#include "LaggyDxFwd.h"


namespace Dx
{
  class ISpriteShader : public Shader2d
  {
  public:
    static std::unique_ptr<ISpriteShader> create(const ICamera2* i_camera);

    // Does nothing, added for consistency
    void begin() const {}
    // Draws all the delayed sprites
    virtual void end() const = 0;

    // Delayed mode, supports layering
    // 0 is the lowest layer, 1 is the second layer (above 0), etc.
    virtual void drawDelayed(
      const AnimatedSprite& i_sprite,
      int i_layer = 0,
      const UvOffset* i_uvOffset = nullptr,
      bool i_disableCameraView = false) const = 0;

    // Delayed mode, supports layering
    // 0 is the lowest layer, 1 is the second layer (above 0), etc.
    template <typename T>
    void drawDelayed(
      const AnimatedSprite& i_sprite,
      T i_layer = 0,
      const UvOffset* i_uvOffset = nullptr,
      bool i_disableCameraView = false) const
    {
      drawDelayed(i_sprite, static_cast<int>(i_layer), i_uvOffset, i_disableCameraView);
    }
  };

} // ns Dx
