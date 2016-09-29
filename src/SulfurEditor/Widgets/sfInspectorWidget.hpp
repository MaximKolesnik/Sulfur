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
#include "Editors/sfPropertyEditor.hpp"

namespace Sulfur
{
	
  class InspectorWidget : public QWidget
  {
    Q_OBJECT

  public:
    InspectorWidget(QWidget *parent = 0);
    ~InspectorWidget();

    template <typename T>
    void SetObject(T *object)
    {
      m_object = object;

      QWidget().setLayout(m_layout);
      CreateLayout();

      if (object != nullptr)
      {
        PropertyEditor *editor = PropertyEditor::Create(object, SF_TYPE_INFO(T));

        QObject::connect(
          editor, &PropertyEditor::ValueChanged,
          this, &InspectorWidget::OnPropertyChanged
          );

        m_layout->addWidget(editor);
        m_layout->insertStretch(-1, 1);
      }
    }

    void UpdateValues();

  private:
    void CreateLayout();

  public slots:
    void OnPropertyChanged();

  signals:
    void ObjectChanged();
  
  private:
    ReflectionBase *m_object;
    QBoxLayout *m_layout;

  };
  
}