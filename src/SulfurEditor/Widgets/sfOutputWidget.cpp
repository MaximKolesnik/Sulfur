/******************************************************************************/
/*!
\par     Sulfur
\file    sfOutputWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Debug output widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfOutputWidget.hpp"

namespace Sulfur
{

OutputWidget::OutputWidget(QWidget *parent)
  : QTextEdit(parent)
{
  setReadOnly(true);
}

OutputWidget::~OutputWidget()
{
}

}
