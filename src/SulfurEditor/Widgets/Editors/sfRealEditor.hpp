/******************************************************************************/
/*!
\par     Sulfur
\file    sfRealEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/12/2016

\brief   Real property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfLabeledEditor.hpp"

namespace Sulfur
{
	
  class RealEditor : public LabeledEditor
  {
    Q_OBJECT

  public:
    RealEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    RealEditor(void *ptr, QWidget *parent = 0);
    ~RealEditor();

    virtual void UpdateValue() override;

  private:
    void CreateSliderLayout();
    void CreateEditLayout();
    QLineEdit* CreateEdit();

  public slots:
    void OnSliderChanged(int value);
    void OnValueChanged(const QString& value);

  private:
    QLineEdit *m_edit;

    QSlider *m_slider;
    Real m_min;
    Real m_range;
    static const int c_sliderRange = 1000;
  
  };
  
}