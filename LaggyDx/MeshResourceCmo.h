#pragma once

#include "IResource.h"


class MeshResourceCmo : public IResource
{
public:

  MeshResourceCmo(std::string i_meshFilePath);

  virtual void load(IRenderDevice& i_renderDevice) override;
  virtual void unload() override;

  Model& getModel() const { return *d_model; }

private:

  const std::string d_meshFilePath;
  std::shared_ptr<Model> d_model;

};
