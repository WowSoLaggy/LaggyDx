#include "stdafx.h"
#include "OfbxSceneWrapper.h"

#include "ofbx.h"

#include <LaggySdk/Files.h>


namespace Dx
{
  OfbxSceneWrapper::OfbxSceneWrapper(const fs::path& i_filePath)
  {
    const auto buffer = Sdk::readBytes(i_filePath);
    d_scene = ofbx::load(buffer.data(), (int)buffer.size(), (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
    CONTRACT_ASSERT(d_scene);
  }

  OfbxSceneWrapper::~OfbxSceneWrapper()
  {
    d_scene->destroy();
  }


  const ofbx::IScene& OfbxSceneWrapper::operator*() const { return *d_scene; }
  const ofbx::IScene* OfbxSceneWrapper::operator->() const { return d_scene; }

} // ns Dx
