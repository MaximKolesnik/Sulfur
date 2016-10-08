/******************************************************************************/
/*!
\par     Sulfur
\file    sfResourceEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/28/2016

\brief   Resource property editor

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfResourceEditor.hpp"
#include "Modules/Resource/sfResourcePath.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"

namespace Sulfur
{

ResourceEditor::ResourceEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : LabeledEditor(object, prop, parent)
{
  CreateEdit();
  UpdateValue();
}

ResourceEditor::ResourceEditor(void *ptr, QWidget *parent)
  : LabeledEditor(ptr, parent)
{
  CreateEdit();
  UpdateValue();
}

ResourceEditor::~ResourceEditor()
{
}

void ResourceEditor::UpdateValue()
{
  m_edit->setText(GetValue<std::string>().c_str());
}

void ResourceEditor::CreateEdit()
{
  m_edit = new ResourceBox();
  //m_edit->SetResourceType<Mesh>();
  m_layout->addWidget(m_edit);

  QObject::connect(
    m_edit, &ResourceBox::textChanged,
    this, &ResourceEditor::OnValueChanged
    );
}

void ResourceEditor::OnValueChanged(const QString& value)
{
  QDir resourceDir = QDir(QDir::currentPath() + "/Resources");

  QString relativePath = value;
  if (relativePath.startsWith("file:///"))
    relativePath = relativePath.right(relativePath.size() - 8);

  relativePath = resourceDir.relativeFilePath(relativePath);

  SetValue(ResourcePath(relativePath.toUtf8().data()));
  UpdateValue();
  emit ValueChanged();
}

}
