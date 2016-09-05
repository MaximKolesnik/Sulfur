#pragma once

#include <string>

namespace Sulfur
{
  namespace EngineSettings
  {
    //Paths
    const std::string LogDir = "Logs\\";

    //SlotMap
    const INT64 SlotMapObjsPerPage    = 512;
    const INT64 SlotMapInitNumOfPages = 3;

    //Frame
    const Real DefaultFPS = Real(1.0) / 60;
  };
}