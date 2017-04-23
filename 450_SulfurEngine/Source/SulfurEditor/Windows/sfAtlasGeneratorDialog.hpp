/******************************************************************************/
/*!
\par     Sulfur
\file    sfAtlasGeneratorDialog.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    02/25/2017

\brief   Atlas generator dialog window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Modules/Scene/sfScene.hpp"
#include "Widgets/sfInspectorWidget.hpp"

namespace Sulfur
{
	
  class AtlasGeneratorDialog : public QDialog
  {
    Q_OBJECT

  public:
    AtlasGeneratorDialog(QWidget *parent = 0);
    ~AtlasGeneratorDialog();

  private:
    QBoxLayout *m_layout;
  
  };
  
}