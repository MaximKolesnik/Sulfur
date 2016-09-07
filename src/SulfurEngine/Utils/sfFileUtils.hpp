/******************************************************************************/
/*!
\par     Sulfur
\file    sfFileUtils.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/6/2016

\brief   File utility functions

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class FileUtils
  {
  
  public:
    static bool FileExists(const std::string& filePath);
    static UINT32 GetFileSize(const std::string& filePath);
    static BYTE* ReadFile(const std::string& filePath);

  };
  
}