/******************************************************************************/
/*!
\par     Sulfur
\file    sfTransform.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Transform component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "../Types/sfIEntity.hpp"
#include "../Types/sfTypes.hpp"
#include "../Math/sfVector3.hpp"
#include "../Math/sfMatrix4.hpp"
#include "../Math/sfQuaternion.hpp"
#include "../Factories/sfComponentFactory.hpp"
#include "Managers/TaskManager/sfTaskRegistry.hpp"

namespace Sulfur
{

SF_DECLARE_TASK(UpdateTransformHierarchy)
SF_DECLARE_TASK(UpdateTransforms)

SF_REFLECTED_CLASS_DERIVED(Transform, IEntity)

public:
  Transform(void);
  virtual ~Transform(void);

  virtual void Initialize(void) override final;
  virtual Transform* Clone(void) const override final;
  virtual void Update(void) override final;

  void SetRotationDeg(const Vector3 &axis, const Real &angleDeg)
  {
    m_rotation.Set(axis, angleDeg * SF_DEGS_PER_RAD);
  }
  void SetRotationRad(const Vector3 &axis, const Real &angleRad)
  {
    m_rotation.Set(axis, angleRad);
  }

  void SetRotationEulerXZY(const Real &roll, const Real &pitch, const Real &yaw)
  {
    m_rotation.SetEuler(roll, pitch, yaw);
  }

  const Matrix4& GetLocalMatrix() const;
  const Matrix4& GetWorldMatrix() const;
  const Vector3& GetRight() const;
  const Vector3& GetUp() const;
  const Vector3& GetForward() const;

  SF_PRIVATE_PROPERTY(Vector3, translation, Translation, "Translation")
  SF_PRIVATE_PROPERTY(Quaternion, rotation, Rotation, "Rotation")
  SF_PRIVATE_PROPERTY(Vector3, scale, Scale, "Scale")

private:
  Matrix4 m_localMatrix;
  Matrix4 m_worldMatrix;
  Vector3 m_right;
  Vector3 m_up;
  Vector3 m_forward;

};

}
