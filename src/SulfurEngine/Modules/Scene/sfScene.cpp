/******************************************************************************/
/*!
\par     Sulfur
\file    sfScene.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/16/2016

\brief   Scene (tree of objects)

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfScene.hpp"
#include "Factories/sfObjectFactory.hpp"

namespace Sulfur
{

  SceneProperties::SceneProperties()
    : m_skyboxResource(nullptr), m_skyboxResourcePath()
  {

  }

  template <>
  UINT32 Serialization::SerializedSize<Scene>(const Scene& value)
  {
    ComponentFactory::ComponentMap& components = ComponentFactory::Instance()->GetComponentMap();
    UINT32 size = Serialization::SerializedSize(value.GetRootObjects()) +
      Serialization::SerializedSize(value.GetCameraObject()) +
      Serialization::SerializedSize(value.m_sceneProperties) +
     Serialization::SerializedSize(ObjectFactory::Instance()->GetObjects()) +
      Serialization::SerializedSize((UINT32)components.size());

    for (auto it = components.begin(); it != components.end(); ++it)
    {
      size += Serialization::SerializedSize(it->first);
      size += Serialization::SerializedSize(*it->second);
    }

    return size;
  }

  template <>
  void Serialization::Serialize<Scene>(std::ostream& str, const Scene& value)
  {
    value.GetProperty("Root Objects")->Serialize(str, &value);
    value.GetProperty("Camera Object")->Serialize(str, &value);
    Serialization::Serialize(str, value.m_sceneProperties);
    Serialization::Serialize(str, ObjectFactory::Instance()->GetObjects());

    ComponentFactory::ComponentMap& components = ComponentFactory::Instance()->GetComponentMap();
    Serialization::Serialize(str, (UINT32)components.size());
    for (auto it = components.begin(); it != components.end(); ++it)
    {
      Serialization::Serialize(str, it->first);
      Serialization::Serialize(str, *it->second);
    }
  }

  template <>
  void Serialization::Deserialize<Scene>(std::istream& str, Scene& value)
  {
    ObjectFactory::Instance()->DestroyAll();
    value.GetProperty("Root Objects")->Deserialize(str, &value);
    value.GetProperty("Camera Object")->Deserialize(str, &value);
    Serialization::Deserialize(str, value.m_sceneProperties);
    Serialization::Deserialize(str, ObjectFactory::Instance()->GetObjects());

    ComponentFactory::ComponentMap& components = ComponentFactory::Instance()->GetComponentMap();
    UINT32 componentTypeCount;
    Serialization::Deserialize(str, componentTypeCount);
    for (UINT32 i = 0; i < componentTypeCount; ++i)
    {
      std::string componentName;
      Serialization::Deserialize(str, componentName);
      Serialization::Deserialize(str, *components[componentName]);
    }
  }

  Scene::Scene()
    : m_cameraObject(SF_INV_HANDLE)
  {
  }

  HNDL Scene::CreateObject(const std::string& name, HNDL parent)
  {
    HNDL objHandle = ObjectFactory::Instance()->CreateObject(name)->GetHndl();

    if (parent == SF_INV_HANDLE)
      m_rootObjects.push_back(objHandle);
    else
      ObjectFactory::Instance()->GetObject(objHandle)->SetParent(parent);

    return objHandle;
  }

  void Scene::AddObject(HNDL object, HNDL parent)
  {
    ObjectFactory::Instance()->GetObject(object)->SetParent(parent);

    if (parent == SF_INV_HANDLE)
      m_rootObjects.push_back(object);
  }

  void Scene::RemoveFromRoot(HNDL object)
  {
    m_rootObjects.remove(object);
  }

}
