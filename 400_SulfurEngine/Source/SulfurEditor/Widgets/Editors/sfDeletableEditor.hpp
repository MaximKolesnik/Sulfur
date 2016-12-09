/******************************************************************************/
/*!
\par     Sulfur
\file    sfDeletableEditor.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/7/2016

\brief   Deleteable container property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "sfReflectedObjectEditor.hpp"

namespace Sulfur
{
	
  class DeletableEditor : public ReflectedObjectEditor
  {
    Q_OBJECT

  public:
    DeletableEditor(ReflectionBase *object, Property *prop, QWidget *parent = 0);
    DeletableEditor(void *ptr, QWidget *parent = 0);
    ~DeletableEditor();

  private:
    void Setup();

  signals:
    void Deleted();

  public slots:
    void OnDeleteClicked(bool checked);

  protected:
    QPushButton *m_deleteButton;
  
  };
  
}