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
#include "sfCollapsableEditor.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Modules/Script/sfScriptManager.hpp"
#include "Components/sfScriptComponent.hpp"

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

  auto& components = object->GetComponents();
  for (auto& componentPair : components)
  {
    IEntity *component = ComponentFactory::Instance()->GetComponent(componentPair.first, componentPair.second);

    CollapsableEditor *editor = static_cast<CollapsableEditor*>(PropertyEditor::Create(component, SF_TYPE_INFO(IEntity)));
    editor->SetHeaderText(component->m_name);
    AddChild(editor);
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
    if (!object->HasComponent(componentType))
    {
      QAction *action = menu->addAction(componentType.c_str());

      QObject::connect(
        action, &QAction::triggered,
        [=]() { this->OnAddComponent(action); }
      );
    }
  }

  QMenu *scriptMenu = new QMenu("Script");
  menu->addMenu(scriptMenu);

  auto& scriptNames = ScriptManager::Instance()->GetScriptNames();
  for (const std::string& scriptName : scriptNames)
  {
    if (!object->HasComponent(scriptName))
    {
      QAction *action = scriptMenu->addAction(scriptName.c_str());

      QObject::connect(
        action, &QAction::triggered,
        [=]() { this->OnAddScript(action); }
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
  AddChild(PropertyEditor::Create(component, SF_TYPE_INFO(IEntity)));
  m_childrenLayout->removeWidget(m_newComponentButton);
  m_childrenLayout->addWidget(m_newComponentButton);

  object->AttachComponent(component);
}

void ObjectEditor::OnAddScript(QAction *action)
{
  Object *object = const_cast<Object*>(&GetValue<Object>());

  ScriptComponent *script = ScriptManager::Instance()->CreateScript(action->text().toUtf8().data());
  CollapsableEditor *editor = static_cast<CollapsableEditor*>(PropertyEditor::Create(script, SF_TYPE_INFO(IEntity)));
  editor->SetHeaderText(script->m_name);
  AddChild(editor);

  m_childrenLayout->removeWidget(m_newComponentButton);
  m_childrenLayout->addWidget(m_newComponentButton);

  object->AttachComponent(script);
}

}
