#include "sfRigidBodyData.hpp"
#include "Components\sfRigidBody.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Components\sfTransform.hpp"

namespace Sulfur
{
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

      //Transform
      Transform *trans = SF_GET_OBJECT(rbComp->GetOwner())->GetComponent<Transform>();
      m_transformHndl = trans->GetHndl();
      m_position = trans->GetTranslation();
      m_orientation = trans->GetRotation();

      m_forces = m_acceleration = Vector3(Real(0.0), Real(0.0), Real(0.0));
    }
  }
}