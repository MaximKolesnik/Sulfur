/******************************************************************************/
/*!
\par     Sulfur
\file    sfReflectedObjectEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Editor for reflected objects

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfCollapsableEditor.hpp"

namespace Sulfur
{
	
  class ReflectedObjectEditor : public CollapsableEditor
  {
    Q_OBJECT

  public:
    ReflectedObjectEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    ReflectedObjectEditor(void *ptr, QWidget *parent = 0);
    ~ReflectedObjectEditor();

    virtual void UpdateValue() override;

  };
  
}