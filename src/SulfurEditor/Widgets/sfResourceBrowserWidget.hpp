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
	
  class ResourceBrowserWidget : public QWidget
  {
    Q_OBJECT

  public:
    ResourceBrowserWidget(QWidget *parent = 0);
    ~ResourceBrowserWidget();
  
  private:
    QSplitter *m_splitter;

  };
  
}