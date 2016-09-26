/******************************************************************************/
/*!
\par     Sulfur
\file    sfFileWatcher.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    9/26/2016

\brief   File wathcer

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfFileWatcher.hpp"

namespace Sulfur
{

    BOOL _RefreshWatch( FileWatcher::WatchInfo *info, 
      VOID (CALLBACK *func)(DWORD, DWORD, LPOVERLAPPED) )
    {
      return ReadDirectoryChangesW(info->m_dirHandle, info->m_buffer,
        info->m_bufferSize, info->m_isRecursive,
        FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION,
        NULL, &info->m_overlapped, func);
    }

    VOID CALLBACK FileIOCompletionRoutine(
      DWORD        dwErrorCode,
      DWORD        dwNumberOfBytesTransfered,
      LPOVERLAPPED lpOverlapped
      )
    {
      if (dwNumberOfBytesTransfered == 0)
        return;

      char file[MAX_PATH];
      FILE_NOTIFY_INFORMATION *notifyStruct;
      FileWatcher::WatchInfo *watchInfo = 
        reinterpret_cast<FileWatcher::WatchInfo*>(lpOverlapped);
      size_t offset = 0;

      if (dwErrorCode == ERROR_SUCCESS)
      {
        do
        {
          notifyStruct = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&watchInfo[offset]);
          int numChars = WideCharToMultiByte(CP_ACP, 0, notifyStruct->FileName,
            notifyStruct->FileNameLength / sizeof(WCHAR), file, MAX_PATH - 1, NULL, NULL);

          file[numChars] = '\0';

          //process changes here

          offset += notifyStruct->NextEntryOffset;
        } while (notifyStruct->NextEntryOffset != 0);
      }

      if (!watchInfo->m_stop)
        _RefreshWatch(watchInfo, FileIOCompletionRoutine);
    }

  FileWatcher::FileWatcher(void)
  {

  }

  FileWatcher::~FileWatcher(void)
  {
    for (auto &it : m_dirsMap)
    {
      _StopWatch(it.second);
      delete it.second;
    }
  }

  bool FileWatcher::AddDirectoryToWatch(const std::string &dir, bool recursive)
  {
    if (m_dirsMap.find(dir) != m_dirsMap.end())
    {
      SF_WARNING("Directory is already being wathced");
      return false;
    }

    WatchInfo *newInfo = new WatchInfo();

    newInfo->m_dirHandle = CreateFileA(dir.c_str(), FILE_LIST_DIRECTORY,
      FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
      FILE_FLAG_OVERLAPPED | FILE_FLAG_BACKUP_SEMANTICS, NULL);

    if (newInfo->m_dirHandle == INVALID_HANDLE_VALUE)
    {
      SF_WARNING("Directory was not added to watch");
      delete newInfo;
      return false;
    }

    newInfo->m_overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    newInfo->m_isRecursive = recursive;
    newInfo->m_stop = false;
    newInfo->m_bufferSize = c_numFiles * sizeof(FILE_NOTIFY_INFORMATION);
    newInfo->m_buffer = new BYTE[newInfo->m_bufferSize];

    BOOL result = ReadDirectoryChangesW(newInfo->m_dirHandle, newInfo->m_buffer, 
      newInfo->m_bufferSize, recursive,
      FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION,
      NULL, &newInfo->m_overlapped, &FileIOCompletionRoutine);

    if (result == 0)
    {
      SF_WARNING("Cannot start watcher");
      delete newInfo->m_buffer;
      delete newInfo;
      return false;
    }

    m_dirsMap.insert(std::make_pair(dir, newInfo));

    return true;
  }

  void FileWatcher::RemoveDirectoryFromWatch(const std::string &dir)
  {
    auto &it = m_dirsMap.find(dir);
    if (it == m_dirsMap.end())
      return;

    _StopWatch(it->second);

    delete it->second->m_buffer;
    delete it->second;

    m_dirsMap.erase(it);
  }

  void FileWatcher::Update(void)
  { 
    MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
  }

  void FileWatcher::_StopWatch(WatchInfo *info) const
  {
    info->m_stop = true;

    CancelIo(info->m_dirHandle);

    _RefreshWatch(info, NULL);

    if (!HasOverlappedIoCompleted(&info->m_overlapped))
      SleepEx(INFINITE, 1);

    CloseHandle(info->m_overlapped.hEvent);
    CloseHandle(info->m_dirHandle);
  }
}