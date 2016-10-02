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
#pragma once
#include "Modules/Resource/sfResourceManager.hpp"

namespace Sulfur
{
	
  class ResourceBox : public QLineEdit
  {
    Q_OBJECT

    Q_PROPERTY(bool dragging READ isDragging)

  public:
    typedef bool(*ExtensionCheckFunc)(const std::string&);

  public:
    ResourceBox(QWidget *parent = 0);
    ~ResourceBox();

    template <typename ResourceType>
    void SetResourceType()
    {
      m_extensionChecker = &ResourceManager<ResourceType>::IsExtensionRegistered;
    }

    bool isDragging() const;

  protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

  private:
    void SetHighlight(bool highlight);

  private:
    bool m_dragging;
    ExtensionCheckFunc m_extensionChecker;

  };
  
}