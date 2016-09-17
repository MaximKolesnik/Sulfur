/******************************************************************************/
/*!
\par     Sulfur
\file    sfSceneManager.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/16/2016

\brief   Scene manager

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "sfScene.hpp"

namespace Sulfur
{
	
  class SceneManager
  {
    SINGLETON(SceneManager)

    Scene& GetScene();
    const Scene& GetScene() const;

  private:
    Scene m_scene;

  };
  
}