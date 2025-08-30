#include "stdafx.h"
#include "TextureUtils.h"

#include "App.h"
#include "DxResourceWrapper.h"
#include "MemoryTexture.h"
#include "RenderDevice.h"


namespace Dx
{
  namespace TextureUtils
  {
    const Dx::ITexture& getTexture(const fs::path& i_name)
    {
      const auto& rc = Dx::App::get().getResourceController();
      return rc.getTexture(i_name);
    }


    std::shared_ptr<MemoryTexture> createMemoryTexture(
      const IRenderDevice& i_renderDevice,
      const unsigned char* i_imageData,
      const int i_width,
      const int i_height,
      const GlChannels i_channels)
    {
      std::vector<unsigned char> rgbaData;

      // If the image is RGB, convert to RGBA
      if (i_channels == GlChannels::RGB)
      {
        rgbaData.resize(i_width * i_height * 4);
        for (int i = 0; i < i_width * i_height; ++i)
        {
          rgbaData[i * 4 + 0] = i_imageData[i * 3 + 0];
          rgbaData[i * 4 + 1] = i_imageData[i * 3 + 1];
          rgbaData[i * 4 + 2] = i_imageData[i * 3 + 2];
          rgbaData[i * 4 + 3] = 255;
        }
      }
      else if (i_channels == GlChannels::RGBA)
      {
        rgbaData.assign(i_imageData, i_imageData + i_width * i_height * 4);
        // For debug purposes assign 255 to all elements:
        for (auto& byte : rgbaData)
          byte = 255;
      }
      else
      {
        CONTRACT_THROW("Not Implemented");
      }

      D3D11_TEXTURE2D_DESC desc = {};
      desc.Width = i_width;
      desc.Height = i_height;
      desc.MipLevels = 1;
      desc.ArraySize = 1;
      desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      desc.SampleDesc.Count = 1;
      desc.Usage = D3D11_USAGE_DEFAULT;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

      D3D11_SUBRESOURCE_DATA initData = {};
      initData.pSysMem = i_imageData;
      initData.SysMemPitch = i_width * 4;

      DxResourceWrapper<ID3D11Texture2D> texture;
      const auto& renderDevice = dynamic_cast<const RenderDevice&>(i_renderDevice);
      auto hResult = renderDevice.getDevicePtr()->CreateTexture2D(&desc, &initData, texture.getPp());
      CONTRACT_ASSERT(SUCCEEDED(hResult), "Failed to create texture from memory");

      D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
      srvDesc.Format = desc.Format;
      srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
      srvDesc.Texture2D.MipLevels = 1;

      ID3D11ShaderResourceView* srv = nullptr;
      hResult = renderDevice.getDevicePtr()->CreateShaderResourceView(texture.get(), &srvDesc, &srv);
      CONTRACT_ASSERT(SUCCEEDED(hResult), "Failed to create SRV from texture");

      return std::make_shared<MemoryTexture>(std::move(texture), *srv);
    }

  } // ns TextureUtils
} // ns Dx
