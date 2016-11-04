/******************************************************************************/
/*!
\par     Sulfur
\file    sfSceneManager.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/16/2016

\brief   Scene manager

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfSingleton.hpp"
#include "sfScene.hpp"

namespace Sulfur
{
	
  class SceneManager
  {
    SF_SINGLETON(SceneManager);
  public:
    Scene& GetScene();
    const Scene& GetScene() const;

    void Update();

  private:
    Scene m_scene;
  };
  
}