/******************************************************************************/
/*!
\par     Sulfur
\file    sfExplicitEuler.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfExplicitEuler.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Modules\Time\sfTime.hpp"
namespace Sulfur
{
  namespace Physics
  {
    Quaternion QuatIntegrate(const Quaternion& q, const Vector3& omega, float deltaT)
    {
      Quaternion deltaQ;
      Vector3 theta = omega * deltaT * 0.5f;
      float thetaMagSq = theta.LengthSq();
      float s;
      if (thetaMagSq * thetaMagSq / 24.0f < SF_EPSILON)
      {
        deltaQ[0] = 1.0f - thetaMagSq / 2.0f;
        s = 1.0f - thetaMagSq / 6.0f;
      }
      else
      {
        float thetaMag = sqrt(thetaMagSq);
        deltaQ[0] = cos(thetaMag);
        s = sin(thetaMag) / thetaMag;
      }
      deltaQ[1] = theta[0] * s;
      deltaQ[2] = theta[1] * s;
      deltaQ[3] = theta[2] * s;
      return deltaQ * q;
    }

    void ExplicitEuler::Integrate(RigidBodyData *bData)
    {
      bData->m_forces += PhysicsWorld::c_gravity;
      bData->m_acceleration = bData->m_forces * bData->m_invMass;

      bData->m_velocity = bData->m_velocity + bData->m_acceleration * EngineSettings::DefaultFPS;
      bData->m_position = bData->m_position + bData->m_velocity * EngineSettings::DefaultFPS;

      bData->m_orientation = QuatIntegrate(bData->m_orientation, bData->m_angularVelocity, EngineSettings::DefaultFPS);

      static Real damping = pow(Real(0.85), EngineSettings::DefaultFPS);

      bData->m_angularVelocity = damping * bData->m_angularVelocity;
      bData->m_velocity = damping * bData->m_velocity;
    }
  }
}