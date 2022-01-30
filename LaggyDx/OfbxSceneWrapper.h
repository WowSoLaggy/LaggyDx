#pragma once

#include "LaggyDxFwd.h"


namespace Dx
{
  class OfbxSceneWrapper
  {
  public:
    OfbxSceneWrapper(const fs::path& i_filePath);
    ~OfbxSceneWrapper();

    const ofbx::IScene& operator*() const;
    const ofbx::IScene* operator->() const;

  private:
    ofbx::IScene* d_scene = nullptr;
  };

} // ns Dx
