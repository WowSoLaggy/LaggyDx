#include "stdafx.h"
#include "MeshResourceCmo.h"

#include "RenderDevice.h"

#include <LaggySdk/StringUtils.h>


namespace Dx
{
  MeshResourceCmo::MeshResourceCmo(fs::path i_meshFilePath)
    : d_meshFilePath(std::move(i_meshFilePath))
  {
  }


  void MeshResourceCmo::load(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    DGSLEffectFactory dgslEffectFactory(renderDevice.getDevicePtr());
    dgslEffectFactory.SetDirectory(L".\\Data");
    d_model = DirectX::Model::CreateFromCMO(renderDevice.getDevicePtr(),
      d_meshFilePath.wstring().c_str(), dgslEffectFactory);
  }

  void MeshResourceCmo::unload()
  {
    d_model.reset();
  }

} // ns Dx
