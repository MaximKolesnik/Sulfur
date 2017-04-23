/******************************************************************************/
/*!
\par     Sulfur
\file    sfAboutWindow.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/7/2016

\brief   About window, displays version and copyright info

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Widgets/sfInspectorWidget.hpp"

namespace Sulfur
{
	
  class AboutWindow : public QDialog
  {
    Q_OBJECT

  public:
    AboutWindow(QWidget *parent = 0);
    ~AboutWindow();

  private:
    QGridLayout *m_layout;
  
  };
  
}