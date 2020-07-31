#include "stdafx.h"
#include "ResourceController.h"

#include "FontResource.h"
#include "MeshResourceCmo.h"
#include "PixelShaderResource.h"
#include "TextureResource.h"
#include "VertexShaderResource.h"

#include <LaggySdk/Contracts.h>


namespace Dx
{
  namespace
  {

    bool isDirNotDots(struct dirent* pEntity)
    {
      return pEntity && (pEntity->d_type == DT_DIR) &&
        (strcmp(pEntity->d_name, ".") != 0) && (strcmp(pEntity->d_name, "..") != 0);
    }

  } // anonymous NS


  ResourceController::ResourceController(IRenderDevice& i_renderDevice, fs::path i_resourcesFolder)
    : d_renderDevice(i_renderDevice)
    , d_resourceFolder(std::move(i_resourcesFolder))
  {
    indexResourcesInDir(d_resourceFolder);
  }

  ResourceController::~ResourceController()
  {
    unloadResources();
  }


  const IMeshResourceCmo& ResourceController::getMeshResourceCmo(const std::string& i_resourceName)
  {
    const auto it = d_meshResourcesMap.find((d_resourceFolder / i_resourceName).string());
    CONTRACT_EXPECT(it != d_meshResourcesMap.cend());
    CONTRACT_EXPECT(it->second);

    loadResource(*it->second);
    return *it->second;
  }

  const ITextureResource& ResourceController::getTextureResource(const std::string& i_resourceName)
  {
    const auto it = d_textureResourcesMap.find((d_resourceFolder / i_resourceName).string());
    CONTRACT_EXPECT(it != d_textureResourcesMap.cend());
    CONTRACT_EXPECT(it->second);

    loadResource(*it->second);
    return *it->second;
  }

  const IPixelShaderResource& ResourceController::getPixelShaderResource(const std::string& i_resourceName)
  {
    const auto it = d_pixelShaderResourcesMap.find((d_resourceFolder / i_resourceName).string());
    CONTRACT_EXPECT(it != d_pixelShaderResourcesMap.cend());
    CONTRACT_EXPECT(it->second);

    loadResource(*it->second);
    return *it->second;
  }

  const IVertexShaderResource& ResourceController::getVertexShaderResource(const std::string& i_resourceName)
  {
    const auto it = d_vertexShaderResourcesMap.find((d_resourceFolder / i_resourceName).string());
    CONTRACT_EXPECT(it != d_vertexShaderResourcesMap.cend());
    CONTRACT_EXPECT(it->second);

    loadResource(*it->second);
    return *it->second;
  }

  const IFontResource& ResourceController::getFontResource(const std::string& i_resourceName)
  {
    const auto it = d_fontResourcesMap.find((d_resourceFolder / i_resourceName).string());
    CONTRACT_EXPECT(it != d_fontResourcesMap.cend());
    CONTRACT_EXPECT(it->second);

    loadResource(*it->second);
    return *it->second;
  }


  void ResourceController::loadResource(ILoadableResource& i_resource)
  {
    if (!i_resource.isLoaded())
      i_resource.load(d_renderDevice);
  }

  void ResourceController::loadResources()
  {
    auto loadAll = [&](auto& i_map)
    {
      for (auto& it : i_map)
        loadResource(*it.second);
    };

    loadAll(d_meshResourcesMap);
    loadAll(d_textureResourcesMap);
    loadAll(d_pixelShaderResourcesMap);
    loadAll(d_vertexShaderResourcesMap);
    loadAll(d_fontResourcesMap);
  }

  void ResourceController::unloadResources()
  {
    auto unloadAll = [](auto& i_map)
    {
      for (auto& it : i_map)
        it.second->unload();
    };

    unloadAll(d_meshResourcesMap);
    unloadAll(d_textureResourcesMap);
    unloadAll(d_pixelShaderResourcesMap);
    unloadAll(d_vertexShaderResourcesMap);
    unloadAll(d_fontResourcesMap);
  }


  void ResourceController::indexResourcesInDir(const fs::path& i_dirName)
  {
    auto* pDir = opendir(i_dirName.string().c_str());
    if (!pDir)
      return;

    struct dirent* pEntity;
    while (pEntity = readdir(pDir))
    {
      if (isDirNotDots(pEntity))
      {
        indexResourcesInDir(i_dirName / pEntity->d_name);
        continue;
      }

      if (pEntity->d_type != DT_REG)
      {
        // Not a regular file - we don't care
        continue;
      }

      const std::regex texturePattern("\\w*.(dds|png|bmp|jpg|jpeg|tiff|gif)");
      const std::regex modelCmoPattern("\\w*.(cmo)");
      const std::regex vertexShaderPattern("\\w*.(vs)");
      const std::regex pixelShaderPattern("\\w*.(ps)");
      const std::regex fontPattern("\\w*.(spritefont)");

      auto resourceName = (i_dirName / pEntity->d_name).string();

      if (std::regex_match(pEntity->d_name, modelCmoPattern))
        d_meshResourcesMap.insert({ resourceName, std::make_shared<MeshResourceCmo>(resourceName) });
      else if (std::regex_match(pEntity->d_name, texturePattern))
        d_textureResourcesMap.insert({ resourceName, std::make_shared<TextureResource>(resourceName) });
      else if (std::regex_match(pEntity->d_name, vertexShaderPattern))
        d_vertexShaderResourcesMap.insert({ resourceName, std::make_shared<VertexShaderResource>(resourceName) });
      else if (std::regex_match(pEntity->d_name, pixelShaderPattern))
        d_pixelShaderResourcesMap.insert({ resourceName, std::make_shared<PixelShaderResource>(resourceName) });
      else if (std::regex_match(pEntity->d_name, fontPattern))
        d_fontResourcesMap.insert({ resourceName, std::make_shared<FontResource>(resourceName) });
    }
  }

} // ns Dx
