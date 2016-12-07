#include "sfRigidBodyData.hpp"
#include "Components\sfRigidBody.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Components\sfTransform.hpp"

/******************************************************************************
Maxim TODO: INERTIA IS SET ONLY FOR A BOX COLLIDER
*******************************************************************************/

namespace Sulfur
{
  SF_BASE_TYPE_SERIALIZATION_DEFAULT(Physics::RBDynamicState);

  namespace Physics
  {
    void RigidBodyData::Initialize(void)
    {
      SF_ASSERT(m_compHndl != SF_INV_HANDLE, "RigidBody handle is not set");

      //RigidBody
      RigidBody *rbComp = SF_GET_COMP_TYPE(RigidBody, m_compHndl);
      m_velocity = rbComp->GetVelocity();
      m_invMass = rbComp->GetInverseMass();
      m_state = rbComp->GetDynamicState();
      m_angularVelocity = rbComp->GetAngularVelocity();

      //Transform
      Transform *trans = SF_GET_OBJECT(rbComp->GetOwner())->GetComponent<Transform>();
      m_transformHndl = trans->GetHndl();
      m_position = trans->GetTranslation();
      m_orientation = trans->GetRotation();

      m_forces = m_acceleration = Vector3(Real(0.0), Real(0.0), Real(0.0));

      if (m_state == RB_Static)
        m_invMass = 0.0;
      else
      {
        Vector3 scale = trans->GetScale();
        m_invMass = 1 / (scale[0] * scale[1] * scale[2]);
      }


      if (m_invMass != Real(0.0))
      {
        Vector3 scale = trans->GetScale();
        Real mul = (Real(1.0) / m_invMass) * (Real(1.0) / 12);

        Real xx = (scale[1] * scale[1] + scale[2] * scale[2]) * mul;
        Real yy = (scale[0] * scale[0] + scale[2] * scale[2]) * mul;
        Real zz = (scale[0] * scale[0] + scale[1] * scale[1]) * mul;

        m_inertia.Set(xx, Real(0.0), Real(0.0),
                      Real(0.0), yy, Real(0.0),
                      Real(0.0), Real(0.0), zz);

        m_invInertia.Set((Real)1.0 / xx, Real(0.0), Real(0.0),
          Real(0.0), (Real)1.0 / yy, Real(0.0),
          Real(0.0), Real(0.0), (Real)1.0 / zz);
      }
      else
      {
        m_inertia.Set(Real(0.0), Real(0.0), Real(0.0),
          Real(0.0), Real(0.0), Real(0.0),
          Real(0.0), Real(0.0), Real(0.0));

        m_invInertia.Set(Real(0.0), Real(0.0), Real(0.0),
          Real(0.0), Real(0.0), Real(0.0),
          Real(0.0), Real(0.0), Real(0.0));
      }
    }
  }
}