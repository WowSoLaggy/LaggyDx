#pragma once


namespace Dx
{
  enum class FillMode
  {
    Solid,
    Wire,
  };


  class IRenderDevice
  {
  public:

    static std::shared_ptr<IRenderDevice> create();

  public:

    virtual ~IRenderDevice() = default;

    virtual bool isInitialized() const = 0;

    virtual void initialize(HWND i_hWnd, int i_resolutionX, int i_resolutionY) = 0;
    virtual void dispose() = 0;

    virtual void beginScene() = 0;
    virtual void endScene() = 0;

    virtual void switchFillMode() = 0;
    virtual void setFillMode(FillMode i_fillMode) = 0;
  };

} // ns Dx
