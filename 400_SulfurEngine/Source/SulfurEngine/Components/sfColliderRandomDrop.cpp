/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderRandomDrop.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    12/8/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfColliderRandomDrop.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Types\sfObject.hpp"
#include "Modules\Time\sfTime.hpp"

#include "sfTransform.hpp"
#include "sfRigidBody.hpp"
#include "sfBoxCollider.hpp"
#include "sfSphereCollider.hpp"
#include "sfCapsuleCollider.hpp"
#include "sfMeshRenderer.hpp"

#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Modules\Scene\sfSceneManager.hpp"
namespace Sulfur
{
  ColliderRandomDrop::ColliderRandomDrop(void) : m_delta(2.0), m_dropHeight(5.0),
    m_elapsed(0.0)
  {

  }

  ColliderRandomDrop::~ColliderRandomDrop(void)
  {

  }

  void ColliderRandomDrop::Initialize(void)
  {
    srand((unsigned)time(NULL));
  }

  ColliderRandomDrop* ColliderRandomDrop::Clone(void) const
  {
    ColliderRandomDrop *newComp = SF_CREATE_COMP(ColliderRandomDrop);

    newComp->m_delta = m_delta;

    return newComp;
  }

  void ColliderRandomDrop::Update(void)
  {
    m_elapsed += Time::Instance()->GetDt();

    if (m_elapsed < m_delta)
     return;

    m_elapsed -= m_delta;

    Transform *trans = SF_GET_OBJECT(m_owner)->GetComponent<Transform>();
    
    Physics::ColliderType type = Physics::ColliderType(rand() % Physics::CT_NUM_TYPES);

    Object *randObj = SF_CREATE_OBJECT("randObj");
    RigidBody *rb = SF_CREATE_COMP(RigidBody);
    rb->SetDynamicState(Physics::RB_Dynamic);
    randObj->AttachComponent(rb);

    switch (type)
    {
    case Physics::CT_SPHERE:
    {
      randObj->AttachComponent(SF_CREATE_COMP(SphereCollider));

      MeshRenderer *mesh = SF_CREATE_COMP(MeshRenderer);
      mesh->SetMesh("Models\\sphere.fbx");
      randObj->AttachComponent(mesh);
    }
      break;
    case Physics::CT_CAPSULE:
    {
      randObj->AttachComponent(SF_CREATE_COMP(CapsuleCollider));

      MeshRenderer *mesh = SF_CREATE_COMP(MeshRenderer);
      mesh->SetMesh("Models\\normCapsule.fbx");
      randObj->AttachComponent(mesh);
    }
      break;
    case Physics::CT_BOX:
    {
      randObj->AttachComponent(SF_CREATE_COMP(BoxCollider));

      MeshRenderer *mesh = SF_CREATE_COMP(MeshRenderer);
      mesh->SetMesh("Models\\cube.fbx");
      randObj->AttachComponent(mesh);
    }
      break;
    default:
      SF_ASSERT(false, "Unrecognized collider type");
    }

    Vector3 pos = trans->GetTranslation();
    Vector3 scale = trans->GetScale();
    Real xVal = scale[0] / 2;

    xVal = (rand() % (2*(int)xVal + 1)) - xVal;

    pos[1] += m_dropHeight;
    pos[0] = xVal;


    SceneManager::Instance()->GetScene().AddObject(randObj->GetHndl());
    randObj->GetComponent<Transform>()->SetTranslation(pos);
  }

  void ColliderRandomDrop::DrawDebug(DebugDraw *draw) const
  {

  }
}