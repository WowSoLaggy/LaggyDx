#pragma once

#include <LaggySdk/ILockable.h>
#include <LaggySdk/Vector.h>


namespace Dx
{
  enum class FillMode
  {
    Solid,
    Wire,
  };


  class IRenderDevice : public Sdk::ILockable
  {
  public:
    static std::unique_ptr<IRenderDevice> create(HWND i_hWnd, Sdk::Vector2I i_resolution);

  public:
    virtual ~IRenderDevice() = default;

    virtual void beginScene() = 0;
    virtual void endScene() = 0;

    virtual void switchFillMode() = 0;
    virtual void setFillMode(FillMode i_fillMode) = 0;

    virtual void setClearColor(std::array<float, 4> i_clearColor) = 0;
  };

} // ns Dx
