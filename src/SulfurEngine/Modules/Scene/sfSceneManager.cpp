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
#include "sfSceneManager.hpp"

namespace Sulfur
{

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

Scene& SceneManager::GetScene()
{
  return m_scene;
}

const Scene& SceneManager::GetScene() const
{
  return m_scene;
}

}
