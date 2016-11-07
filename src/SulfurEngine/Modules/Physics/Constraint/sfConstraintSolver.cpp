#include "sfConstraintSolver.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Modules\Time\sfTime.hpp"

namespace Sulfur
{
  namespace Physics
  {
    void ConstraintSolver::Solve(Contacts &contacts) const
    {
      _PrepareContacts(contacts);
      for (int i = 0; i < 2; ++i)
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
        Real kn = (((bodyA->m_invInertia * rACrossN).Cross(rA))
          + (bodyB->m_invInertia * rBCrossN).Cross(rB)).Dot(it.m_contactNormal);
        kn += sumInvMass;

        it.m_massNormal = Real(1.0) / kn;

        Vector3 relVel = bodyB->m_velocity + bodyB->m_angularVelocity.Cross(rB)
          - bodyA->m_velocity - bodyA->m_angularVelocity.Cross(rA);;
        Vector3 relVelTan = relVel - it.m_contactNormal * (relVel.Dot(it.m_contactNormal));

        it.m_tangent = relVel - it.m_contactNormal * Dot(relVel, it.m_contactNormal);
        if (!it.m_tangent.IsZeroEpsilon())
          it.m_tangent.Normalize();

        Vector3 rACrossT = rA.Cross(it.m_tangent);
        Vector3 rBCrossT = rB.Cross(it.m_tangent);
        Real kt = (((bodyA->m_invInertia * rACrossT).Cross(rA))
          + (bodyB->m_invInertia * rBCrossT).Cross(rB)).Dot(it.m_tangent);
        kt += sumInvMass;
        it.m_massTangent1 = Real(1.0) / kn;

        /*Vector3 t1, t2;
        if (relVelTan.IsZeroEpsilon())
          GramSchmidt(it.m_contactNormal, t1, t2);
        else
        {
          relVelTan.Normalize();
          t1 = relVelTan;
          t2 = t1.Cross(it.m_contactNormal);
        }

        Vector3 rACrossT1 = rA.Cross(t1);
        Vector3 rBCrossT1 = rB.Cross(t2);

        Real kt1 = ((bodyA->m_invInertia * rACrossT1.Cross(rA))
          + (bodyB->m_invInertia * rBCrossT1.Cross(rB))).Dot(t1);
        kt1 += sumInvMass;

        it.m_massTangent1 = Real(1.0) / kt1;

        Vector3 rACrossT2 = rA.Cross(t2);
        Vector3 rBCrossT2 = rB.Cross(t2);

        Real kt2 = ((bodyA->m_invInertia * rACrossT2).Cross(rA)
          + (bodyB->m_invInertia * rBCrossT2).Cross(rB)).Dot(t2);
        kt2 += sumInvMass;

        it.m_massTangent2 = Real(1.0) / kt2;*/

        it.m_bias = PhysicsWorld::c_biasFactor * (Real(1.0) / EngineSettings::DefaultFPS)
          * std::max(Real(0.0), it.m_penetration - PhysicsWorld::c_allowedPenetration);
        /*it.m_bias = Dot(it.m_contactNormal, bodyB->m_velocity + bodyB->m_angularVelocity.Cross(rB)
          - bodyA->m_velocity - bodyA->m_angularVelocity.Cross(rA)) * (-0.5f);*/

        /*Vector3 P = it.Pn * it.m_contactNormal + it.Pt * it.m_tangent;

        body1->velocity -= body1->invMass * P;
        body1->angularVelocity -= body1->invI * Cross(r1, P);

        body2->velocity += body2->invMass * P;
        body2->angularVelocity += body2->invI * Cross(r2, P);*/
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

        Real vn = dv.Dot(it.m_contactNormal);
        Real dPn = it.m_massNormal * (-vn + it.m_bias);

        dPn = std::max(Real(0.0), dPn);

        Vector3 Pn = dPn * it.m_contactNormal;

        bodyA->m_velocity -= bodyA->m_invMass * Pn;
        bodyA->m_angularVelocity -= bodyA->m_invInertia * rA.Cross(Pn);

        bodyB->m_velocity += bodyB->m_invMass * Pn;
        bodyB->m_angularVelocity += bodyB->m_invInertia * rB.Cross(Pn);

        dv = bodyB->m_velocity + bodyB->m_angularVelocity.Cross(rB)
          - bodyA->m_velocity - bodyA->m_angularVelocity.Cross(rA);

        Real dvt = dv.Dot(it.m_tangent);
        Real dPt = (0.6f) * it.m_massTangent1 * (-dvt);
        Vector3 Pt = dPt * it.m_tangent;
        bodyA->m_velocity -= bodyA->m_invMass * Pt;
        bodyA->m_angularVelocity -= bodyA->m_invInertia * rA.Cross(Pt);

        bodyB->m_velocity += bodyB->m_invMass * Pt;
        bodyB->m_angularVelocity += bodyB->m_invInertia * rB.Cross(Pt);

        //Vector3 relVel = bodyB->m_velocity + bodyB->m_angularVelocity.Cross(rB)
        //  - bodyA->m_velocity - bodyA->m_angularVelocity.Cross(rA);
        //Vector3 relVelTan = relVel - it.m_contactNormal * (relVel.Dot(it.m_contactNormal));

        //Vector3 t1, t2;
        //if (relVelTan.IsZeroEpsilon())
        //  GramSchmidt(it.m_contactNormal, t1, t2);
        //else
        //{
        //  relVelTan.Normalize();
        //  t1 = relVelTan;
        //  t2 = t1.Cross(it.m_contactNormal);
        //}

        //float dPt1 = (it.m_massTangent1 * (-Dot(dv, t1)));
        //Real dPt2 = (it.m_massTangent2 * (-Dot(dv, t2)));

        ////Real maxPt = Real(0.8) * dPn;
        ////dPt = std::max(-maxPt, std::min(dPt, maxPt));

        ////Vector3 Pt = dPt * t1;

        //bodyA->m_velocity -= bodyA->m_invMass * dPt1 * t1;
        ////bodyA->m_angularVelocity -= bodyA->m_invInertia * Cross(rA, dPt1 * t1);

        //bodyB->m_velocity += bodyB->m_invMass * dPt2 * t2;
        ////bodyB->m_angularVelocity += bodyB->m_invInertia * Cross(rB, dPt2 * t2);
      }
    }
  }
}