#include "stdafx.h"
#include "TextureResource.h"

#include "RenderDevice.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/json.h>


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
    setSize();
    loadAnnotation();

    d_loaded = true;
  }

  void TextureResource::unload()
  {
    if (d_texture && d_loaded)
      d_texture->Release();

    d_loaded = false;
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

  void TextureResource::setSize()
  {
    ID3D11Texture2D* textureResource = nullptr;
    d_texture->GetResource(reinterpret_cast<ID3D11Resource**>(&textureResource));
    D3D11_TEXTURE2D_DESC desc;
    textureResource->GetDesc(&desc);

    d_description.width = (int)desc.Width;
    d_description.height = (int)desc.Height;
  }

  void TextureResource::loadAnnotation()
  {
    auto annotationFile = d_textureFilePath;
    annotationFile.replace_extension("json");
    if (!fs::exists(annotationFile))
      return;

    std::ifstream test(annotationFile.string(), std::ifstream::binary);

    Json::Reader reader;
    Json::Value root;
    CONTRACT_ASSERT(reader.parse(test, root, false));

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

} // ns Dx
