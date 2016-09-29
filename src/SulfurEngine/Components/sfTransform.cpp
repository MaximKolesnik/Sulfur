/******************************************************************************/
/*!
\par     Sulfur
\file    sfTransform.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Transform component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

/*******************************************************************************
Maxim Kolesnik: TODO: Write Clone() as soon as object and component factory
                      are done.
*******************************************************************************/

#include "sfTransform.hpp"
#include "Factories/sfObjectFactory.hpp"
#include "Factories/sfComponentFactory.hpp"
#include "Managers/TaskManager/sfTaskManager.hpp"
#include "Modules/Scene/sfSceneManager.hpp"

namespace Sulfur
{

void UpdateTransform(HNDL objectHandle)
{
  Object *object = ObjectFactory::Instance()->GetObject(objectHandle);

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

  SF_ENQUEUE_JOBS(UpdateTransformHierarchy, data, JOBS);
  SF_YEILD_AND_WAIT();
}
SF_END_DEFINE_TASK(UpdateTransforms)

  Transform::Transform(void) 
    : IEntity(), m_translation(0.0f, 0.0f, 0.0f), m_rotation(1.0f, 0.0f, 0.0f, 0.0f), m_scale(1.0f, 1.0f, 1.0f)
  {
    
  }

  Transform::~Transform(void)
  {

  }

  void Transform::Initialize(void)
  {

  }

  Transform* Transform::Clone(void) const
  {
    Transform *newTransform = SF_CREATE_COMP(Transform);

    newTransform->m_scale = m_scale;
    newTransform->m_translation = m_translation;
    newTransform->m_rotation = m_rotation;

    return newTransform;
  }

  void Transform::Update()
  {
    m_localMatrix.SetTransformation(m_rotation, m_scale, m_translation);

    Object *object = ObjectFactory::Instance()->GetObject(m_owner);

    if (object->GetOwner() != SF_INV_HANDLE)
    {
      Transform *parentTransform = ObjectFactory::Instance()->GetObject(object->GetOwner())->GetComponent<Transform>();
      m_worldMatrix = parentTransform->GetWorldMatrix() * m_localMatrix;
    }
    else
    {
      m_worldMatrix = m_localMatrix;
    }

    m_right = m_worldMatrix.TransformNormal(Vector3(1.0f, 0.0f, 0.0f));
    m_up = m_worldMatrix.TransformNormal(Vector3(0.0f, 1.0f, 0.0f));
    m_forward = m_worldMatrix.TransformNormal(Vector3(0.0f, 0.0f, 1.0f));
  }

  const Matrix4& Transform::GetLocalMatrix() const
  {
    return m_localMatrix;
  }

  const Matrix4& Transform::GetWorldMatrix() const
  {
    return m_worldMatrix;
  }

}