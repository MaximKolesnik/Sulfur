/******************************************************************************/
/*!
\par     Sulfur
\file    sfGameWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Game rendering widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfGameWidget.hpp"
#include "Core/sfCore.hpp"

namespace Sulfur
{

GameWidget::GameWidget(QWidget *parent)
  : QWidget(parent)
{
  setAttribute(Qt::WA_PaintOnScreen, true);
  setAttribute(Qt::WA_NativeWindow, true);

  Core::Instance()->StartUp((HWND)winId());
}

GameWidget::~GameWidget()
{
  Core::Instance()->ShutDown();
}

void GameWidget::Frame()
{
  Core::Instance()->Frame();
}

}
