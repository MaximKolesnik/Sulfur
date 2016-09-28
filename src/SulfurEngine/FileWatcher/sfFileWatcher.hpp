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
    enum Action
    {
      FileAdded = FILE_ACTION_ADDED,
      FileRemoved = FILE_ACTION_REMOVED,
      FileModified = FILE_ACTION_MODIFIED,
      FileRenamedOld = FILE_ACTION_RENAMED_OLD_NAME,
      FileRenamedNew = FILE_ACTION_RENAMED_NEW_NAME
    };

    struct ActionInfo
    {
      Action m_action;

      std::string m_fileName;
      std::string m_path;
    };

    struct WatchInfo
    {
      OVERLAPPED m_overlapped;
      std::string m_dir;
      HANDLE m_dirHandle;
      bool m_isRecursive;

      bool m_stop;

      BYTE *m_buffer;
      DWORD m_bufferSize;

      FileWatcher *m_fileWatcher;
    };

    typedef void(*FileWatcherCallback)(const ActionInfo &);

    FileWatcher(FileWatcherCallback func);
    ~FileWatcher(void);

    bool AddDirectoryToWatch(const std::string &dir, bool recursive = true);
    void RemoveDirectoryFromWatch(const std::string &dir);
    void Update(void);

  private:
    friend VOID CALLBACK FileIOCompletionRoutine(
      DWORD        dwErrorCode,
      DWORD        dwNumberOfBytesTransfered,
      LPOVERLAPPED lpOverlapped
      );

    void _StopWatch(WatchInfo *info) const;
    std::string _GetFileName(const std::string &fileName) const;
    std::string _GetPath(const std::string &fileName) const;

    std::unordered_map<std::string, WatchInfo*> m_dirsMap;
    FileWatcherCallback m_callback;
  };
}