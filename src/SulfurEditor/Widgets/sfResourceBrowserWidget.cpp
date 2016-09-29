/******************************************************************************/
/*!
\par     Sulfur
\file    sfResourceBrowserWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Resource browser widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfResourceBrowserWidget.hpp"

namespace Sulfur
{

ResourceBrowserWidget::ResourceBrowserWidget(QWidget *parent)
  : QSplitter(parent)
{
  setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Ignored);
  setMinimumSize(350, 175);

  m_folderTree = new QTreeView();
  addWidget(m_folderTree);

  m_folderTreeModel = new QFileSystemModel();
  m_folderTreeModel->setFilter(QFlags<QDir::Filter>{QDir::Dirs, QDir::NoDotAndDotDot});
  m_folderTree->setModel(m_folderTreeModel);

  m_fileList = new QListView();
  m_fileList->setViewMode(QListView::ViewMode::IconMode);
  addWidget(m_fileList);

  m_fileListModel = new QFileSystemModel();
  m_fileListModel->setFilter(QFlags<QDir::Filter>{QDir::Files});

  QObject::connect(
    m_folderTree->selectionModel(), &QItemSelectionModel::currentChanged,
    this, &ResourceBrowserWidget::OnFolderSelected
    );

}

ResourceBrowserWidget::~ResourceBrowserWidget()
{
}

void ResourceBrowserWidget::SetProjectRoot(const QString& root)
{
  m_projectRoot = root;
  QString resourcesPath = root + "/Resources";
  m_folderTreeModel->setRootPath(resourcesPath);
  m_folderTree->setRootIndex(m_folderTreeModel->index(resourcesPath));
}

void ResourceBrowserWidget::OnFolderSelected(const QModelIndex &current, const QModelIndex &previous)
{
  QString path = m_folderTreeModel->filePath(current);
  m_fileListModel->setRootPath(path);
  m_fileList->setModel(m_fileListModel);
  m_fileList->setRootIndex(m_fileListModel->index(path));
}

}
