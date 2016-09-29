/******************************************************************************/
/*!
\par     Sulfur
\file    sfScriptManager.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/22/2016

\brief   Script manager
         When new script is added, it is going to be compiled as soon
         as both cpp and hpp are located
         Script is recompiled when either hpp or cpp is modified
         When script is modified recompilation is attempted. If there are any
         errors, last working dll version will be used.
         If hpp or cpp is removed, script is removed completely
         Renaming will only change internal data

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <Shlwapi.h>

#include "sfScriptManager.hpp"
#include "Types\sfIEntity.hpp"
#include "Logger\sfLogger.hpp"
#include "Settings\sfEngineSettings.hpp"
#include "RuntimeCompiler\sfCompiler.hpp"

namespace Sulfur
{
  void FileWatcherCallback(const FileWatcher::ActionInfo &actionInfo)
  {
    switch (actionInfo.m_action)
    {
    case FileWatcher::Action::FileAdded:
      ScriptManager::Instance()->_HandleFileAddedAction(actionInfo);
      break;
    case FileWatcher::Action::FileModified:
      ScriptManager::Instance()->_HandleFileModifiedAction(actionInfo);
      break;
    case FileWatcher::Action::FileRemoved:
      ScriptManager::Instance()->_HandleFileRemovedAction(actionInfo);
      break;
    case FileWatcher::Action::FileRenamedNew:
      ScriptManager::Instance()->_HandleFileRenamedNewAction(actionInfo);
      break;
    case FileWatcher::Action::FileRenamedOld:
      ScriptManager::Instance()->_HandleFileRenamedOldAction(actionInfo);
      break;
    default:
      SF_ASSERT(false, "Unrecognized action");
    }
  }

  ScriptManager::ScriptManager(void) : m_compiler(nullptr), m_fileWatcher(nullptr)
  {

  }

  ScriptManager::~ScriptManager(void)
  {
    if (m_compiler)
      delete m_compiler;
  }

  void ScriptManager::Initialize(void)
  {
    //Init compiler
    m_compiler = new Compiler();
    m_compiler->Initialize();

    //Init fileWatcher
    m_fileWatcher = new FileWatcher(FileWatcherCallback);

    //Get script directory path
    char buffer[MAX_PATH];
    GetCurrentDirectory((DWORD)MAX_PATH, buffer);
    std::string s = EngineSettings::ScriptSourceFolder;
    s.pop_back();

    std::string scriptPath = buffer + ("\\" + s);

    PathCanonicalize(buffer, scriptPath.c_str());

    m_compiler->SetWorkingDirectory(buffer);
    m_compiler->AddIncludeFolderRelative("..\\SulfurEngine\\");

    m_fileWatcher->AddDirectoryToWatch(std::string(buffer));
  }

  void ScriptManager::RegisterScript(IEntity *scriptInstance)
  {
    return;
  }

  void ScriptManager::Update(void) 
  { 
    m_fileWatcher->Update(); 
  }

  std::string ScriptManager::_GetDllName(const std::string &file) const
  {
    size_t pos = file.find_last_of(".");
    SF_ASSERT(pos != std::string::npos, "File has no extension");

    return file.substr(0, pos);
  }

  void ScriptManager::_HandleFileAddedAction(const FileWatcher::ActionInfo &actionInfo)
  {
    std::string fileName = actionInfo.m_fileName;
    std::string scriptName = _RemoveExtension(fileName);

    auto &scriptIt = m_scriptMap.find(scriptName);

    ScriptData *scriptData;
    if (scriptIt == m_scriptMap.end())
    {
      scriptData = new ScriptData;
      m_scriptMap[scriptName] = scriptData;
    }
    else
      scriptData = scriptIt->second;
    
    if (_IsHeader(fileName))
    {
      scriptData->m_header = fileName;
      scriptData->m_relativePath = actionInfo.m_path;
      m_compiler->AddIncludeFolderRelative(scriptData->m_relativePath);
    }
    else
      scriptData->m_cpp = fileName;

    scriptData->m_dllName = scriptName;

    //Compile only when both header and cpp are located
    if (!scriptData->m_cpp.empty() && !scriptData->m_header.empty())
    {
      bool compilationRes 
        = m_compiler->Compile(scriptData->m_relativePath + scriptData->m_cpp, scriptName);

      if (compilationRes)
      {
        BOOL res = 1;
        if (scriptData->m_libHandle != NULL)
        {
          res = FreeLibrary(scriptData->m_libHandle);
          SF_ASSERT(res != 0, "Dll cannot be freed");
        }

        //Unload scripts here

        scriptData->m_libHandle = LoadLibrary((scriptData->m_dllName + ".dll").c_str());
        SF_ASSERT(scriptData->m_libHandle != NULL, "Dll was not loaded");

        scriptData->m_compiled = true;
        m_compiler->AddIncludeFolderRelative(scriptData->m_relativePath + scriptData->m_header);
      }
    }
  }

  void ScriptManager::_HandleFileModifiedAction(const FileWatcher::ActionInfo &actionInfo)
  {

  }

  void ScriptManager::_HandleFileRemovedAction(const FileWatcher::ActionInfo &actionInfo)
  {

  }

  void ScriptManager::_HandleFileRenamedOldAction(const FileWatcher::ActionInfo &actionInfo)
  {

  }

  void ScriptManager::_HandleFileRenamedNewAction(const FileWatcher::ActionInfo &actionInfo)
  {

  }

  bool ScriptManager::_IsHeader(const std::string &fileName) const
  {
    return fileName.find(".hpp") != std::string::npos;
  }

  std::string ScriptManager::_RemoveExtension(const std::string &fileName) const
  {
    size_t pos = fileName.find_last_of(".");

    SF_ASSERT(pos != std::string::npos, "File name does not have an extension");

    return fileName.substr(0, pos);
  }

  std::vector<std::string> ScriptManager::_GatherHeadersPathes(void) const
  {
    std::vector<std::string> pathes;

    for (auto &it : m_scriptMap)
    { 
      if (!it.second->m_relativePath.empty())
        pathes.push_back(it.second->m_relativePath);
    }

    return pathes;
  }
}