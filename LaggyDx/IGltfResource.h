#pragma once

#include "LoadableResource.h"

#include <LaggySdk/Vector.h>


namespace Dx
{
  // How a loaded model is mapped into a consumer's own space - applied offset, then scale, then yaw.
  struct ModelTransform
  {
    Sdk::Vector3F offset;
    float scale = 1.0f;
    float yaw = 0.0f;
  };


  class IGltfResource : public LoadableResource
  {
  public:
    virtual std::shared_ptr<IModel> getModel() const = 0;

    // A private copy of the model with i_transform baked into its verts - callers may mutate its materials.
    virtual std::shared_ptr<IModel> makeTransformedModel(const ModelTransform& i_transform) const = 0;
  };

} // ns Dx
