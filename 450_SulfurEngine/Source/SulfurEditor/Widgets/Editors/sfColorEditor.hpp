/******************************************************************************/
/*!
\par     Sulfur
\file    sfColorEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/7/2016

\brief   Color property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"
#include "../sfColorSelectorBox.hpp"

namespace Sulfur
{
	
  class ColorEditor : public LabeledEditor
  {
    Q_OBJECT

  public:
    ColorEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    ColorEditor(void *ptr, QWidget *parent = 0);
    ~ColorEditor();

    virtual void UpdateValue() override;

  private:
    void CreateSelector();

  public slots:
    void OnColorChanged(const QColor& color);

  private:
    ColorSelectorBox *m_colorSelector;
  
  };
  
}