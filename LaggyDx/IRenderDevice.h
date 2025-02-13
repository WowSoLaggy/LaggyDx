#pragma once

#include "LaggyDxFwd.h"

#include <LaggySdk/ILockable.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class IRenderDevice : public Sdk::ILockable
  {
  public:
    static std::unique_ptr<IRenderDevice> create(
      HWND i_hWnd, Sdk::Vector2I i_resolution, const int i_refreshRate, bool i_debugMode);

  public:
    virtual ~IRenderDevice() = default;

    virtual void begin() = 0;
    virtual void end() = 0;

    virtual const Sdk::Vector2I& getResolution() const = 0;
    virtual void setClearColor(const Sdk::Vector4F& i_clearColor) = 0;
    virtual void resetState() = 0;
    virtual void resetShaders() = 0;
    
    virtual void prepareDepthBufferTexture() const = 0;
    virtual const ITexture& getDepthBufferTexture() const = 0;
    virtual void bindDepthBuffer() = 0;
    virtual void unbindDepthBuffer() = 0;
  };

} // ns Dx
