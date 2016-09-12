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
  : QWidget(parent)
{
  m_splitter = new QSplitter(this);
}

ResourceBrowserWidget::~ResourceBrowserWidget()
{
}

}
