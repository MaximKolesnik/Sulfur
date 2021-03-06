/******************************************************************************/
/*!
\par     Sulfur
\file    sfUpdateTransforms.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/15/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "../sfTransform.hpp"
#include "../../Factories/sfObjectFactory.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "../../Managers/TaskManager/sfTaskManager.hpp"

namespace Sulfur
{
  void UpdateTransform(HNDL objectHandle)
  {
    Object *object = SF_GET_OBJECT(objectHandle);

    Transform *transform = object->GetComponent<Transform>();
    transform->Update();

    auto& children = object->GetChildren();
    for (HNDL child : children)
      UpdateTransform(child);
  }

  SF_DEFINE_TASK(UpdateTransformHierarchy)
  {
    std::vector<HNDL>* roots = reinterpret_cast<std::vector<HNDL>*>(_taskData->m_data);

    for (UINT32 i = 0; i < roots->size(); ++i)
      UpdateTransform(roots->at(i));
  }
  SF_END_DEFINE_TASK(UpdateTransformHierarchy)

    SF_DEFINE_TASK(UpdateTransforms)
  {
    const UINT32 JOBS = 4;
    static std::vector<HNDL> roots[JOBS];
    static void *data[JOBS];

    Scene& scene = SceneManager::Instance()->GetScene();
    auto& rootObjects = scene.GetRootObjects();
    UINT32 transformsPerJob = (UINT32)((rootObjects.size() + JOBS - 1) / JOBS);
    auto it = rootObjects.begin();
    for (UINT32 i = 0; i < JOBS; ++i)
    {
      data[i] = roots + i;
      roots[i].clear();
      if (it == rootObjects.end()) continue;

      for (UINT32 t = 0; t < transformsPerJob && it != rootObjects.end(); ++t)
        roots[i].push_back(*it++);
    }

    SF_ENQUEUE_JOBS(UpdateTransformHierarchy, data, JOBS, JOBS);
    SF_YEILD_AND_WAIT();
  }
  SF_END_DEFINE_TASK(UpdateTransforms)
}