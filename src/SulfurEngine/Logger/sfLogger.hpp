/******************************************************************************/
/*!
\par     Sulfur
\file    sfLogger.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/24/2016

\brief   Saves the messages in Logs sorted by date and time.
         Most of the Warnings and Asserts in sfError.hpp are paired with it

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include <fstream>
#include <assert.h>

namespace Sulfur
{
  class Logger
  {
  public:
    static Logger* Instance(void);

    void Log(const std::string &message, const std::string &fileName,
      const std::string &functionName, UINT64 lineNumber, const std::string &type);

  private:
    static Logger *m_instance;

    Logger() {};
    ~Logger(void);

    void Initialize(void);

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    //HH:MM:SS
    std::string _GetCurrentTime(void) const;
    std::string _RemoveFullPathFileName(const std::string &fileName) const;

    std::ofstream m_logFile;
  };

#define SF_LOG_MESSAGE(message) \
Sulfur::Logger::Instance()->Log(message, __FILE__, __FUNCTION__, __LINE__, "MESSAGE")

}