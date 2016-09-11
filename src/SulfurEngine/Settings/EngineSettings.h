#pragma once

#include <string>

namespace Sulfur
{
  namespace EngineSettings
  {
    // Logging
    const std::string LogDir = "logs\\";
    const bool ShowConsole   = true;

    // SlotMap
    const INT64 SlotMapObjsPerPage    = 512;
    const INT64 SlotMapInitNumOfPages = 3;
  };
}