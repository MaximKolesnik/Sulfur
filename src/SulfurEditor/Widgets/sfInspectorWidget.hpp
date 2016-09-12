/******************************************************************************/
/*!
\par     Sulfur
\file    sfInspectorWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Object inspector widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Reflection/sfReflection.hpp"

namespace Sulfur
{
	
  class InspectorWidget : public QWidget
  {
    Q_OBJECT

  public:
    InspectorWidget(QWidget *parent = 0);
    ~InspectorWidget();

    void SetObject(ReflectionBase *object);
    void UpdateValues();
  
  private:
    ReflectionBase *m_object;
    QBoxLayout *m_layout;

  };
  
}