/******************************************************************************/
/*!
\par     Sulfur
\file    sfResourceBox.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/30/2016

\brief   Resource box for resource editor.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfResourceBox.hpp"
#include "Utils/sfFileUtils.hpp"

namespace Sulfur
{

ResourceBox::ResourceBox(QWidget *parent)
  : QLineEdit(parent), m_dragging(false), m_extensionChecker(nullptr)
{
  setDragEnabled(true);
  setReadOnly(true);
}

ResourceBox::~ResourceBox()
{
}


bool ResourceBox::isDragging() const
{
  return m_dragging;
}

void ResourceBox::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasText())
  {
    if (m_extensionChecker)
    {
      std::string ext = FileUtils::GetExtension(event->mimeData()->text().toUtf8().data());
      if (!(*m_extensionChecker)(ext))
        return;
    }
  }

  QWidget::dragEnterEvent(event);
  SetHighlight(true);
  event->accept();
}

void ResourceBox::dragMoveEvent(QDragMoveEvent *event)
{
  QWidget::dragMoveEvent(event);
}

void ResourceBox::dragLeaveEvent(QDragLeaveEvent *event)
{
  QWidget::dragLeaveEvent(event);
  SetHighlight(false);
  event->accept();
}

void ResourceBox::dropEvent(QDropEvent *event)
{
  QWidget::dropEvent(event);
  SetHighlight(false);
  event->accept();

  if (event->mimeData()->hasText())
  {
    setText(event->mimeData()->text());
  }
}

void ResourceBox::SetHighlight(bool highlight)
{
  m_dragging = highlight;
  style()->polish(this);
}

}
