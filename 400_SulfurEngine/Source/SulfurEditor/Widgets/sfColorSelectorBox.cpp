/******************************************************************************/
/*!
\par     Sulfur
\file    sfColorSelectorBox.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/7/2016

\brief   Color selector widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfColorSelectorBox.hpp"

namespace Sulfur
{

  ColorSelectorBox::ColorSelectorBox(QWidget *parent)
  : QFrame(parent)
  {
    setFrameShape(Shape::Panel);
    setFrameShadow(Shadow::Sunken);
    setAutoFillBackground(true);

    m_colorDialog = new QColorDialog(this);
    m_colorDialog->setOption(QColorDialog::ShowAlphaChannel, true);

    QObject::connect(
      m_colorDialog, &QColorDialog::currentColorChanged,
      this, &ColorSelectorBox::OnCurrentColorChanged
      );

    QObject::connect(
      m_colorDialog, &QColorDialog::rejected,
      this, &ColorSelectorBox::OnRejected
      );
  }

  ColorSelectorBox::~ColorSelectorBox()
  {
  }

  void ColorSelectorBox::SetColor(const QColor& color)
  {
    m_colorDialog->setCurrentColor(color);
    repaint();
  }

  QColor ColorSelectorBox::GetColor() const
  {
    return m_colorDialog->currentColor();
  }

  void ColorSelectorBox::mouseReleaseEvent(QMouseEvent *event)
  {
    if (event->button() == Qt::MouseButton::LeftButton)
    {
      m_previousColor = GetColor();
      m_colorDialog->show();
    }

    QWidget::mouseReleaseEvent(event);
  }

  void ColorSelectorBox::OnCurrentColorChanged(const QColor &color)
  {
    repaint();

    emit ColorChanged(color);
  }

  void ColorSelectorBox::OnRejected()
  {
    SetColor(m_previousColor);
  }

  void ColorSelectorBox::paintEvent(QPaintEvent * event)
  {
    QPainter painter(this);
    QBrush background(m_colorDialog->currentColor());
    painter.fillRect(event->rect(), background);
    QFrame::paintEvent(event);
  }

}
