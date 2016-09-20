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
	
  class PropertyEditor : public QFrame
  {
    Q_OBJECT

  public:
    PropertyEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    PropertyEditor(void *ptr, QWidget *parent = 0);
    ~PropertyEditor();

    virtual void UpdateValue() {}

    static PropertyEditor* Create(ReflectionBase *object, Property *prop);
    static PropertyEditor* Create(void *ptr, const TypeInfo *typeInfo);

  protected:
    template <typename T>
    const T& GetValue()
    {
      if (m_ptr != nullptr)
        return *reinterpret_cast<T*>(m_ptr);

      return m_property->GetValue<T>(*m_object);
    }

    template <typename T>
    void SetValue(const T& value)
    {
      if (m_ptr != nullptr)
        *reinterpret_cast<T*>(m_ptr) = value;
      else
        m_property->SetValue(*m_object, value);

      emit ValueChanged();
    }

  signals:
    void ValueChanged();

  protected:
    ReflectionBase *m_object;
    Property *m_property;

    void *m_ptr;
  
  };
  
}