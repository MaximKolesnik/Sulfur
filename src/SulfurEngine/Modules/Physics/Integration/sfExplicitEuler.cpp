#include "sfExplicitEuler.hpp"
#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "Modules\Physics\sfPhysicsWorld.hpp"
#include "Modules\Time\sfTime.hpp"
namespace Sulfur
{
  namespace Physics
  {
    void ExplicitEuler::Integrate(RigidBodyData *bData)
    {
      bData->m_forces += PhysicsWorld::c_gravity;
      bData->m_acceleration = bData->m_forces * bData->m_invMass;

      bData->m_velocity = bData->m_velocity + bData->m_acceleration * Time::Instance()->GetDt();
      bData->m_position = bData->m_position + bData->m_velocity * Time::Instance()->GetDt();
    }
  }
}