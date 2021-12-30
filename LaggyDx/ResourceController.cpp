#include "stdafx.h"
#include "ResourceController.h"

#include "FontResource.h"
#include "MeshResourceCmo.h"
#include "PsResource.h"
#include "TextureResource.h"
#include "VsResource.h"

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

    template <typename T>
    T& getResource(const ResourcesMap& i_map, const std::string& i_name)
    {
      const auto it = i_map.find(i_name);
      CONTRACT_EXPECT(it != i_map.cend());
      CONTRACT_EXPECT(it->second);
      return dynamic_cast<T&>(*it->second);
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


  const IMeshResourceCmo& ResourceController::getMeshResourceCmo(const std::string& i_resourceName) const
  {
    const std::string fullName = (d_resourceFolder / i_resourceName).string();
    auto& res = getResource<IMeshResourceCmo>(d_resources, fullName);
    loadResource(res);
    return res;
  }

  const ITextureResource& ResourceController::getTextureResource(const std::string& i_resourceName) const
  {
    const std::string fullName = (d_resourceFolder / i_resourceName).string();
    auto& res = getResource<ITextureResource>(d_resources, fullName);
    loadResource(res);
    return res;
  }

  const IPsResource& ResourceController::getPsResource(const std::string& i_resourceName) const
  {
    const std::string fullName = (d_resourceFolder / i_resourceName).string();
    auto& res = getResource<IPsResource>(d_resources, fullName);
    loadResource(res);
    return res;
  }

  const IVsResource& ResourceController::getVsResource(const std::string& i_resourceName) const
  {
    const std::string fullName = (d_resourceFolder / i_resourceName).string();
    auto& res = getResource<IVsResource>(d_resources, fullName);
    loadResource(res);
    return res;
  }

  const IFontResource& ResourceController::getFontResource(const std::string& i_resourceName) const
  {
    const std::string fullName = (d_resourceFolder / i_resourceName).string();
    auto& res = getResource<IFontResource>(d_resources, fullName);
    loadResource(res);
    return res;
  }


  void ResourceController::loadResource(ILoadableResource& i_resource) const
  {
    i_resource.loadIfNeeded(d_renderDevice);
  }

  void ResourceController::loadResources() const
  {
    for (auto& it : d_resources)
      loadResource(*it.second);
  }

  void ResourceController::unloadResources() const
  {
    for (auto& it : d_resources)
      it.second->unloadIfNeeded();
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
      const std::regex vsPattern("\\w*.(vs)");
      const std::regex psPattern("\\w*.(ps)");
      const std::regex fontPattern("\\w*.(spritefont)");

      auto resourceName = i_dirName / pEntity->d_name;

      if (std::regex_match(pEntity->d_name, modelCmoPattern))
        d_resources.insert({ resourceName.string(), std::make_shared<MeshResourceCmo>(resourceName) });
      else if (std::regex_match(pEntity->d_name, texturePattern))
        d_resources.insert({ resourceName.string(), std::make_shared<TextureResource>(resourceName) });
      else if (std::regex_match(pEntity->d_name, vsPattern))
        d_resources.insert({ resourceName.string(), std::make_shared<VsResource>(resourceName) });
      else if (std::regex_match(pEntity->d_name, psPattern))
        d_resources.insert({ resourceName.string(), std::make_shared<PsResource>(resourceName) });
      else if (std::regex_match(pEntity->d_name, fontPattern))
        d_resources.insert({ resourceName.string(), std::make_shared<FontResource>(resourceName) });
    }
  }

} // ns Dx
