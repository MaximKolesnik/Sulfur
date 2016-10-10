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
#pragma once
#include "Reflection/sfReflection.hpp"
#include "Reflection/sfSerialization.hpp"
#include "Modules/Graphics/Resources/Texture/sfCubeMap.hpp"

namespace Sulfur
{

  SF_REFLECTED_CLASS(SceneProperties)
  
  public:
    SceneProperties();

    SF_RESOURCE(CubeMap, skybox, Skybox, "Skybox")
    SF_PRIVATE_PROPERTY(bool, ibl, Ibl, "IBL")

  };

  SF_REFLECTED_CLASS(Scene)
  
  public:
    Scene();

    HNDL CreateObject(const std::string& name, HNDL parent = SF_INV_HANDLE);
    void AddObject(HNDL object, HNDL parent = SF_INV_HANDLE);
    void RemoveFromRoot(HNDL object);

    SF_PRIVATE_PROPERTY(std::list<HNDL>, rootObjects, RootObjects, "Root Objects")
    SF_PRIVATE_PROPERTY(HNDL, cameraObject, CameraObject, "Camera Object")
    SF_PUBLIC_PROPERTY(SceneProperties, sceneProperties, SceneProperties, "Scene Properties")

  };

  SF_BASE_TYPE_SPECIALIZATION(Scene)
  
}