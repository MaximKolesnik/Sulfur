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
#include "Factories\sfComponentFactory.hpp"

namespace Sulfur
{
  void FileWatcherCallback(const FileWatcher::ActionInfo &actionInfo)
  {
    switch (actionInfo.m_action)
    {
    case FileWatcher::Action::FileAdded:
      g_SystemTable->ScriptManager->_HandleFileAddedAction(actionInfo);
      break;
    case FileWatcher::Action::FileModified:
      g_SystemTable->ScriptManager->_HandleFileModifiedAction(actionInfo);
      break;
    case FileWatcher::Action::FileRemoved:
      g_SystemTable->ScriptManager->_HandleFileRemovedAction(actionInfo);
      break;
    case FileWatcher::Action::FileRenamedNew:
      g_SystemTable->ScriptManager->_HandleFileRenamedNewAction(actionInfo);
      break;
    case FileWatcher::Action::FileRenamedOld:
      g_SystemTable->ScriptManager->_HandleFileRenamedOldAction(actionInfo);
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

    for (auto it : m_scriptMap)
    {
      ScriptData *data = it.second;
      if (data->m_compiled)
      {
        g_SystemTable->CompFactory->_RemoveScript(data->m_scriptName);
        FreeLibrary(data->m_libHandle);
      }
    }
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
    std::string libPath;

    PathCanonicalize(buffer, scriptPath.c_str());
    m_scriptFolder = buffer;
    m_compiler->SetWorkingDirectory(m_scriptFolder);
    m_fileWatcher->AddDirectoryToWatch(std::string(buffer));

    m_compiler->AddIncludeFolderRelative("..\\SulfurEngine\\");

    std::string interFolder = m_scriptFolder + "\\..\\..\\bin\\Scripts\\";
    std::string outFolder = m_scriptFolder + "\\..\\..\\bin\\Scripts\\";

#ifdef _DEBUG
    std::string engineLib = m_scriptFolder + "\\..\\..\\builds\\bin\\SulfurEngine\\vs2015\\SulfurEngine_d.lib";
    std::string dependencyPath = m_scriptFolder + "\\..\\..\\dependencies\\";
    PathCanonicalize(buffer, dependencyPath.c_str());
    dependencyPath = buffer;
    m_compiler->AddDependencyLibrary(dependencyPath + "fbx\\lib\\libfbxsdk-mdd.lib");
    m_compiler->AddDependencyLibrary(dependencyPath + "libjpeg\\lib\\jpeg.lib");
    m_compiler->AddDependencyLibrary(dependencyPath + "libpng\\lib\\libpng16d.lib");
    m_compiler->AddDependencyLibrary(dependencyPath + "zlib\\lib\\zlibd.lib");
    m_compiler->AddDependencyLibrary("d3dcompiler.lib");
    m_compiler->AddDependencyLibrary("d3d11.lib");
    m_compiler->AddDependencyLibrary("dxgi.lib");
    m_compiler->AddDependencyLibrary("dxguid.lib");

#else
    std::string engineLib = m_scriptFolder + "\\..\\..\\builds\\bin\\SulfurEngine\\vs2015\\SulfurEngine.lib";
    std::string dependencyPath = m_scriptFolder + "\\..\\..\\dependencies\\";
    PathCanonicalize(buffer, dependencyPath.c_str());
    dependencyPath = buffer;
    m_compiler->AddDependencyLibrary(dependencyPath + "fbx\\lib\\libfbxsdk-md.lib");
    m_compiler->AddDependencyLibrary(dependencyPath + "libjpeg\\lib\\jpeg.lib");
    m_compiler->AddDependencyLibrary(dependencyPath + "zlib\\lib\\zlib.lib");
    m_compiler->AddDependencyLibrary("d3dcompiler.lib");
    m_compiler->AddDependencyLibrary(dependencyPath + "libpng\\lib\\libpng16.lib");
    m_compiler->AddDependencyLibrary("d3d11.lib");
    m_compiler->AddDependencyLibrary("dxgi.lib");
    m_compiler->AddDependencyLibrary("dxguid.lib");
#endif

    PathCanonicalize(buffer, interFolder.c_str());
    m_interFolder = buffer;
    PathCanonicalize(buffer, outFolder.c_str());
    m_dllFolder = buffer;
    PathCanonicalize(buffer, engineLib.c_str());
    m_engineLib = buffer;

    m_compiler->SetOutputDirectory(m_dllFolder);
    m_compiler->SetIntermediateDirectory(m_interFolder);
    m_compiler->SetEngineLibrary(m_engineLib);

    _InitializeScriptData();
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
    std::string scriptFileName = _RemoveExtension(fileName);

    auto &scriptIt = m_scriptMap.find(scriptFileName);

    ScriptData *scriptData;
    if (scriptIt == m_scriptMap.end())
    {
      scriptData = new ScriptData;
      m_scriptMap[scriptFileName] = scriptData;
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

    scriptData->m_dllName = scriptFileName;

    //Compile only when both header and cpp are located
    if (!scriptData->m_cpp.empty() && !scriptData->m_header.empty())
    {
      bool compilationRes 
        = m_compiler->Compile(scriptData->m_relativePath + scriptData->m_cpp, scriptFileName);

      if (compilationRes)
      {
        scriptData->m_compiled = true;

        scriptData->m_libHandle = LoadLibrary((m_dllFolder + scriptData->m_dllName + ".dll").c_str());
        SF_ASSERT(scriptData->m_libHandle != NULL, "Dll was not loaded");
                          
        getTableFunc tableFunc = (getTableFunc)GetProcAddress(scriptData->m_libHandle, "SFGetTable");
        SF_ASSERT(tableFunc != NULL, "Script registration is not located");

        const FunctionTable::FUNCTABLE& table = tableFunc();

        RegFunc regFunc = nullptr;
        RegName regNameFunc = nullptr;

        _GetFunctionsFromTable(scriptData->m_header, table, &regFunc, &regNameFunc);

        SF_ASSERT(regFunc != nullptr, "Script registration is not located");
        SF_ASSERT(regNameFunc != nullptr, "Script fin registration is not located");

        ISlotMap *slotMap = regFunc(g_SystemTable);
        scriptData->m_scriptName = regNameFunc();

        g_SystemTable->CompFactory->_InsertNewScript(scriptData->m_scriptName, slotMap);
      }
    }

    //Consume one Modification update
  }

  void ScriptManager::_HandleFileModifiedAction(const FileWatcher::ActionInfo &actionInfo)
  {
    std::string fileName = actionInfo.m_fileName;
    std::string scriptFileName = _RemoveExtension(fileName);

    SF_ASSERT(m_scriptMap.find(scriptFileName) != m_scriptMap.end(), "Cannot locate the script");

    ScriptData *scriptData = m_scriptMap.find(scriptFileName)->second;

    if (!scriptData->m_compiled) //If the script is not compiled there is nothing to do
      return;

    //Try to compile dll
    std::string tempDllName = "Temp" + scriptFileName;

    bool compilationResult = m_compiler->Compile(scriptData->m_relativePath + scriptData->m_cpp,
      tempDllName);

    if (compilationResult)
    {
      std::vector<std::pair<HNDL, HNDL> > hndls
        = g_SystemTable->CompFactory->_DeallocateScripts(scriptData->m_scriptName);

      BOOL res = FreeLibrary(scriptData->m_libHandle);
      SF_CRITICAL_ERR_EXP(res != NULL, "Cannot unload script");

      _DeleteFile(scriptFileName + "*");
      _RenameFiles(tempDllName, scriptData->m_dllName);

      scriptData->m_libHandle = LoadLibrary((m_dllFolder + scriptData->m_dllName + ".dll").c_str());
      SF_ASSERT(scriptData->m_libHandle != NULL, "Dll was not loaded");

      getTableFunc tableFunc = (getTableFunc)GetProcAddress(scriptData->m_libHandle, "SFGetTable");
      SF_ASSERT(tableFunc != NULL, "Script registration is not located");

      const FunctionTable::FUNCTABLE& table = tableFunc();

      RegFunc regFunc = nullptr;
      RegName regNameFunc = nullptr;

      _GetFunctionsFromTable(scriptData->m_header, table, &regFunc, &regNameFunc);

      SF_ASSERT(regFunc != nullptr, "Script registration is not located");
      SF_ASSERT(regNameFunc != nullptr, "Script fin registration is not located");

      ISlotMap *slotMap = regFunc(g_SystemTable);
      
      SF_CRITICAL_ERR_EXP(scriptData->m_scriptName == regNameFunc(), 
        "Cannot change script name on runtime");

      g_SystemTable->CompFactory->_RestoreScripts(hndls, scriptData->m_scriptName, slotMap);
    }
    else
      _DeleteFile(tempDllName + "*");

    //Consume one
  }

  void ScriptManager::_HandleFileRemovedAction(const FileWatcher::ActionInfo &actionInfo)
  {
    std::string fileName = actionInfo.m_fileName;
    std::string scriptFileName = _RemoveExtension(fileName);

    SF_ASSERT(m_scriptMap.find(scriptFileName) != m_scriptMap.end(), "Cannot locate the script");

    ScriptData *scriptData = m_scriptMap.find(scriptFileName)->second;

    if (scriptData->m_compiled) //Remove dll
    {
      g_SystemTable->CompFactory->_RemoveScript(scriptData->m_scriptName);
      BOOL res = FreeLibrary(scriptData->m_libHandle);
      SF_CRITICAL_ERR_EXP(res != NULL, "Cannot unload script");

      scriptData->m_compiled = false;
      scriptData->m_libHandle = NULL;
    }

    if (_IsHeader(fileName))
    {
      scriptData->m_header = "";
      if (scriptData->m_relativePath != "")
        m_compiler->RemoveIncludeFolderRelative(scriptData->m_relativePath);
    }
    else
      scriptData->m_cpp = "";

    if (scriptData->m_cpp.empty() && scriptData->m_header.empty())
      m_scriptMap.erase(scriptFileName);
  }

  void ScriptManager::_HandleFileRenamedOldAction(const FileWatcher::ActionInfo &actionInfo)
  {
    SF_CRITICAL_ERR("NOT IMPLEMENTED");
  }

  void ScriptManager::_HandleFileRenamedNewAction(const FileWatcher::ActionInfo &actionInfo)
  {
    SF_CRITICAL_ERR("NOT IMPLEMENTED");
  }

  bool ScriptManager::_IsHeader(const std::string &fileName) const
  {
    return fileName.find(".hpp") != std::string::npos;
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

  std::string ScriptManager::_RemoveExtension(const std::string &fileName) const
  {
    size_t pos = fileName.find_last_of(".");

    SF_ASSERT(pos != std::string::npos, "File name does not have an extension");

    return fileName.substr(0, pos);
  }

  void ScriptManager::_DeleteFile(const std::string &file) const
  {
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile(file.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
      do
      {
        DeleteFile(fd.cFileName);
      } while (FindNextFile(hFind, &fd));
      FindClose(hFind);
    }
  }

  void ScriptManager::_RenameFiles(const std::string &dllName, 
    const std::string &newName) const
  {
    WIN32_FIND_DATA fd;
    HANDLE hFind = FindFirstFile((dllName + "*").c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE)
    {
      do
      {
        std::string fileName = fd.cFileName;
        size_t pos = fileName.find_last_of(".");

        SF_ASSERT(pos != std::string::npos, "File name does not have an extension");

        std::string ext = fileName.substr(pos);
        MoveFile(fileName.c_str(), (newName + ext).c_str());

      } while (FindNextFile(hFind, &fd));
      FindClose(hFind);
    }
  }

  void ScriptManager::_InitializeScriptData(void)
  {
    _LocateScripts(m_scriptFolder.c_str());

    for (auto &it : m_scriptMap)
    {
      ScriptData *scriptData = it.second;

      if (scriptData->m_cpp.empty() || scriptData->m_header.empty())
        continue;

      scriptData->m_dllName = _RemoveExtension(scriptData->m_cpp);

      bool compilationRes 
        = m_compiler->Compile(scriptData->m_relativePath + scriptData->m_cpp, scriptData->m_dllName);

      SF_CRITICAL_ERR_EXP(compilationRes, (scriptData->m_cpp + " cannot be compiled"));

      scriptData->m_compiled = true;

      scriptData->m_libHandle = LoadLibrary((m_dllFolder + scriptData->m_dllName + ".dll").c_str());
      SF_ASSERT(scriptData->m_libHandle != NULL, "Dll was not loaded");

      getTableFunc tableFunc = (getTableFunc)GetProcAddress(scriptData->m_libHandle, "SFGetTable");
      SF_ASSERT(tableFunc != NULL, "Script registration is not located");

      const FunctionTable::FUNCTABLE& table = tableFunc();

      RegFunc regFunc = nullptr;
      RegName regNameFunc = nullptr;

      _GetFunctionsFromTable(scriptData->m_header, table, &regFunc, &regNameFunc);

      SF_ASSERT(regFunc != nullptr, "Script registration is not located");
      SF_ASSERT(regNameFunc != nullptr, "Script fin registration is not located");

      ISlotMap *slotMap = regFunc(g_SystemTable);
      scriptData->m_scriptName = regNameFunc();

      g_SystemTable->CompFactory->_InsertNewScript(scriptData->m_scriptName, slotMap);
    }
  }

  void ScriptManager::_LocateScripts(LPCTSTR folder)
  {
    WIN32_FIND_DATA findData;
    HANDLE hFindFile;
    TCHAR path[MAX_PATH];

    PathCombine(path, folder, "?");
    hFindFile = FindFirstFile(path, &findData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
      do
      {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY
          &&
          strcmp(findData.cFileName, ".") != 0
          &&
          strcmp(findData.cFileName, "..") != 0)
        {
          PathCombine(path, folder, findData.cFileName);
          _LocateScripts(path);
        }
      } while (FindNextFile(hFindFile, &findData));
      FindClose(hFindFile);
    }

    PathCombine(path, folder, "*.cpp");
    hFindFile = FindFirstFile(path, &findData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
      if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
        std::string fileName = _RemoveExtension(findData.cFileName);
        ScriptData *newData = new ScriptData();
        newData->m_cpp = findData.cFileName;
        newData->m_relativePath = std::string(folder).erase(0, m_scriptFolder.size());
        m_scriptMap[fileName] = newData;
      }
    } while (FindNextFile(hFindFile, &findData));
    FindClose(hFindFile);

    PathCombine(path, folder, "*.h*");
    hFindFile = FindFirstFile(path, &findData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
      if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
        std::string fileName = _RemoveExtension(findData.cFileName);
        auto it = m_scriptMap.find(fileName);

        ScriptData *data;
        if (it != m_scriptMap.end())
        {
          data = m_scriptMap[fileName];
          data->m_header = findData.cFileName;
        }
        else
        {
          data = new ScriptData();
          data->m_header = findData.cFileName;
          data->m_relativePath = std::string(folder).erase(0, m_scriptFolder.size());
          m_scriptMap[fileName] = data;
        }
      }
    } while (FindNextFile(hFindFile, &findData));
    FindClose(hFindFile);
  }

  void ScriptManager::_GetFunctionsFromTable(const std::string &scriptHeader,
    const FunctionTable::FUNCTABLE &table, RegFunc *regFunc, RegName *regName) const
  {
    for (auto &it : table)
    {
      if (it.first.find(scriptHeader) != std::string::npos)
      {
        *regFunc = it.second.first;
        *regName = it.second.second;
        return;
      }
    }

    SF_CRITICAL_ERR("Cannot find reg functions in dll");
  }
}