/******************************************************************************/
/*!
\par     Sulfur
\file    sfGameplayUpdate.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    12/7/2016

\brief   Gameplay update tasks

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfGameplayUpdate.hpp"
#include "../../Factories/sfObjectFactory.hpp"
#include "../../Managers/TaskManager/sfTaskManager.hpp"
#include "Modules/Time/sfTime.hpp"

// Gameplay components
#include "../sfThirdPersonController.hpp"

namespace Sulfur
{

SF_DEFINE_TASK(UpdateGameplay)
{
  if (!Time::Instance()->IsPaused())
  {
    ComponentFactory::ComponentData tpcData = SF_GET_COMP_DATA(ThirdPersonController);
    for (auto it = tpcData.begin(); it != tpcData.end(); ++it)
      it->Update();
  }
}
SF_END_DEFINE_TASK(UpdateGameplay)

}
