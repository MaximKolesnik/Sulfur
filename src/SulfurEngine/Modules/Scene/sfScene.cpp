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

}
