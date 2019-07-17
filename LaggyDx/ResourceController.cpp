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


  ResourceController::ResourceController(const std::string& i_resourcesFolder)
    : d_resourceFolder(".\\" + i_resourcesFolder + "\\")
  {
    indexResourcesInDir(d_resourceFolder);
  }

  ResourceController::~ResourceController()
  {
    unloadResources();
  }


  ResourceId ResourceController::getResourceId(const std::string& i_resourceName) const
  {
    const auto it = d_nameToIdMap.find(d_resourceFolder + i_resourceName);
    CONTRACT_EXPECT(it != d_nameToIdMap.cend());
    return it->second;
  }


  const MeshResourceCmo& ResourceController::getMeshResourceCmo(ResourceId i_resourceId) const
  {
    return dynamic_cast<const MeshResourceCmo&>(*d_idToResourceMap.at(i_resourceId));
  }

  const TextureResource& ResourceController::getTextureResource(ResourceId i_resourceId) const
  {
    return dynamic_cast<const TextureResource&>(*d_idToResourceMap.at(i_resourceId));
  }

  const PixelShaderResource& ResourceController::getPixelShaderResource(ResourceId i_resourceId) const
  {
    return dynamic_cast<const PixelShaderResource&>(*d_idToResourceMap.at(i_resourceId));
  }

  const VertexShaderResource& ResourceController::getVertexShaderResource(ResourceId i_resourceId) const
  {
    return dynamic_cast<const VertexShaderResource&>(*d_idToResourceMap.at(i_resourceId));
  }

  const FontResource& ResourceController::getFontResource(ResourceId i_resourceId) const
  {
    return dynamic_cast<const FontResource&>(*d_idToResourceMap.at(i_resourceId));
  }


  void ResourceController::loadResources(IRenderDevice& i_renderDevice)
  {
    for (auto& it : d_idToResourceMap)
      it.second->load(i_renderDevice);
  }

  void ResourceController::unloadResources()
  {
    for (auto& it : d_idToResourceMap)
      it.second->unload();
  }


  ResourceId ResourceController::getFreeResourceId()
  {
    return d_nextResourceId++;
  }


  void ResourceController::indexResourcesInDir(const std::string& i_dirName)
  {
    auto* pDir = opendir(i_dirName.c_str());
    if (!pDir)
      return;

    struct dirent* pEntity;
    while (pEntity = readdir(pDir))
    {
      if (isDirNotDots(pEntity))
      {
        indexResourcesInDir(i_dirName + pEntity->d_name + "\\");
        continue;
      }

      if (pEntity->d_type != DT_REG)
      {
        // Not a regular file - we don't care
        continue;
      }

      const std::regex texturePattern("\\w*.(dds)");
      const std::regex modelCmoPattern("\\w*.(cmo)");
      const std::regex vertexShaderPattern("\\w*.(vs)");
      const std::regex pixelShaderPattern("\\w*.(ps)");
      const std::regex fontPattern("\\w*.(spritefont)");

      auto resourceName = i_dirName + pEntity->d_name;
      auto freeResourceId = getFreeResourceId();

      if (std::regex_match(pEntity->d_name, modelCmoPattern))
      {
        d_nameToIdMap.insert({ resourceName, freeResourceId });
        d_idToResourceMap.insert({ freeResourceId, std::make_shared<MeshResourceCmo>(resourceName) });
      }
      else if (std::regex_match(pEntity->d_name, texturePattern))
      {
        d_nameToIdMap.insert({ resourceName, freeResourceId });
        d_idToResourceMap.insert({ freeResourceId, std::make_shared<TextureResource>(resourceName) });
      }
      else if (std::regex_match(pEntity->d_name, vertexShaderPattern))
      {
        d_nameToIdMap.insert({ resourceName, freeResourceId });
        d_idToResourceMap.insert({ freeResourceId, std::make_shared<VertexShaderResource>(resourceName) });
      }
      else if (std::regex_match(pEntity->d_name, pixelShaderPattern))
      {
        d_nameToIdMap.insert({ resourceName, freeResourceId });
        d_idToResourceMap.insert({ freeResourceId, std::make_shared<PixelShaderResource>(resourceName) });
      }
      else if (std::regex_match(pEntity->d_name, fontPattern))
      {
        d_nameToIdMap.insert({ resourceName, freeResourceId });
        d_idToResourceMap.insert({ freeResourceId, std::make_shared<FontResource>(resourceName) });
      }
    }
  }

} // ns Dx
