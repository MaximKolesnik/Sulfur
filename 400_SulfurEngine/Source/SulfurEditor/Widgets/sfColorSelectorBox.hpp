/******************************************************************************/
/*!
\par     Sulfur
\file    sfColorSelectorBox.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/7/2016

\brief   Color selector widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class ColorSelectorBox : public QFrame
  {

    Q_OBJECT

  public:
    ColorSelectorBox(QWidget *parent = 0);
    ~ColorSelectorBox();

    void SetColor(const QColor& color);
    QColor GetColor() const;

    virtual void mouseReleaseEvent(QMouseEvent *event) override;

  protected:
    virtual void	paintEvent(QPaintEvent * event) override;

  public slots:
    void OnCurrentColorChanged(const QColor &color);
    void OnRejected();
    
  signals:
    void ColorChanged(const QColor& color);

  private:
    QColor m_previousColor;
    QColorDialog *m_colorDialog;
  
  };
  
}