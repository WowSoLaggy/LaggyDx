#include "stdafx.h"
#include "TextureBase.h"

#include "Bitmap.h"
#include "RenderDevice.h"

#include <LaggySdk/Locker.h>


namespace Dx
{
  const std::shared_ptr<IBitmap> TextureBase::getBitmap(IRenderDevice& i_renderDevice) const
  {
    auto* texture = getTexturePtr();
    if (!texture)
      return nullptr;

    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    D3D11_TEXTURE2D_DESC readTexDesc = getTextureDesc();
    readTexDesc.BindFlags = 0; //No bind flags allowed for staging
    readTexDesc.Usage = D3D11_USAGE_STAGING; //need staging flag for read
    readTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    D3D11_MAPPED_SUBRESOURCE subres;
    auto bmp = std::make_shared<Bitmap>();

    {
      const Sdk::Locker scopeLocker(renderDevice);

      ID3D11Texture2D* stagingTex = nullptr;
      HRESULT hRes = renderDevice.getDevicePtr()->CreateTexture2D(&readTexDesc, 0, &stagingTex);
      CONTRACT_ASSERT(!FAILED(hRes));
      CONTRACT_ASSERT(stagingTex != nullptr);

      ID3D11Texture2D* sourceTex = nullptr;
      texture->GetResource(reinterpret_cast<ID3D11Resource**>(&sourceTex));

      renderDevice.getDeviceContextPtr()->CopyResource(stagingTex, sourceTex);

      hRes = renderDevice.getDeviceContextPtr()->Map(stagingTex, 0, D3D11_MAP::D3D11_MAP_READ, 0, &subres);
      CONTRACT_ASSERT(!FAILED(hRes));

      bmp->resize(readTexDesc.Width, readTexDesc.Height, subres.RowPitch);
      memcpy(bmp->getData(), (unsigned char*)subres.pData, bmp->getStride() * bmp->getHeight());

      renderDevice.getDeviceContextPtr()->Unmap(stagingTex, 0);
      stagingTex->Release();
    }

    return bmp;
  }

} // ns Dx
