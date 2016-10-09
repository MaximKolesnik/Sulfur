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

  Scene::Scene()
    : m_cameraObject(SF_INV_HANDLE)
  {
  }

  HNDL Scene::CreateObject(const std::string& name, HNDL parent)
  {
    HNDL objHandle = g_SystemTable->ObjFactory->CreateObject(name)->GetHndl();

    if (parent == SF_INV_HANDLE)
      m_rootObjects.push_back(objHandle);
    else
      g_SystemTable->ObjFactory->GetObject(objHandle)->SetParent(parent);

    return objHandle;
  }

  void Scene::AddObject(HNDL object, HNDL parent)
  {
    g_SystemTable->ObjFactory->GetObject(object)->SetParent(parent);

    if (parent == SF_INV_HANDLE)
      m_rootObjects.push_back(object);
  }

  void Scene::RemoveFromRoot(HNDL object)
  {
    m_rootObjects.remove(object);
  }

}
