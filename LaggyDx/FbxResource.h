#pragma once

#include "Animation.h"
#include "IFbxResource.h"
#include "LaggyDxFwd.h"
#include "Model.h"


namespace Dx
{
  class FbxResource : public IFbxResource
  {
  public:
    FbxResource(fs::path i_filePath);

    virtual void load(IRenderDevice& i_renderDevice) override;
    virtual void unload() override;

    const IModel& getModel() const;

  private:
    const fs::path d_filePath;
    std::unique_ptr<IModel> d_model;
  };
} // ns Dx
