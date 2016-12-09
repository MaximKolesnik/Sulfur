/******************************************************************************/
/*!
\par     Sulfur
\file    sfConstraintSolver.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfConstraintSolver.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Modules\Time\sfTime.hpp"
#include "Modules\Physics\Collision\sfContact.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Components\sfTransform.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void ConstraintSolver::Solve(Contacts &contacts) const
    {
      _PrepareContacts(contacts);
      for (int i = 0; i < 10; ++i)
        _ApplyImpulses(contacts);
    }

    void ConstraintSolver::_PrepareContacts(Contacts &contacts) const
    {
      for (auto &it : contacts)
      {
        RigidBodyData *bodyA = it.m_colliderA->m_rbData, *bodyB = it.m_colliderB->m_rbData;

        Vector3 rA = it.m_contactPoint - bodyA->m_position;
        Vector3 rB = it.m_contactPoint - bodyB->m_position;

        Real sumInvMass = bodyA->m_invMass + bodyB->m_invMass;

        Vector3 rACrossN = rA.Cross(it.m_contactNormal);
        Vector3 rBCrossN = rB.Cross(it.m_contactNormal);
        Real kn = (bodyA->m_invInertia * rACrossN).Dot(rACrossN)
          + (bodyB->m_invInertia * rBCrossN).Dot(rBCrossN);

        kn += sumInvMass;
        it.m_massNormal = Real(1.0) / kn;

        if (MathUtils::Abs(it.m_contactNormal[0]) >= Real(0.57735027))
          it.m_tangent1.Set(it.m_contactNormal[1], -it.m_contactNormal[0], Real(0.0));
        else
          it.m_tangent1.Set(Real(0.0), -it.m_contactNormal[2], -it.m_contactNormal[1]);

        it.m_tangent1.Normalize();
        it.m_tangent2 = it.m_contactNormal.Cross(it.m_tangent1);

        Vector3 rat1 = it.m_tangent1.Cross(rA);
        Vector3 rbt1 = it.m_tangent1.Cross(rB);
        Real kt1 = rat1.Dot(bodyA->m_invInertia * rat1) + rbt1.Dot(bodyB->m_invInertia * rbt1);
        it.m_massTangent1 = Real(1.0) / kt1;

        Vector3 rat2 = it.m_tangent2.Cross(rA);
        Vector3 rbt2 = it.m_tangent2.Cross(rB);
        Real kt2 = rat2.Dot(bodyA->m_invInertia * rat2) + rbt2.Dot(bodyB->m_invInertia * rbt2);
        it.m_massTangent2 = Real(1.0) / kt2;

        Vector3 relVel = bodyB->m_velocity + bodyB->m_angularVelocity.Cross(rB)
          - bodyA->m_velocity - bodyA->m_angularVelocity.Cross(rA);
        Vector3 relVelTan = relVel - it.m_contactNormal * (relVel.Dot(it.m_contactNormal));

        it.m_bias = -PhysicsWorld::c_biasFactor * (Real(1.0) / EngineSettings::DefaultFPS)
          * std::min(Real(0.0), -it.m_penetration + PhysicsWorld::c_allowedPenetration);

        Real dv = relVel.Dot(it.m_contactNormal);
        if (dv < -Real(5.0))
          it.m_bias += -dv * (0.0f);
      }
    }

    void ConstraintSolver::_ApplyImpulses(Contacts &contacts) const
    {
      for (auto &it : contacts)
      {
        RigidBodyData *bodyA = it.m_colliderA->m_rbData, *bodyB = it.m_colliderB->m_rbData;

        Vector3 rA = it.m_contactPoint - bodyA->m_position;
        Vector3 rB = it.m_contactPoint - bodyB->m_position;

        Vector3 dv = bodyB->m_velocity + bodyB->m_angularVelocity.Cross(rB)
          - bodyA->m_velocity - bodyA->m_angularVelocity.Cross(rA);

        Real dvt = dv.Dot(it.m_tangent1);
        Real dPt = it.m_massTangent1 * (-dvt);
        Real maxPt = Real(0.8) * it.m_massNormal;
        dPt = std::max(-maxPt, std::min(dPt, maxPt));
        Vector3 Pt = dPt * it.m_tangent1;

        bodyA->m_velocity -= bodyA->m_invMass * Pt;
        bodyA->m_angularVelocity -= bodyA->m_invInertia * rA.Cross(Pt);

        bodyB->m_velocity += bodyB->m_invMass * Pt;
        bodyB->m_angularVelocity += bodyB->m_invInertia * rB.Cross(Pt);

        dvt = dv.Dot(it.m_tangent2);
        dPt = it.m_massTangent2 * (-dvt);
        maxPt = Real(0.3) * it.m_massNormal;
        dPt = std::max(-maxPt, std::min(dPt, maxPt));
        Pt = dPt * it.m_tangent2;

        bodyA->m_velocity -= bodyA->m_invMass * Pt;
        bodyA->m_angularVelocity -= bodyA->m_invInertia * rA.Cross(Pt);

        bodyB->m_velocity += bodyB->m_invMass * Pt;
        bodyB->m_angularVelocity += bodyB->m_invInertia * rB.Cross(Pt);

        dv = bodyB->m_velocity + bodyB->m_angularVelocity.Cross(rB)
          - bodyA->m_velocity - bodyA->m_angularVelocity.Cross(rA);

        Real vn = dv.Dot(it.m_contactNormal);
        Real dPn = it.m_massNormal * (-vn + it.m_bias);

        dPn = std::max(Real(0.0), dPn);

        Vector3 Pn = dPn * it.m_contactNormal;

        bodyA->m_velocity -= bodyA->m_invMass * Pn;
        bodyA->m_angularVelocity -= bodyA->m_invInertia * rA.Cross(Pn);

        bodyB->m_velocity += bodyB->m_invMass * Pn;
        bodyB->m_angularVelocity += bodyB->m_invInertia * rB.Cross(Pn);

      }
    }
  }
}