/******************************************************************************/
/*!
\par     Sulfur
\file    sfOutputWidget.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/11/2016

\brief   Debug output widget

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class OutputWidget : public QTextEdit, public std::basic_streambuf<char, std::char_traits<char>>
  {
    Q_OBJECT

  private:
    typedef std::char_traits<char> TraitsType;

  public:
    OutputWidget(QWidget *parent = 0);
    ~OutputWidget();

    std::streamsize xsputn(const char* _Ptr, std::streamsize _Count);
    typename TraitsType::int_type overflow(typename TraitsType::int_type v);

  private:
    void Write(const char *buffer, std::streamsize size);

  private:
    std::streambuf *m_oldStreamBuffer;

  };
  
}