#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  class ISkydomeShader
  {
  public:
    static std::unique_ptr<ISkydomeShader> create(
      IRenderDevice& i_renderDevice,
      const ICamera& i_camera,
      const IResourceController& i_resourceController);

  public:
    virtual ~ISkydomeShader() = default;

    virtual void setZeroLevelColor(const Sdk::Vector4F& i_color) = 0;
    virtual void setTopLevelColor(const Sdk::Vector4F& i_color) = 0;

    virtual void draw(const IObject3& i_object) const = 0;
  };

} // ns Dx
