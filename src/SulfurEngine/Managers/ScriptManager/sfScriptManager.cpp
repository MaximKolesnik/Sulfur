#include <Shlwapi.h>

#include "sfScriptManager.hpp"
#include "Types\sfIEntity.hpp"
#include "Logger\sfLogger.hpp"
#include "Settings\sfEngineSettings.hpp"
#include "RuntimeCompiler\sfCompiler.hpp"
#include "FileWatcher\sfFileWatcher.hpp"

namespace Sulfur
{
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
    m_fileWatcher = new FileWatcher();

    //Get script directory path
    char buffer[MAX_PATH];
    GetCurrentDirectory((DWORD)MAX_PATH, buffer);
    std::string s = EngineSettings::ScriptSourceFolder;
    s.pop_back();

    std::string scriptPath = buffer + ("\\" + s);

    PathCanonicalize(buffer, scriptPath.c_str());

    m_fileWatcher->AddDirectoryToWatch(std::string(buffer));

    //m_compiler->Compile("sfTestScript.cpp");
  }

  void ScriptManager::RegisterScript(IEntity *scriptInstance)
  {

  }

  void ScriptManager::Update(void) { m_fileWatcher->Update(); }

  int ScriptManager::_CompileScript(const std::string &scriptFileName) const
  {
    std::string command = "compileScript ";
    command += EngineSettings::ScriptSourceFolder + scriptFileName + " ";
    command += _GetDllName(scriptFileName);

    system("set");

    ShellExecute(::GetConsoleWindow(), "open", command.c_str(), NULL, NULL, SW_SHOWNORMAL);
    return 0;
  }

  std::string ScriptManager::_GetDllName(const std::string &file) const
  {
    size_t pos = file.find_last_of(".");
    SF_ASSERT(pos != std::string::npos, "File has no extension");

    return file.substr(0, pos);
  }

  void ScriptManager::_SetEnvVariables(void) const
  {
    SF_LOG_MESSAGE("Setting environment variables");
    ShellExecute(::GetConsoleWindow(), "open", "\"C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\Common7\\Tools\\vcvars32\"", NULL, NULL, SW_SHOWNORMAL);
    ShellExecute(::GetConsoleWindow(), "open", "set", NULL, NULL, SW_SHOWNORMAL);
    //std::string command = "\"" + EngineSettings::VCVarsPath + "\\vcvarsall\" && set";
    //system(command.c_str());
    system("set");
  }
}