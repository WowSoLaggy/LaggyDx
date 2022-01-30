#pragma once

#include "IMeshResourceCmo.h"


namespace Dx
{
  class MeshResourceCmo : public IMeshResourceCmo
  {
  public:
    MeshResourceCmo(fs::path i_meshFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    DirectX::Model& getModel() const { return *d_model; }

  private:
    const fs::path d_meshFilePath;
    std::shared_ptr<DirectX::Model> d_model;
  };
} // ns Dx
