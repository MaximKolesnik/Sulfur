/******************************************************************************/
/*!
\par     Sulfur
\file    sfLogger.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/24/2016

\brief   Saves the messages in Logs sorted by date and time.
         Most of the Warnings and Asserts in sfError.hpp are paired with it

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fileapi.h>
#include <iostream>

#include <ctime>

#include "sfLogger.hpp"
#include "../Settings/sfEngineSettings.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{
  Logger::Logger(void)
  {
    // Show console window
    if (EngineSettings::ShowConsole)
    {
      AllocConsole();
      freopen("CONOUT$", "w", stdout);
    }

    //Get session start time
    time_t t = time(0);
    struct tm *now = localtime(&t);
    int month = now->tm_mon + 1, day = now->tm_mday;

    std::string logName = std::to_string(month) + "_" + std::to_string(day)
      + "__";

    //Format time hhmmss;
    char buffer[9];
    strftime(buffer, sizeof(buffer), "%H_%M_%S", now);

    logName += buffer;
    logName += ".txt";

    //Create directory if it does not exist
    struct stat info;
    BOOL directoryExists = true;

    if (stat(EngineSettings::LogDir.c_str(), &info) != 0)
      directoryExists = CreateDirectory(EngineSettings::LogDir.c_str(), NULL);

    SF_CRITICAL_ERR_EXP(directoryExists,
      "Log directory does not exist and cannot be created");

    //Create log file
    m_logFile.open((EngineSettings::LogDir + logName).c_str(), std::ofstream::out);
    if (!m_logFile.is_open())
      SF_CRITICAL_ERR("Log file cannot be created");
  }

  Logger::~Logger(void)
  {
    m_logFile.close();                                                          
  }

  void Logger::Log(const std::string &message, const std::string &fileName,
    const std::string &functionName, UINT64 lineNumber, const std::string &type)
  {
    std::string finalMessage;

    std::string time = _GetCurrentTime();
    finalMessage += time + " ";
    
    finalMessage += "FILE: " + _RemoveFullPathFileName(fileName) + " ";
    finalMessage += "FUNCTION: " + functionName + " ";
    finalMessage += "LINE: " + std::to_string(lineNumber) + " ";
    finalMessage += type + ": " + message + "\n";

    m_logFile << finalMessage << std::endl;

    //if (EngineSettings::ShowConsole)
      std::cout << finalMessage;
  }

  std::string Logger::_GetCurrentTime(void) const
  {
    time_t t = time(0);
    struct tm *now = localtime(&t);

    char buffer[9];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", now);

    return buffer;
  }

  std::string Logger::_RemoveFullPathFileName(const std::string &fileName) const
  {
    std::string::size_type pos = fileName.find_last_of("\\");

    return fileName.substr(pos + 1, std::string::npos);
  }
}