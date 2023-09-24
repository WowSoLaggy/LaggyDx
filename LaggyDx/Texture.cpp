#include "stdafx.h"
#include "Texture.h"

#include "RenderDevice.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/json.h>
#include <LaggySdk/Locker.h>


namespace Dx
{
  Texture::Texture(fs::path i_textureFilePath)
    : d_textureFilePath(std::move(i_textureFilePath))
  {
  }


  void Texture::load(IRenderDevice& i_renderDevice)
  {
    loadTexture(i_renderDevice);
    setSizeFromTexture();
    loadAnnotation();
    fillAlphaMask(i_renderDevice);
  }

  void Texture::unload()
  {
    if (d_texture)
    {
      d_texture->Release();
      d_texture = nullptr;
    }
  }


  void Texture::loadTexture(IRenderDevice& i_renderDevice)
  {
    auto& renderDevice = dynamic_cast<RenderDevice&>(i_renderDevice);

    HRESULT hRes = 1;
    if (d_textureFilePath.extension() == ".dds")
    {
      hRes = CreateDDSTextureFromFileEx(
        renderDevice.getDevicePtr(),
        d_textureFilePath.wstring().c_str(),
        0,
        D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
        DDS_LOADER_IGNORE_SRGB,
        nullptr, &d_texture,
        nullptr);
    }
    else
    {
      hRes = CreateWICTextureFromFileEx(
        renderDevice.getDevicePtr(),
        d_textureFilePath.wstring().c_str(),
        0,
        D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
        WIC_LOADER_IGNORE_SRGB,
        nullptr, &d_texture);
    }

    CONTRACT_EXPECT(!FAILED(hRes));
    CONTRACT_EXPECT(d_texture);
  }

  void Texture::setSizeFromTexture()
  {
    ID3D11Texture2D* textureResource = nullptr;
    d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&textureResource));
    textureResource->GetDesc(&d_textureDesc);

    d_description.width = (int)d_textureDesc.Width;
    d_description.height = (int)d_textureDesc.Height;
    d_description.frameWidth = d_description.width;
    d_description.frameHeight = d_description.height;

    d_alphaMask.resize(d_description.width * d_description.height, true);
    d_solidAlpha = true;
  }

  void Texture::loadAnnotation()
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
    d_description.frameWidth = descriptionNode["FrameWidth"].asInt();
    d_description.frameHeight = descriptionNode["FrameHeight"].asInt();
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

  void Texture::fillAlphaMask(IRenderDevice& i_renderDevice)
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

  bool Texture::hasAlpha() const
  {
    return !d_solidAlpha;
  }

  bool Texture::checkAlpha(Sdk::Vector2I i_coords, int i_frame /*= 0*/) const
  {
    if (d_solidAlpha)
      return true;

    if (i_frame != 0)
      i_coords.x += i_frame * d_description.frameWidth;

    return d_alphaMask.at(i_coords.x + i_coords.y * d_textureDesc.Width);
  }

  
  fs::path Texture::getFilename() const
  {
    return d_textureFilePath.filename();
  }

  const ImageDescription& Texture::getDescription() const
  {
    return d_description;
  }

  const Animations2Map& Texture::getAnimationsMap() const
  {
    return d_animations;
  }


  ID3D11ShaderResourceView* Texture::getTexturePtr() const
  {
    return d_texture;
  }

  const D3D11_TEXTURE2D_DESC& Texture::getTextureDesc() const
  {
    return d_textureDesc;
  }

} // ns Dx
