/******************************************************************************/
/*!
\par     Sulfur
\file    sfResourceManager.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/23/2016

\brief   Resource manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "sfResourceImporter.hpp"
#include "Utils/sfFileUtils.hpp"

namespace Sulfur
{
	
  template <typename ResourceType>
  class ResourceManager
  {
    // Singleton
    SF_PRIVATE_CTOR_DTOR_EMPTY(ResourceManager)
    SF_FORBID_COPY(ResourceManager)
    SF_SINGLETON_INSTANCE(ResourceManager<ResourceType>)

  private:
    typedef std::unordered_map<std::string, ResourceType*> ResourceMap;
    typedef std::unordered_map<std::string, IResourceImporter<ResourceType>*> ImporterMap;

  public:
    ResourceType* LoadResource(const std::string& path)
    {
      AssertRegistered();

      // Check if resource already loaded
      auto resIt = m_resources.find(path);
      if (resIt != m_resources.end())
        return resIt->second;

      // Get the importer for the extension
      ImporterMap& importers = GetImporters();
      std::string ext = FileUtils::GetExtension(path);
      auto it = importers.find(ext);
      SF_CRITICAL_ERR_EXP(
        it != importers.end(),
        std::string("No importer registered for file type: ") + ext
        );

      ResourceType *resource = new ResourceType();
      m_resources[path] = resource;

      std::string actualPath = "Resources/" + path;
      SF_CRITICAL_ERR_EXP(
        FileUtils::FileExists(actualPath),
        std::string("File does not exist:") + path
        );

      UINT32 fileSize = FileUtils::GetFileSize(actualPath);
      BYTE *fileData = FileUtils::ReadFile(actualPath);
      it->second->Load(fileData, fileSize, resource);

      return resource;
    }

    void UnloadResource(const std::string& path)
    {
      AssertRegistered();

      auto it = m_resources.find(path);
      
      SF_CRITICAL_ERR_EXP(
        it != m_resources.end(),
        "Trying to unload a resource that isn't loaded"
        );

      delete it->second;
      m_resources.erase(it);
    }

    static void UnloadAll()
    {
      AssertRegistered();

      for (auto& pair : ResourceManager<ResourceType>::Instance()->m_resources)
        delete pair.second;

      ResourceManager<ResourceType>::Instance()->m_resources.clear();
    }

    template <typename ImporterType>
    static void RegisterImporter(const std::string& ext)
    {
      GetImporters()[ext] = new ImporterType();
    }

    static void RegisterImporters()
    {
    }

    static bool IsExtensionRegistered(const std::string& ext)
    {
      ImporterMap& importers = GetImporters();
      return importers.find(ext) != importers.end();
    }

  private:
    static void AssertRegistered()
    {
      SF_CRITICAL_ERR_EXP(
        s_registered,
        "Resource type not registered"
        );
    }

    static ImporterMap& GetImporters()
    {
      static ImporterMap importers;
      return importers;
    }

  private:
    ResourceMap m_resources;
    static bool s_registered;

  };

  //template <typename ResourceType>
  //bool ResourceManager<ResourceType>::s_registered = false;

  class ResourceRegistry
  {

  private:
    struct ResourceInfo
    {
      std::string name;
      void(*unloadAll)();
    };

  public:
    typedef std::vector<ResourceInfo> ResourceInfoList;

  public:
    template <typename ResourceType>
    static bool RegisterResource(const std::string& name)
    {
      GetInfoList().push_back(ResourceInfo{ name, &ResourceManager<ResourceType>::UnloadAll });
      ResourceManager<ResourceType>::RegisterImporters();
      return true;
    }

    static void UnloadAllResources()
    {
      ResourceInfoList& infoList = GetInfoList();
      for (ResourceInfo& info : infoList)
      {
        (*info.unloadAll)();
      }
    }

  private:
    static ResourceInfoList& GetInfoList()
    {
      static ResourceInfoList infoList;
      return infoList;
    }

  };
  
}

#define SF_REGISTER_RESOURCE_TYPE(type) bool Sulfur::ResourceManager<type>::s_registered = ResourceRegistry::RegisterResource<type>(#type); \
template <> void Sulfur::ResourceManager<type>::RegisterImporters() {

#define SF_IMPORTER(importerType, ext) RegisterImporter<importerType>(ext);

#define SF_END_REGISTER_RESOURCE_TYPE() }

#define SF_RESOURCE_MANAGER(type) Sulfur::ResourceManager<type>::Instance()
