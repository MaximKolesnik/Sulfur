/******************************************************************************/
/*!
\par     Sulfur
\file    sfAboutWindow.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/7/2016

\brief   About window, displays version and copyright info

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfAboutWindow.hpp"

namespace Sulfur
{

AboutWindow::AboutWindow(QWidget *parent)
  : QDialog(parent)
{
  setModal(false);
  setWindowTitle("About");
  setWindowFlags(Qt::WindowStaysOnTopHint);

  m_layout = new QGridLayout();
  setLayout(m_layout);
  setFixedSize(QSize(450, 300));

  QLabel *digipenLabel = new QLabel();
  digipenLabel->setPixmap(QPixmap("images/Digipen.png").scaledToWidth(375));
  digipenLabel->setFixedSize(digipenLabel->pixmap()->size());
  m_layout->addWidget(digipenLabel, 0, 0, Qt::AlignHCenter);

  QLabel *infoLabel = new QLabel("Sulfur Engine\nVersion: 0.1\nTeam Midnight Rage\n"
    "Dylan Norris and Maxim Kolesnik\nInstructor: Jen Sward\nPresident: Claude Comair\n"
    "All content (C) 2016 DigiPen (USA) Corporation, all rights reserved.");
  m_layout->addWidget(infoLabel, 1, 0);
}

AboutWindow::~AboutWindow()
{

}

}
