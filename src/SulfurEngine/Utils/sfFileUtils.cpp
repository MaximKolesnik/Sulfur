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
#include "sfFileUtils.hpp"
#include <sys/stat.h>
#include <fstream>

namespace Sulfur
{

bool FileUtils::FileExists(const std::string& filePath)
{
  struct stat fileStat;
  return stat(filePath.c_str(), &fileStat) == 0;
}

UINT32 FileUtils::GetFileSize(const std::string& filePath)
{
  struct stat fileStat;
  if (stat(filePath.c_str(), &fileStat) != 0)
    return 0;

  return (UINT32)fileStat.st_size;
}

BYTE* FileUtils::ReadFile(const std::string& filePath)
{
  UINT32 fileSize = GetFileSize(filePath);
  if (fileSize == 0) return nullptr;

  BYTE *buffer = new BYTE[fileSize];
  std::ifstream stream(filePath.c_str(), std::ios_base::binary);
  stream.read((char*)buffer, fileSize);

  return buffer;
}

}
