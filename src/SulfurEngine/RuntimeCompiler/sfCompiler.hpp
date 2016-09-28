/******************************************************************************/
/*!
\par     Sulfur
\file    sfCompiler.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/24/2016

\brief   Runtime dll complier

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/


#pragma once

#include <vector>
#include <thread>
#include <mutex>

namespace Sulfur
{
  void CompilerOutputWorker(LPVOID a);

  class Compiler
  {
  public:
    Compiler(void);
    ~Compiler(void);

    void Initialize(void);

    void SetOutputDirectory(const std::string &outputDir) { m_outputDir = outputDir; }
    void SetIntermediateDirectory(const std::string &interDir) { m_interDir = interDir; }

    //Should have fullpath to files and space separated
    bool Compile(const std::string &cpps, const std::string &headers,
      const std::string &dllName);
    bool IsCompilationDone(void) const { return m_compilationDone; }

  private:
    friend void CompilerOutputWorker(LPVOID a);

    void _GetVSPath(void);
    void _StartCompilerProcess(void);
    void _Write(const std::string &input);
    void _WaitForCompletion(void);
    std::string _RemoveExtension(const std::string &file) const;

    const std::string c_compilationComplete = "__COMPILATION_FINISHED__";

    std::string m_VSPath;

    PROCESS_INFORMATION m_cmdInfo;
    HANDLE m_cmdOutput;
    HANDLE m_cmdInput;

    bool m_compiledWithoutErrors;

    bool m_compilationDone;
    std::mutex m_compDoneMutex;
    std::mutex m_compilationResMutex;

    std::thread *m_outputWorker;

    bool m_destroy;

    //Dirs
    std::string m_outputDir;
    std::string m_interDir;
  };
}