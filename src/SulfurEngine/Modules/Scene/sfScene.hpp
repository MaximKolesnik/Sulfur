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
#include "Types/sfObject.hpp"
#include "Reflection/sfReflection.hpp"

namespace Sulfur
{
	
  SF_REFLECTED_CLASS(Scene)
  
  public:
    HNDL CreateObject(const std::string& name, HNDL parent = SF_INV_HANDLE);
    void AddObject(HNDL object, HNDL parent = SF_INV_HANDLE);

    SF_PRIVATE_PROPERTY(std::vector<HNDL>, rootObjects, RootObjects, "Root Objects")
    SF_PRIVATE_PROPERTY(HNDL, cameraObject, CameraObject, "Camera Object")

  };
  
}