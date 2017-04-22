/******************************************************************************/
/*!
\par     Sulfur
\file    sfOutputWidget.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Debug output widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfOutputWidget.hpp"

namespace Sulfur
{

OutputWidget::OutputWidget(QWidget *parent)
  : QTextEdit(parent)
{
  setReadOnly(true);
  m_oldStreamBuffer = std::cout.rdbuf(this);

  setSizePolicy(QSizePolicy::Policy::Ignored, QSizePolicy::Ignored);
  setMinimumSize(250, 175);
}

OutputWidget::~OutputWidget()
{
  std::cout.rdbuf(m_oldStreamBuffer);
}

std::streamsize OutputWidget::xsputn(const char* _Ptr, std::streamsize _Count)
{
  Write(_Ptr, _Count);
  return _Count;
}

typename OutputWidget::TraitsType::int_type OutputWidget::overflow(typename TraitsType::int_type v)
{
  char ch = std::char_traits<char>::to_char_type(v);
  Write(&ch, 1);
  return std::char_traits<char>::not_eof(v);
}

void OutputWidget::Write(const char *buffer, std::streamsize size)
{
  append(QString(QByteArray(buffer, (int)size)));
}

}
