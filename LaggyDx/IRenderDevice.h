#pragma once

#include <LaggySdk/ILockable.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  class IRenderDevice : public Sdk::ILockable
  {
  public:
    static std::unique_ptr<IRenderDevice> create(HWND i_hWnd, Sdk::Vector2I i_resolution);

  public:
    virtual ~IRenderDevice() = default;

    virtual void beginScene() = 0;
    virtual void endScene() = 0;

    /// i_solid == true - solid
    /// i_solid == false - wireframe
    virtual void setFillMode(bool i_solid) = 0;

    virtual void setClearColor(const Sdk::Vector4F& i_clearColor) = 0;

    virtual const Sdk::Vector2I& getResolution() const = 0;

    virtual void resetState() = 0;
  };

} // ns Dx
