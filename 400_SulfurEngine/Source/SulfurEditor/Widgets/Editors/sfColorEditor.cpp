/******************************************************************************/
/*!
\par     Sulfur
\file    sfColorEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/7/2016

\brief   Color property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfColorEditor.hpp"
#include "Modules/Graphics/Types/sfColor.hpp"

namespace Sulfur
{

ColorEditor::ColorEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  CreateSelector();
  UpdateValue();
}

ColorEditor::ColorEditor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateSelector();
  UpdateValue();
}

ColorEditor::~ColorEditor()
{
}

void ColorEditor::UpdateValue()
{
  const Color& sulfurColor = GetValue<Color>();
  QColor color((int)(sulfurColor[0] * 255), (int)(sulfurColor[1] * 255), (int)(sulfurColor[2] * 255), (int)(sulfurColor[3] * 255));
  m_colorSelector->SetColor(color);
}

void ColorEditor::CreateSelector()
{
  m_colorSelector = new ColorSelectorBox();
  m_layout->addWidget(m_colorSelector);

  QObject::connect(
    m_colorSelector, &ColorSelectorBox::ColorChanged,
    this, &ColorEditor::OnColorChanged
    );
}

void ColorEditor::OnColorChanged(const QColor& color)
{
  Color sulfurColor = Color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  SetValue(sulfurColor);
  //UpdateValue();
  emit ValueChanged();
}

}
