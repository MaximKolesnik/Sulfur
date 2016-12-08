/******************************************************************************/
/*!
\par     Sulfur
\file    sfObjectEditor.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/18/2016

\brief   Editor for objects

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfObjectEditor.hpp"
#include "sfDeletableEditor.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Factories/sfComponentFactory.hpp"

namespace Sulfur
{

ObjectEditor::ObjectEditor(ReflectionBase *object, Property *prop, QWidget *parent)
  : ContainerEditor(object, prop, parent)
{
  UpdateValue();
}

ObjectEditor::ObjectEditor(void *ptr, QWidget *parent)
  : ContainerEditor(ptr, parent)
{
  UpdateValue();
}

ObjectEditor::~ObjectEditor()
{
}

void ObjectEditor::UpdateValue()
{
  //ClearLayout();
  Object *object = const_cast<Object*>(&GetValue<Object>());
  AddChild(PropertyEditor::Create(object, object->GetProperty("Name")));

  CollapsableEditor *transformEditor = static_cast<CollapsableEditor*>(PropertyEditor::Create(object->GetComponent("Transform"), SF_TYPE_INFO(IEntity)));
  transformEditor->SetHeaderText("Transform");
  AddChild(transformEditor);

  auto& components = object->GetComponents();
  for (auto& componentPair : components)
  {
    IEntity *component = ComponentFactory::Instance()->GetComponent(componentPair.first, componentPair.second);

    if (component->m_name != "Transform")
    {
      DeletableEditor *editor = new DeletableEditor(component);
      editor->SetHeaderText(component->m_name);
      AddChild(editor);

      connect(editor, &DeletableEditor::Deleted,
        [=]() { ComponentFactory::Instance()->DeleteComponent(component); });
    }
  }

  m_newComponentButton = new QToolButton();
  m_newComponentButton->setText("Add Component");
  m_newComponentButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
  m_childrenLayout->addWidget(m_newComponentButton);

  QObject::connect(
    m_newComponentButton, &QToolButton::clicked,
    this, &ObjectEditor::OnAddComponentClicked
    );

  
}

void ObjectEditor::OnAddComponentClicked()
{
  Object *object = const_cast<Object*>(&GetValue<Object>());

  QMenu *menu = new QMenu();
  menu->setFixedWidth(m_newComponentButton->width());

  auto& componentTypes = ComponentFactory::Instance()->GetComponentTypes();
  for (const std::string& componentType : componentTypes)
  {
    //Exclusive components like colliders
    if (ComponentFactory::Instance()->IsGrouped(componentType))
    {
      std::string groupName = ComponentFactory::Instance()->GetGroupName(componentType);

      if (object->HasComponentOfGroup(groupName))
        continue;
    }

    if (!object->HasComponent(componentType))
    {
      QAction *action = menu->addAction(componentType.c_str());

      QObject::connect(
        action, &QAction::triggered,
        [=]() { this->OnAddComponent(action); }
      );
    }
  }

  m_newComponentButton->setMenu(menu);
  m_newComponentButton->showMenu();
}

void ObjectEditor::OnAddComponent(QAction *action)
{
  Object *object = const_cast<Object*>(&GetValue<Object>());

  IEntity *component = ComponentFactory::Instance()->CreateComponent(action->text().toUtf8().data());
  DeletableEditor *editor = new DeletableEditor(component);
  AddChild(editor);
  m_childrenLayout->removeWidget(m_newComponentButton);
  m_childrenLayout->addWidget(m_newComponentButton);

  connect(editor, &DeletableEditor::Deleted,
    [=]() { ComponentFactory::Instance()->DeleteComponent(component); });

  object->AttachComponent(component);
}

}
