#include "sfEngineSettings.hpp"

namespace Sulfur
{
  //namespace
  //{
  //  static bool dirExists(const std::string& dirName_in)
  //  {
  //    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  //    if (ftyp == INVALID_FILE_ATTRIBUTES)
  //      return false;

  //    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
  //      return true;

  //    return false;
  //  }

  //  static std::string GetVCVarsPath(void)
  //  {
  //    FILE *cmdPtr;
  //    char buffer[1024];

  //    SF_CRITICAL_ERR_EXP(system("findVSTools") == 0, "Visual Studio 2015 not found");

  //    cmdPtr = _popen("findVSTools", "rt");
  //    SF_CRITICAL_ERR_EXP(cmdPtr != NULL, "Cannot execute VS tools lookup");

  //    fgets(buffer, sizeof(buffer), cmdPtr);

  //    std::string path = buffer;
  //    path.pop_back(); //Remove \n

  //    SF_CRITICAL_ERR_EXP(dirExists(path), "CPath was not found");

  //    _pclose(cmdPtr);
  //    return path;
  //  }
  //}

  //namespace EngineSettings
  //{
  //  const std::string VCVarsPath = GetVCVarsPath();
  //}
}