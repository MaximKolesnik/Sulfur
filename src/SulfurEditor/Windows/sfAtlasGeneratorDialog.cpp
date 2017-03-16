/******************************************************************************/
/*!
\par     Sulfur
\file    sfAtlasGeneratorDialog.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    02/25/2017

\brief   Atlas generator dialog window

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfAtlasGeneratorDialog.hpp"

namespace Sulfur
{

AtlasGeneratorDialog::AtlasGeneratorDialog(QWidget *parent)
  : QDialog(parent)
{
  setModal(false);
  setWindowTitle("Atlas Generator");
  setWindowFlags(Qt::WindowStaysOnTopHint);

  m_layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
  setLayout(m_layout);
}

AtlasGeneratorDialog::~AtlasGeneratorDialog()
{

}

}
