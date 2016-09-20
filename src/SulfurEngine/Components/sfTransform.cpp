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
#include "../Error/sfError.hpp"
#include "Factories\sfComponentFactory.hpp"

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
    m_transformMatrix.SetTransformation(m_rotation, m_scale, m_translation);
    m_right = m_transformMatrix.TransformNormal(Vector3(1.0f, 0.0f, 0.0f));
    m_up = m_transformMatrix.TransformNormal(Vector3(0.0f, 1.0f, 0.0f));
    m_forward = m_transformMatrix.TransformNormal(Vector3(0.0f, 0.0f, 1.0f));
  }

  const Matrix4& Transform::GetTransformMatrix() const
  {
    return m_transformMatrix;
  }

}