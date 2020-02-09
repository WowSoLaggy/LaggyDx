#pragma once

#include <Sdk/ILockable.h>


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
    static std::unique_ptr<IRenderDevice> create(HWND i_hWnd, int i_resolutionX, int i_resolutionY);

  public:
    virtual ~IRenderDevice() = default;

    virtual void beginScene() = 0;
    virtual void endScene() = 0;

    virtual void switchFillMode() = 0;
    virtual void setFillMode(FillMode i_fillMode) = 0;
  };

} // ns Dx
