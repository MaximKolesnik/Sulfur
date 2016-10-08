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

namespace Sulfur
{
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

    Object *object = g_SystemTable->ObjFactory->GetObject(m_owner);

    if (object->GetOwner() != SF_INV_HANDLE)
    {
      Transform *parentTransform = g_SystemTable->ObjFactory->GetObject(object->GetOwner())->GetComponent<Transform>();
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

  const Vector3& Transform::GetRight() const
  {
    return m_right;
  }

  const Vector3& Transform::GetUp() const
  {
    return m_up;
  }

  const Vector3& Transform::GetForward() const
  {
    return m_forward;
  }

}