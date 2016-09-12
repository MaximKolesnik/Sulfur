/******************************************************************************/
/*!
\par     Sulfur
\file    sfWidget.hpp
\author
\par     DP email:
\date    9/5/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class Widget : public QWidget
  {
    Q_OBJECT

  public:
    Widget(QWidget *parent = 0);
    ~Widget();
  
  };
  
}