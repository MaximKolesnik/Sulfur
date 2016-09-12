/******************************************************************************/
/*!
\par     Sulfur
\file    sfOutputWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Debug output widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class OutputWidget : public QTextEdit
  {
    Q_OBJECT

  public:
    OutputWidget(QWidget *parent = 0);
    ~OutputWidget();

  };
  
}