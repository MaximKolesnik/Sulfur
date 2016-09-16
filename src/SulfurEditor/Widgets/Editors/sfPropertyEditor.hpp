/******************************************************************************/
/*!
\par     Sulfur
\file    sfPropertyEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Base property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Reflection/sfReflection.hpp"

namespace Sulfur
{
	
  class PropertyEditor : public QWidget
  {
    Q_OBJECT

  public:
    PropertyEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    ~PropertyEditor();

    virtual void UpdateValue() {}

  protected:
    template <typename T>
    const T& GetValue()
    {
      return m_property->GetValue<T>(*m_object);
    }

    template <typename T>
    void SetValue(const T& value)
    {
      m_property->SetValue(*m_object, value);
    }

  protected:
    QBoxLayout *m_layout;

    ReflectionBase *m_object;
    Property *m_property;
  
  };
  
}