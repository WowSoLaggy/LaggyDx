#include "stdafx.h"
#include "TextureResource.h"

#include "RenderDevice.h"

#include <Sdk/Contracts.h>
#include <Sdk/json.h>
#include <Sdk/Locker.h>


namespace Dx
{
  TextureResource::TextureResource(fs::path i_textureFilePath)
    : d_textureFilePath(std::move(i_textureFilePath))
    , d_loaded(false)
  {
  }


  void TextureResource::load(IRenderDevice& i_renderDevice)
  {
    loadTexture(i_renderDevice);
    setSizeFromTexture();
    loadAnnotation();
    fillAlphaMask(i_renderDevice);

    d_loaded = true;
  }

  void TextureResource::unload()
  {
    d_loaded = false;

    if (d_texture && d_loaded)
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
      HRESULT hres = renderDevice.getDevicePtr()->CreateTexture2D(&readTexDesc, 0, &stagingTex);

      ID3D11Texture2D* sourceTex = nullptr;
      d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&sourceTex));

      renderDevice.getDeviceContextPtr()->CopyResource(stagingTex, sourceTex);

      auto res = renderDevice.getDeviceContextPtr()->Map(stagingTex, 0, D3D11_MAP::D3D11_MAP_READ, 0, &subres);

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


  fs::path TextureResource::getFilename() const
  {
    return d_textureFilePath.filename();
  }
} // ns Dx
