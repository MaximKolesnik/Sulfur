/******************************************************************************/
/*!
\par     Sulfur
\file    sfFileWatcher.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/26/2016

\brief   File wathcer

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

namespace Sulfur
{
  class FileWatcher
  {
  private:
    //Number of files to support in one event call
    size_t c_numFiles = 512;

  public:
    struct WatchInfo
    {
      OVERLAPPED m_overlapped;
      std::string m_dir;
      HANDLE m_dirHandle;
      bool m_isRecursive;

      bool m_stop;

      BYTE *m_buffer;
      DWORD m_bufferSize;
    };

    FileWatcher(void);
    ~FileWatcher(void);

    bool AddDirectoryToWatch(const std::string &dir, bool recursive = true);
    void RemoveDirectoryFromWatch(const std::string &dir);
    void Update(void);
  private:

    void _StopWatch(WatchInfo *info) const;

    std::unordered_map<std::string, WatchInfo*> m_dirsMap;
  };
}