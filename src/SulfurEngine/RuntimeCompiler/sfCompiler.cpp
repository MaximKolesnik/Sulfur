/******************************************************************************/
/*!
\par     Sulfur
\file    sfCompiler.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/24/2016

\brief   Runtime dll complier

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <Shlwapi.h>
#include <iostream>

#include "sfCompiler.hpp"
#include "Settings\sfEngineSettings.hpp"

namespace Sulfur
{

  void CompilerOutputWorker(LPVOID a)
  {
    Compiler *compiler = reinterpret_cast<Compiler*>(a);
    CHAR buffer[2048];
    DWORD bytesRead = 0;

    while (!compiler->m_destroy)
    {
      if (!ReadFile(compiler->m_cmdOutput, buffer, sizeof(buffer) - 1, &bytesRead, NULL))
      {
        SF_CRITICAL_ERR(std::to_string(GetLastError()));
      }
      else
      {
        buffer[bytesRead] = 0;
        std::string stringBuffer(buffer);

        size_t donePos = stringBuffer.find(compiler->c_compilationComplete);

        if (donePos == std::string::npos)
        {
          size_t errorPos = stringBuffer.find(": error ");
          size_t fatalErrorPos = stringBuffer.find(": fatal error ");
          size_t cmdLineErrosPos = stringBuffer.find(": Command line error");
          
          if (errorPos != std::string::npos 
            || 
            fatalErrorPos != std::string::npos
            ||
            cmdLineErrosPos != std::string::npos)
          {
            compiler->m_compilationResMutex.lock();
            compiler->m_compiledWithoutErrors = false;
            compiler->m_compilationResMutex.unlock();
          }

          std::cout << buffer << std::endl;
        }
        else
        {
          std::cout << stringBuffer.substr(0, donePos) << std::endl;
          std::cout << "Compilation is done\n";
          
          compiler->m_compDoneMutex.lock();
          compiler->m_compilationDone = true;
          compiler->m_compDoneMutex.unlock();
        }
      }
    }
  }

  Compiler::Compiler(void) : m_cmdInput(NULL), m_cmdOutput(NULL), m_compiledWithoutErrors(true),
    m_compilationDone(true), m_destroy(false)
  {
    ZeroMemory(&m_cmdInfo, sizeof(m_cmdInfo));
  }

  void Compiler::Initialize(void)
  {
    _GetVSPath();
    _StartCompilerProcess();
  }

  void Compiler::SetWorkingDirectory(const std::string &workingDirectory)
  {
    m_workingDir = workingDirectory;

    if (workingDirectory.back() != '\\')
      m_workingDir += "\\";
  }

  void Compiler::SetOutputDirectoryRelative(const std::string &outputDir)
  {
    m_outputDir = m_workingDir + outputDir;
  }

  void Compiler::SetIntermediateDirectoryRealtive(const std::string &interDir)
  {
    m_interDir = m_workingDir + interDir;
  }

  void Compiler::SetIncludePathesRelative(const std::vector<std::string> &includePathes)
  {
    m_includePathes = includePathes;
  }

  void Compiler::AddIncludeFolderRelative(const std::string &includeFolder)
  {
    m_includePathes.push_back(includeFolder);
  }

  bool Compiler::Compile(const std::string &cpp, const std::string &dllName)
  {
    SF_ASSERT(!m_VSPath.empty(), "Compiler is not supported");

    m_compilationResMutex.lock();
    m_compiledWithoutErrors = true;
    m_compilationResMutex.unlock();

    m_compDoneMutex.lock();
    m_compilationDone = false;
    m_compDoneMutex.unlock();

    struct stat info;
    BOOL res = true;

    if (stat(m_outputDir.c_str(), &info) != 0)
      BOOL res = CreateDirectory(m_outputDir.c_str(), NULL);

    SF_CRITICAL_ERR_EXP(res,
      "Script output dll directory does not exist and cannot be created");

    res = true;

    if (stat(m_interDir.c_str(), &info) != 0)
      BOOL res = CreateDirectory(m_interDir.c_str(), NULL);

    SF_CRITICAL_ERR_EXP(res,
      "Script output obj directory does not exist and cannot be created");

    std::string source = m_workingDir + cpp + " ";
    std::string compilerFlags = " /W3 /WX /O2 /MD /LD /EHsc ";
    std::string outputFileName = dllName + ".dll";
    std::string output = " /link /out:\"" + m_outputDir + outputFileName + "\" " ;
    std::string interm = " /Fo\"" + m_interDir + dllName + ".obj\" ";

    std::string command = "cl " + source + _ConstructIncludeString() + interm + compilerFlags + output;
    command += "\necho " + c_compilationComplete + "\n";

    _Write(command);

    _WaitForCompletion();

    return m_compiledWithoutErrors;
  }

  Compiler::~Compiler(void)
  {
    m_destroy = true;
    _Write("echo Compiler is exiting\n");

    m_outputWorker->join();
    delete m_outputWorker;

    TerminateProcess(m_cmdInfo.hProcess, 1);

    CloseHandle(m_cmdOutput);
    CloseHandle(m_cmdInput);
  }

  void Compiler::_GetVSPath(void)
  {
   std::string regKey = "SOFTWARE\\Microsoft\\VisualStudio\\SxS\\VC7";

   const std::string versions[] = {"8.0", "9.0", "10.0", "11.0", "12.0", "14.0"};
   const size_t numVersions = 6;

   int startVersion = numVersions - 1;

   UINT32 msvVer = _MSC_VER;
   switch (msvVer)
   {
   case 1400:
     startVersion = 0;
     break;
   case 1500:
     startVersion = 1;
     break;
   case 1600:
     startVersion = 2;
     break;
   case 1700:
     startVersion = 3;
     break;
   case 1800:
     startVersion = 4;
     break;
   case 1900:
     startVersion = 5;
     break;
   default:
     SF_CRITICAL_ERR("Compiler is not supported");
   }

   HKEY key;
   LONG retVal = RegOpenKeyExA(HKEY_LOCAL_MACHINE, regKey.c_str(), 0, 
     KEY_READ | KEY_WOW64_32KEY, &key);

   SF_CRITICAL_ERR_EXP(retVal == ERROR_SUCCESS, "Cannot open RegKey");

   m_VSPath = "";
   std::vector<std::string> foundVersions;

   int numLoops = 1;
   if (startVersion != numVersions - 1)
     numLoops = 2;

   char buffer[2048];
   DWORD bufferSize = 2048;

   for (int loop = 0; loop < numLoops; ++loop)
   {
     for (int i = startVersion; i >= 0; --i)
     {
       LONG retVal = RegQueryValueExA(key, versions[i].c_str(), 0, 0, 
         (LPBYTE)buffer, &bufferSize);
       if (retVal == ERROR_SUCCESS)
       {
         foundVersions.push_back(buffer);
       }
     }
     startVersion = numVersions - 1;
   }

   SF_CRITICAL_ERR_EXP(!foundVersions.empty(), "Failed to get VS path");

   m_VSPath = foundVersions[0];
  }

  void Compiler::_StartCompilerProcess(void)
  {
    std::string cmdCommand = "\"" + m_VSPath + "vcvarsall.bat\" x86_amd64\n";

    SECURITY_ATTRIBUTES secAttrs;
    secAttrs.nLength = sizeof(SECURITY_ATTRIBUTES);
    secAttrs.lpSecurityDescriptor = NULL;
    secAttrs.bInheritHandle = TRUE;

    STARTUPINFO startInfo;
    ZeroMemory(&startInfo, sizeof(startInfo));
    startInfo.cb = sizeof(startInfo);
    startInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    startInfo.wShowWindow = SW_SHOWMAXIMIZED;

    HANDLE handleOutReadTemp = NULL;
    HANDLE handleOutWrite = NULL;
    if (!CreatePipe(&handleOutReadTemp, &handleOutWrite, &secAttrs, 4096))
      SF_CRITICAL_ERR("Cannot create output redirection");

    startInfo.hStdOutput = handleOutWrite;

    HANDLE handleErrorWrite = NULL;
    if (!DuplicateHandle(GetCurrentProcess(), handleOutWrite, GetCurrentProcess(),
      &handleErrorWrite, 0, TRUE, DUPLICATE_SAME_ACCESS))
      SF_CRITICAL_ERR("Cannot duplicate error output");

    startInfo.hStdError = handleErrorWrite;

    if (startInfo.hStdOutput)
    {
      if (!DuplicateHandle(GetCurrentProcess(), handleOutReadTemp, GetCurrentProcess(),
        &m_cmdOutput, 0, FALSE, DUPLICATE_SAME_ACCESS))
        SF_CRITICAL_ERR("Cannot duplicate output");

      CloseHandle(handleOutReadTemp);
      handleOutReadTemp = NULL;
    }

    HANDLE handleInRead = NULL;
    HANDLE handleInWriteTemp = NULL;
    if (!CreatePipe(&handleInRead, &handleInWriteTemp, &secAttrs, 4096))
      SF_CRITICAL_ERR("Cannot create input redirection");

    startInfo.hStdInput = handleInRead;
    
    if (startInfo.hStdOutput)
    {
      if (!DuplicateHandle(GetCurrentProcess(), handleInWriteTemp,
        GetCurrentProcess(), &m_cmdInput, 0, FALSE, DUPLICATE_SAME_ACCESS))
        SF_CRITICAL_ERR("Cannot duplicate input");
    }

    char *commandLine = "cmd ";
    CreateProcess(NULL, commandLine, NULL, NULL, TRUE, 0, NULL, NULL, &startInfo,
      &m_cmdInfo);

    _Write(cmdCommand);

    m_outputWorker = new std::thread(CompilerOutputWorker, this);

    if (handleOutReadTemp)
      CloseHandle(handleOutReadTemp);
    if (handleOutWrite)
      CloseHandle(handleOutWrite);
    if (handleErrorWrite)
      CloseHandle(handleErrorWrite);
  }

  void Compiler::_Write(const std::string &input)
  {
    DWORD bytes;
    WriteFile(m_cmdInput, input.c_str(), (DWORD)input.length(), &bytes, NULL);
  }
  
  void Compiler::_WaitForCompletion(void)
  {
    while (true)
    {
      m_compDoneMutex.lock();
      if (m_compilationDone) return;
      m_compDoneMutex.unlock();
    }
  }

  std::string Compiler::_RemoveExtension(const std::string &file) const
  {
    size_t pos = file.find_last_of(".");
    SF_ASSERT(pos != std::string::npos, "File has no extension");
    return file.substr(0, pos);
  }

  std::string Compiler::_ConstructIncludeString(void) const
  {
    std::string includeString;
    char buffer[MAX_PATH];

    for (auto &it : m_includePathes)
    {
      std::string path = m_workingDir + it;
      path.pop_back();
      PathCanonicalize(buffer, path.c_str());

      includeString += "/I\"" + std::string(buffer) + "\" ";
    }

    return includeString;
  }
}
