/******************************************************************************/
/*!
\par     Sulfur
\file    sfResourceBrowserWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Resource browser widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class ResourceBrowserWidget : public QSplitter
  {
    Q_OBJECT

  public:
    ResourceBrowserWidget(QWidget *parent = 0);
    ~ResourceBrowserWidget();
  
    void SetProjectRoot(const QString& root);

  public slots:
    void OnFolderSelected(const QModelIndex &current, const QModelIndex &previous);

  private:
    QTreeView *m_folderTree;
    QFileSystemModel *m_folderTreeModel;

    QListView *m_fileList;
    QFileSystemModel *m_fileListModel;

  };
  
}