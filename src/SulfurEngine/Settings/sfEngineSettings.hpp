#pragma once

#include <string>

namespace Sulfur
{
  namespace EngineSettings
  {
    //Paths
    const std::string LogDir = "Logs\\";
    const bool ShowConsole = false;

    //SlotMap
    const INT64 SlotMapObjsPerPage    = 512;
    const INT64 SlotMapInitNumOfPages = 3;

    //Frame
    const Real DefaultFPS = Real(1.0) / 60;

    //Fiber
    const UINT64 FiberStackSize = 0;          //If 0, uses default stack size for exe
    const UINT64 FiberReservedStackSize = 0;  //If 0, uses default reserved stack size for exe

    //Scripts
    const std::string ScriptDllOutputDir = "Scripts\\";
    const std::string ScriptSourceFolder = "..\\src\\SulfurScripts\\";
    const std::string ScriptIntermDir = "..\\builds\\obj\\Scripts\\";
  };
}