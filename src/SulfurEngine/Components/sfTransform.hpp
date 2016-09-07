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

#include "../Types/sfIEntity.h"
#include "../Types/sfTypes.hpp"
#include "../Math/sfVector3.hpp"
#include "../Math/sfQuaternion.hpp"
#include "../Factories/sfComponentFactory.hpp"

namespace Sulfur
{
  class Transform : public IEntity
  {
  public:
    Transform(void);
    virtual ~Transform(void);

    virtual void Initialize(void) override;
    virtual void Update(Real dt) override;
    virtual Transform* Clone(void) const override;

    void SetScale(const Vector3 &scale) { m_scale = scale; }
    void SetScale(const Real &x, const Real &y, const Real &z) 
    {
      m_scale.Set(x, y, z);
    }

    void SetTranslation(const Vector3 &translation) { m_translation = translation;}
    void SetTranslation(const Real &x, const Real &y, const Real &z)
    {
      m_translation.Set(x, y, z);
    }

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

    void SetRotation(const Matrix3 &m) { m_rotation.Set(m); }
    void SetRotation(const Matrix4 &m) { m_rotation.Set(m); }

    Vector3 GetScale(void) const { return m_scale; }
    void GetScale(Vector3 &scale) const { scale = m_scale; }

    Vector3 GetTranslation(void) const { return m_translation; }
    void GetTranslation(Vector3 &translation) const { translation = m_translation; }

  private:
    Vector3    m_scale;
    Vector3    m_translation;
    Quaternion m_rotation;
  };

  //REGISTER_COMPONENT(Transform)
}