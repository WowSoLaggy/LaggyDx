#pragma once

#include "ILoadableResource.h"
#include "IMeshResourceCmo.h"


namespace Dx
{
  class MeshResourceCmo : public ILoadableResource, public IMeshResourceCmo
  {
  public:
    MeshResourceCmo(std::string i_meshFilePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    virtual bool isLoaded() const override { return d_loaded; }

    Model& getModel() const { return *d_model; }

  private:
    bool d_loaded = false;
    const std::string d_meshFilePath;
    std::shared_ptr<Model> d_model;
  };
} // ns Dx
