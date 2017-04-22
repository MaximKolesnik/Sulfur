/******************************************************************************/
/*!
\par     Sulfur
\file    sfColliderDrop.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    3/11/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfColliderDrop.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Types\sfObject.hpp"

#include "sfTransform.hpp"
#include "sfRigidBody.hpp"
#include "sfSphereCollider.hpp"
#include "sfMeshCollider.hpp"
#include "sfMeshRenderer.hpp"

#include "Modules\Physics\Data\sfRigidBodyData.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Modules\Scene\sfSceneManager.hpp"
#include "Modules\Input\sfInputManager.hpp"

namespace Sulfur
{
  ColliderDrop::ColliderDrop(void) : m_dropOffset(Real(0.0), Real(0.0), Real(0.0))
  {

  }

  ColliderDrop::~ColliderDrop(void)
  {
    _DestroyObjects();
  }

  void ColliderDrop::Initialize(void)
  {

  }

  ColliderDrop* ColliderDrop::Clone(void) const
  {
    ColliderDrop *newComp = SF_CREATE_COMP(ColliderDrop);

    newComp->m_dropOffset = m_dropOffset;

    return newComp;
  }

  void ColliderDrop::Update(void)
  {
    if (InputManager::Instance()->WasKeyReleased(VK_F1))
      _CreateSpheres();
    else if (InputManager::Instance()->WasKeyReleased(VK_F2))
      _CreateBoxes();
  }

  void ColliderDrop::DrawDebug(DebugDraw *draw) const
  {

  }

  void ColliderDrop::_CreateSpheres(void)
  {
    _DestroyObjects();

    Transform *ownerTrans = SF_GET_COMP_TYPE(Transform, m_owner);

    for (int i = 0; i < c_numInRow; ++i)
      for (int j = 0; j < c_numInRow; ++j)
        for (int k = 0; k < c_numInRow; ++k)
        {
          Object *newObj = ObjectFactory::Instance()->CreateObject();
          Transform *trans = SF_GET_COMP_TYPE(Transform, newObj->GetHndl());

          Vector3 pos;
          pos[0] = k * 2 + ownerTrans->GetTranslation()[0];
          pos[1] = i * 2 + ownerTrans->GetTranslation()[1];
          pos[2] = j * 2 + ownerTrans->GetTranslation()[2];

          trans->SetTranslation(pos + m_dropOffset);
          trans->Update();
          MeshRenderer *mesh = SF_CREATE_COMP(MeshRenderer);
          mesh->SetMesh("Models\\sphere.fbx");
          newObj->AttachComponent(mesh);

          RigidBody *rb = SF_CREATE_COMP(RigidBody);
          rb->SetDynamicState(Physics::RB_Dynamic);
          newObj->AttachComponent(rb);

          newObj->AttachComponent( SF_CREATE_COMP(SphereCollider) );

          m_createdObjects[i][j][k] = newObj->GetHndl();

          SceneManager::Instance()->GetScene().AddObject(newObj->GetHndl());
        }

    m_created = true;
  }

  void ColliderDrop::_CreateBoxes(void)
  {
    _DestroyObjects();
    //hack
    ObjectFactory::Instance()->EndFrameCleanUp();
    Transform *ownerTrans = SF_GET_COMP_TYPE(Transform, m_owner);

    for (int i = 0; i < c_numInRow; ++i)
      for (int j = 0; j < c_numInRow; ++j)
        for (int k = 0; k < c_numInRow; ++k)
        {
          Object *newObj = ObjectFactory::Instance()->CreateObject();
          Transform *trans = SF_GET_COMP_TYPE(Transform, newObj->GetHndl());

          Vector3 pos;
          pos[0] = k * 2 + ownerTrans->GetTranslation()[0];
          pos[1] = i * 2 + ownerTrans->GetTranslation()[1];
          pos[2] = j * 2 + ownerTrans->GetTranslation()[2];

          trans->SetTranslation(pos + m_dropOffset);
          trans->Update();
          MeshRenderer *mesh = SF_CREATE_COMP(MeshRenderer);
          mesh->SetMesh("Models\\cube.fbx");
          newObj->AttachComponent(mesh);

          RigidBody *rb = SF_CREATE_COMP(RigidBody);
          rb->SetDynamicState(Physics::RB_Dynamic);
          newObj->AttachComponent(rb);

          MeshCollider *col = SF_CREATE_COMP(MeshCollider);
          col->SetColliderMesh("Models\\cube.fbx");
          newObj->AttachComponent(col);

          m_createdObjects[i][j][k] = newObj->GetHndl();

          SceneManager::Instance()->GetScene().AddObject(newObj->GetHndl());
        }

    m_created = true;
  }

  void ColliderDrop::_DestroyObjects(void)
  {
    if (!m_created)
      return;

    for (int i = 0; i < c_numInRow; ++i)
      for (int j = 0; j < c_numInRow; ++j)
        for (int k = 0; k < c_numInRow; ++k)
        {
          ObjectFactory::Instance()->DestroyObject(m_createdObjects[i][j][k]);
          SceneManager::Instance()->GetScene().RemoveObject(m_createdObjects[i][j][k]);
        }
  }
}