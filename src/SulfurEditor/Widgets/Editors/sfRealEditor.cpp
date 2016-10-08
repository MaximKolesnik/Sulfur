/******************************************************************************/
/*!
\par     Sulfur
\file    sfRealEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Real property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfRealEditor.hpp"

namespace Sulfur
{

RealEditor::RealEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  if (prop->HasRange())
  {
    Real max;
    prop->GetRange(m_min, max);
    m_range = max - m_min;

    CreateSliderLayout();
  }
  else
  {
    CreateEditLayout();
  }

  UpdateValue();
}

RealEditor::RealEditor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateEditLayout();
  UpdateValue();
}

RealEditor::~RealEditor()
{
}

void RealEditor::UpdateValue()
{
  Real realValue = GetValue<Real>();
  Real t = (realValue - m_min) / m_range;
  m_slider->setValue((int)(t * c_sliderRange));

  m_edit->setText(QString::number(realValue));
}

void RealEditor::CreateSliderLayout()
{
  m_edit = CreateEdit();
  m_edit->setMinimumWidth(100);
  m_edit->setMaximumWidth(100);
  m_layout->addWidget(m_edit);

  m_slider = new QSlider(Qt::Orientation::Horizontal);
  m_slider->setRange(0, c_sliderRange);
  m_layout->addWidget(m_slider);

  QObject::connect(
    m_slider, &QSlider::valueChanged,
    this, &RealEditor::OnSliderChanged
    );
}

void RealEditor::CreateEditLayout()
{
  m_edit = CreateEdit();
  m_layout->addWidget(m_edit);
}

QLineEdit* RealEditor::CreateEdit()
{
  QLineEdit *edit = new QLineEdit();
  edit->setValidator(new QDoubleValidator());

  QObject::connect(
    edit, &QLineEdit::textEdited,
    this, &RealEditor::OnValueChanged
    );

  return edit;
}

void RealEditor::OnSliderChanged(int value)
{
  Real t = (Real)value / c_sliderRange;
  SetValue(t * m_range + m_min);
  UpdateValue();
}

void RealEditor::OnValueChanged(const QString& value)
{
  Real realValue = (Real)value.toDouble();
  Real t = (realValue - m_min) / m_range;

  m_slider->setValue((int)(t * c_sliderRange));
  SetValue((Real)value.toDouble());
}

}
