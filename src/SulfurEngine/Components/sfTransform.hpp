/******************************************************************************/
/*!
\par     Sulfur
\file    sfTransform.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Transform component

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
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
  SF_REFLECTED_CLASS_DERIVED(Transform, IEntity)

  public:
    Transform(void);
    virtual ~Transform(void);

    virtual void Initialize(void) override;
    virtual void Update(Real dt) override;
    virtual Transform* Clone(void) const override;

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

    SF_PRIVATE_PROPERTY(Vector3, scale, Scale, "Scale")
    SF_PRIVATE_PROPERTY(Vector3, translation, Translation, "Translation")
    SF_PRIVATE_PROPERTY(Quaternion, rotation, Rotation, "Rotation")

  };

  //REGISTER_COMPONENT(Transform)
}