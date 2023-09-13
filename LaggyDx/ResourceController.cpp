#include "stdafx.h"
#include "ResourceController.h"

#include "FbxResource.h"
#include "FontResource.h"
#include "MeshResourceCmo.h"
#include "PsBinResource.h"
#include "PsHlslResource.h"
#include "Texture.h"
#include "VsBinResource.h"
#include "VsHlslResource.h"

#include <LaggySdk/Contracts.h>
#include <LaggySdk/Files.h>


namespace Dx
{
  namespace
  {
    template <typename T>
    T& getResource(const ResourcesMap& i_map, const std::string& i_name)
    {
      const auto it = i_map.find(i_name);
      CONTRACT_EXPECT(it != i_map.cend());
      CONTRACT_EXPECT(it->second);
      return dynamic_cast<T&>(*it->second);
    }

    template <typename T>
    T& getResource(const ResourcesMap& i_map, const fs::path& i_name, const fs::path& i_assetsPath)
    {
      return getResource<T>(i_map, (i_assetsPath / i_name).string());
    }

  } // anonymous NS


  ResourceController::ResourceController(IRenderDevice& i_renderDevice, fs::path i_resourcesFolder)
    : d_renderDevice(i_renderDevice)
  {
    d_resourceFolder = Sdk::getExeFolder() / i_resourcesFolder;
    indexResourcesInDir(d_resourceFolder);
  }

  ResourceController::~ResourceController()
  {
    unloadResources();
  }


  bool ResourceController::hasResource(const fs::path& i_name) const
  {
    const auto it = d_resources.find((d_resourceFolder / i_name).string());
    return it != d_resources.end();
  }


  const IMeshResourceCmo& ResourceController::getMeshCmo(const fs::path& i_name) const
  {
    auto& res = getResource<IMeshResourceCmo>(d_resources, i_name, d_resourceFolder);
    loadResource(res);
    return res;
  }

  const ITexture& ResourceController::getTexture(const fs::path& i_name) const
  {
    auto& res = getResource<ITexture>(d_resources, i_name, d_resourceFolder);
    loadResource(res);
    return res;
  }

  const IPsResource& ResourceController::getPs(const fs::path& i_name) const
  {
    auto& res = getResource<IPsResource>(d_resources, i_name, d_resourceFolder);
    loadResource(res);
    return res;
  }

  const IVsResource& ResourceController::getVs(const fs::path& i_name) const
  {
    auto& res = getResource<IVsResource>(d_resources, i_name, d_resourceFolder);
    loadResource(res);
    return res;
  }

  const IFontResource& ResourceController::getFont(const fs::path& i_name) const
  {
    auto& res = getResource<IFontResource>(d_resources, i_name, d_resourceFolder);
    loadResource(res);
    return res;
  }

  const IFbxResource& ResourceController::getFbx(const fs::path& i_name) const
  {
    auto& res = getResource<IFbxResource>(d_resources, i_name, d_resourceFolder);
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
    for (const auto& it : fs::directory_iterator(i_dirName))
    {
      const auto& path = it.path();

      if (it.is_directory())
      {
        indexResourcesInDir(path);
      }
      else if (it.is_regular_file())
      {
        const std::regex texturePattern("\\w*.(dds|png|bmp|jpg|jpeg|tiff|gif)$");
        const std::regex modelCmoPattern("\\w*.(cmo)$");
        const std::regex vsBinPattern("\\w*.(vs)$");
        const std::regex vsHlslPattern("\\w*.(vs.hlsl)$");
        const std::regex psBinPattern("\\w*.(ps)$");
        const std::regex psHlslPattern("\\w*.(ps.hlsl)$");
        const std::regex fontPattern("\\w*.(spritefont)$");
        const std::regex fbxPattern("\\w*.(fbx)$");

        if (std::regex_match(path.filename().string(), modelCmoPattern))
          d_resources.insert({ path.string(), std::make_shared<MeshResourceCmo>(path) });
        else if (std::regex_match(path.filename().string(), texturePattern))
          d_resources.insert({ path.string(), std::make_shared<Texture>(path) });
        else if (std::regex_match(path.filename().string(), vsBinPattern))
          d_resources.insert({ path.string(), std::make_shared<VsBinResource>(path) });
        else if (std::regex_match(path.filename().string(), vsHlslPattern))
          d_resources.insert({ path.string(), std::make_shared<VsHlslResource>(path) });
        else if (std::regex_match(path.filename().string(), psBinPattern))
          d_resources.insert({ path.string(), std::make_shared<PsBinResource>(path) });
        else if (std::regex_match(path.filename().string(), psHlslPattern))
          d_resources.insert({ path.string(), std::make_shared<PsHlslResource>(path) });
        else if (std::regex_match(path.filename().string(), fontPattern))
          d_resources.insert({ path.string(), std::make_shared<FontResource>(path) });
        else if (std::regex_match(path.filename().string(), fbxPattern))
          d_resources.insert({ path.string(), std::make_shared<FbxResource>(path) });
      }
    }
  }

} // ns Dx
