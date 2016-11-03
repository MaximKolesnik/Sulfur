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
    m_matrix.SetTransformation(m_rotation, m_scale, m_translation);
    m_right = m_matrix.TransformNormal(Vector3(1.0f, 0.0f, 0.0f));
    m_up = m_matrix.TransformNormal(Vector3(0.0f, 1.0f, 0.0f));
    m_forward = m_matrix.TransformNormal(Vector3(0.0f, 0.0f, 1.0f));

    Object *object = SF_GET_OBJECT(m_owner);
    if (object->GetOwner() != SF_INV_HANDLE)
    {
      Transform *parentTransform = SF_GET_OBJECT(object->GetOwner())->GetComponent<Transform>();
      m_worldMatrix = parentTransform->GetWorldMatrix() * m_matrix;
      m_worldTranslation = m_worldMatrix * Vector3(0.0f, 0.0f, 0.0f);
      m_worldRotation = parentTransform->GetWorldRotation() * m_rotation;
      m_worldScale = parentTransform->GetWorldScale();
      m_worldScale = Vector3(m_worldScale[0] * m_scale[0], m_worldScale[1] * m_scale[1], m_worldScale[2] * m_scale[2]);
      m_worldRight = m_worldMatrix.TransformNormal(Vector3(1.0f, 0.0f, 0.0f));
      m_worldUp = m_worldMatrix.TransformNormal(Vector3(0.0f, 1.0f, 0.0f));
      m_worldForward = m_worldMatrix.TransformNormal(Vector3(0.0f, 0.0f, 1.0f));
    }
    else
    {
      m_worldMatrix = m_matrix;
      m_worldTranslation = m_translation;
      m_worldRotation = m_rotation;
      m_worldScale = m_scale;
      m_worldRight = m_right;
      m_worldUp = m_up;
      m_worldForward = m_forward;
    }    
  }

  void Transform::Reparent()
  {
    Object *object = SF_GET_OBJECT(m_owner);
    if (object->GetOwner() != SF_INV_HANDLE)
    {
      Transform *parentTransform = SF_GET_OBJECT(object->GetOwner())->GetComponent<Transform>();
      m_matrix = parentTransform->GetWorldMatrix().Inverted() * m_worldMatrix;
      m_translation = m_matrix * Vector3(0.0f, 0.0f, 0.0f);
      m_rotation = Quaternion(m_matrix);
      m_scale = m_worldScale;
      m_scale = Vector3(m_scale[0] / m_worldScale[0], m_scale[1] / m_worldScale[1], m_scale[2] / m_worldScale[2]);
      m_right = m_matrix.TransformNormal(Vector3(1.0f, 0.0f, 0.0f));
      m_up = m_matrix.TransformNormal(Vector3(0.0f, 1.0f, 0.0f));
      m_forward = m_matrix.TransformNormal(Vector3(0.0f, 0.0f, 1.0f));
    }
    else
    {
      m_matrix = m_worldMatrix;
      m_translation = m_worldTranslation;
      m_rotation = m_worldRotation;
      m_scale = m_worldScale;
      m_right = m_worldRight;
      m_up = m_worldUp;
      m_forward = m_worldForward;
    }
  }

  const Matrix4& Transform::GetMatrix() const
  {
    return m_matrix;
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

  const Matrix4& Transform::GetWorldMatrix() const
  {
    return m_worldMatrix;
  }

  const Vector3& Transform::GetWorldTranslation() const
  {
    return m_worldTranslation;
  }

  const Quaternion& Transform::GetWorldRotation() const
  {
    return m_worldRotation;
  }

  const Vector3& Transform::GetWorldScale() const
  {
    return m_worldScale;
  }

  const Vector3& Transform::GetWorldRight() const
  {
    return m_worldRight;
  }

  const Vector3& Transform::GetWorldUp() const
  {
    return m_worldUp;
  }

  const Vector3& Transform::GetWorldForward() const
  {
    return m_worldForward;
  }

  void Transform::OnTestEvent(OnTestEventData *data)
  {
    SF_LOG_MESSAGE("Trasnform Event");
    SF_LOG_MESSAGE(data->m_message);
  }

}