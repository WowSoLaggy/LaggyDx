#include "stdafx.h"
#include "TextureResource.h"

#include "RenderDevice.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/json.h>
#include <LaggySdk/Locker.h>


namespace Dx
{
  TextureResource::TextureResource(fs::path i_textureFilePath)
    : d_textureFilePath(std::move(i_textureFilePath))
  {
  }


  void TextureResource::load(IRenderDevice& i_renderDevice)
  {
    loadTexture(i_renderDevice);
    setSizeFromTexture();
    loadAnnotation();
    fillAlphaMask(i_renderDevice);
  }

  void TextureResource::unload()
  {
    if (d_texture)
      d_texture->Release();
  }


  void TextureResource::loadTexture(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    auto hresult = 1;
    if (d_textureFilePath.extension() == ".dds")
    {
      hresult = CreateDDSTextureFromFile(renderDevice.getDevicePtr(),
        d_textureFilePath.wstring().c_str(), nullptr, &d_texture);
    }
    else
    {
      hresult = CreateWICTextureFromFile(renderDevice.getDevicePtr(),
        d_textureFilePath.wstring().c_str(), nullptr, &d_texture);
    }

    CONTRACT_EXPECT(hresult == 0);
    CONTRACT_EXPECT(d_texture);
  }

  void TextureResource::setSizeFromTexture()
  {
    ID3D11Texture2D* textureResource = nullptr;
    d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&textureResource));
    textureResource->GetDesc(&d_textureDesc);

    d_description.width = (int)d_textureDesc.Width;
    d_description.height = (int)d_textureDesc.Height;

    d_alphaMask.resize(d_description.width * d_description.height, true);
    d_solidAlpha = true;
  }

  void TextureResource::loadAnnotation()
  {
    auto annotationFile = d_textureFilePath;
    annotationFile.replace_extension("json");
    if (!fs::exists(annotationFile))
      return;

    std::ifstream file(annotationFile.string(), std::ifstream::binary);

    Json::Reader reader;
    Json::Value root;
    CONTRACT_ASSERT(reader.parse(file, root, false));

    const auto& descriptionNode = root["Description"];
    d_description.width = descriptionNode["Width"].asInt();
    d_description.height = descriptionNode["Height"].asInt();
    d_description.alpha = descriptionNode["Alpha"].asBool();

    const auto& animationsNode = root["Animations"];
    for (const auto& animationNode : animationsNode)
    {
      ImageAnimation animation;
      animation.start = animationNode["Start"].asInt();
      animation.end = animationNode["End"].asInt();
      animation.frameTime = animationNode["FrameTime"].asDouble();
      d_animations[animationNode["Name"].asString()] = animation;
    }
  }

  void TextureResource::fillAlphaMask(IRenderDevice& i_renderDevice)
  {
    if (!d_description.alpha)
      return;

    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    D3D11_TEXTURE2D_DESC readTexDesc = d_textureDesc;
    readTexDesc.BindFlags = 0; //No bind flags allowed for staging
    readTexDesc.Usage = D3D11_USAGE_STAGING; //need staging flag for read
    readTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    D3D11_MAPPED_SUBRESOURCE subres;
    std::vector<unsigned char> tempArray;

    {
      const Sdk::Locker scopeLocker(renderDevice);

      ID3D11Texture2D* stagingTex = nullptr;
      HRESULT hRes = renderDevice.getDevicePtr()->CreateTexture2D(&readTexDesc, 0, &stagingTex);
      CONTRACT_ASSERT(!FAILED(hRes));
      CONTRACT_ASSERT(stagingTex != nullptr);

      ID3D11Texture2D* sourceTex = nullptr;
      d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&sourceTex));

      renderDevice.getDeviceContextPtr()->CopyResource(stagingTex, sourceTex);

      hRes = renderDevice.getDeviceContextPtr()->Map(stagingTex, 0, D3D11_MAP::D3D11_MAP_READ, 0, &subres);
      CONTRACT_ASSERT(!FAILED(hRes));

      unsigned char* data = (unsigned char*)subres.pData;
      tempArray.resize(subres.DepthPitch, 0);
      memcpy(tempArray.data(), data, subres.DepthPitch);

      renderDevice.getDeviceContextPtr()->Unmap(stagingTex, 0);
      stagingTex->Release();
    }


    d_solidAlpha = true;
    for (int y = 0; y < (int)readTexDesc.Height; ++y)
    {
      auto* rowStart = tempArray.data() + subres.RowPitch * y;
      for (int x = 0; x < (int)readTexDesc.Width; ++x, rowStart += 4)
      {
        bool val = *(rowStart + 3) != 0;
        d_alphaMask.at(x + y * readTexDesc.Width) = val;
        if (!val)
          d_solidAlpha = false;
      }
    }
  }

  bool TextureResource::checkAlpha(Sdk::Vector2I i_coords, int i_frame /*= 0*/) const
  {
    if (d_solidAlpha)
      return true;

    if (i_frame != 0)
      i_coords.x += i_frame * d_description.width;

    return d_alphaMask.at(i_coords.x + i_coords.y * d_textureDesc.Width);
  }


  const Channels& TextureResource::getChannelsInfo(IRenderDevice& i_renderDevice) const
  {
    if (!d_channels)
      calculateChannels(i_renderDevice);

    CONTRACT_EXPECT(d_channels);
    return *d_channels;
  }

  void TextureResource::calculateChannels(IRenderDevice& i_renderDevice) const
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    D3D11_TEXTURE2D_DESC readTexDesc = d_textureDesc;
    readTexDesc.BindFlags = 0; //No bind flags allowed for staging
    readTexDesc.Usage = D3D11_USAGE_STAGING; //need staging flag for read
    readTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

    D3D11_MAPPED_SUBRESOURCE subres;
    std::vector<unsigned char> tempArray;

    {
      const Sdk::Locker scopeLocker(renderDevice);

      ID3D11Texture2D* stagingTex = nullptr;
      HRESULT hRes = renderDevice.getDevicePtr()->CreateTexture2D(&readTexDesc, 0, &stagingTex);
      CONTRACT_ASSERT(!FAILED(hRes));
      CONTRACT_ASSERT(stagingTex != nullptr);

      ID3D11Texture2D* sourceTex = nullptr;
      d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&sourceTex));

      renderDevice.getDeviceContextPtr()->CopyResource(stagingTex, sourceTex);

      hRes = renderDevice.getDeviceContextPtr()->Map(stagingTex, 0, D3D11_MAP::D3D11_MAP_READ, 0, &subres);
      CONTRACT_ASSERT(!FAILED(hRes));

      unsigned char* data = (unsigned char*)subres.pData;
      tempArray.resize(subres.DepthPitch, 0);
      memcpy(tempArray.data(), data, subres.DepthPitch);

      renderDevice.getDeviceContextPtr()->Unmap(stagingTex, 0);
      stagingTex->Release();
    }

    auto initChannel = [&](ChannelInfo& i_info, unsigned char i_value) {
      i_info.min = i_value;
      i_info.max = i_value;
      i_info.mean = i_value;
    };

    auto createChannels = [&](unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
      d_channels = Channels();
      initChannel(d_channels->r, r);
      initChannel(d_channels->g, g);
      initChannel(d_channels->b, b);
      initChannel(d_channels->a, a);
    };

    auto addValue = [&](ChannelInfo& i_info, unsigned char i_value) {
      if (i_value < i_info.min)
        i_info.min = i_value;
      if (i_value > i_info.max)
        i_info.max = i_value;
    };

    auto setMean = [&](ChannelInfo& i_info, int i_sum) {
      i_info.mean = (double)i_sum / (readTexDesc.Width * readTexDesc.Height);
    };

    int rSum = 0;
    int gSum = 0;
    int bSum = 0;
    int aSum = 0;


    for (int y = 0; y < (int)readTexDesc.Height; ++y)
    {
      const unsigned char* rowStart = tempArray.data() + subres.RowPitch * y;
      for (int x = 0; x < (int)readTexDesc.Width; ++x, rowStart += 4)
      {
        const unsigned char r = *(rowStart + 0);
        const unsigned char g = *(rowStart + 1);
        const unsigned char b = *(rowStart + 2);
        const unsigned char a = *(rowStart + 3);

        if (!d_channels)
          createChannels(r, g, b, a);
        else
        {
          addValue(d_channels->r, r);
          addValue(d_channels->g, g);
          addValue(d_channels->b, b);
          addValue(d_channels->a, a);
        }

        rSum += r;
        gSum += g;
        bSum += b;
        aSum += a;
      }
    }

    setMean(d_channels->r, rSum);
    setMean(d_channels->g, gSum);
    setMean(d_channels->b, bSum);
    setMean(d_channels->a, aSum);
  }


  fs::path TextureResource::getFilename() const
  {
    return d_textureFilePath.filename();
  }
} // ns Dx
