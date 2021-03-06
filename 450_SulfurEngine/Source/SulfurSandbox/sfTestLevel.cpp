/******************************************************************************/
/*!
\par     Sulfur
\file    sfTestLevel.cpp
\author  Maxim Kolesnik
\par     12/1/2016
\date    DP email: maxim.kolesnik@digipen.edu

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfTestLevel.hpp"
#include "sfSulfurPCH.hpp"
#include "Core/sfCore.hpp"
#include "Reflection\sfSerialization.hpp"
#include "Modules\Scene\sfSceneManager.hpp"
#include "Components\sfCamera.hpp"
#include "Factories\sfComponentFactory.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Components\sfRigidBody.hpp"
#include "Components\sfSphereCollider.hpp"
#include "Components\sfMeshCollider.hpp"
#include "Components\sfCapsuleCollider.hpp"
#include "Components\sfTransform.hpp"
#include "Components\sfMeshRenderer.hpp"
#include "Components\sfSpotLight.hpp"
#include "Components\sfColliderRandomDrop.hpp"

using namespace Sulfur;

static void TestRB(Vector3 &pos)
{
  Object *testObj1 = SF_CREATE_OBJECT("testObj1");
  testObj1->GetComponent<Transform>()->SetTranslation(pos);
  testObj1->GetComponent<Transform>()->SetRotationEulerXZY(SF_PI / 2, 0, 0.0);
  testObj1->GetComponent<Transform>()->Update();
  RigidBody *rb1 = SF_CREATE_COMP(RigidBody);
  rb1->SetDynamicState(Physics::RB_Dynamic);
  testObj1->AttachComponent(rb1);
  MeshCollider *meshCol = SF_CREATE_COMP(MeshCollider);
  testObj1->AttachComponent(meshCol);
  meshCol->SetColliderMesh("Models\\cube.fbx");
  SceneManager::Instance()->GetScene().AddObject(testObj1->GetHndl());
}

void SetupLevel(void)
{
  std::ifstream file("Resources\\TestScene.ssc", std::ios_base::binary);
  if (file.is_open())
    Serialization::Deserialize(file, SceneManager::Instance()->GetScene());

  Object *cameraObj = SF_CREATE_OBJECT("Camera");
  SceneManager::Instance()->GetScene().AddObject(cameraObj->GetHndl());

  Camera *camera = ComponentFactory::Instance()->CreateComponent<Camera>();
  cameraObj->AttachComponent(camera);
  SceneManager::Instance()->GetScene().SetCameraObject(cameraObj->GetHndl());
  SceneManager::Instance()->GetScene().m_sceneProperties.SetIbl(true);

  Object *testObj2 = SF_CREATE_OBJECT("testObj2");
  testObj2->GetComponent<Transform>()->SetTranslation(Vector3(2.5f, -1.0f, 20.0));
  testObj2->GetComponent<Transform>()->SetScale(Vector3(10, 1, 4));
  testObj2->GetComponent<Transform>()->SetRotationEulerXZY(0, -SF_PI / 10, 0.0);
  //testObj2->GetComponent<Transform>()->SetScale(Vector3(20.0, 1.0f, 5.0));
  testObj2->GetComponent<Transform>()->Update();
  RigidBody *rb2 = SF_CREATE_COMP(RigidBody);
  rb2->SetDynamicState(Physics::RB_Static);
  testObj2->AttachComponent(rb2);
  MeshCollider *meshCol = SF_CREATE_COMP(MeshCollider);
  testObj2->AttachComponent(meshCol);
  meshCol->SetColliderMesh("Models\\cube.fbx");
  SceneManager::Instance()->GetScene().AddObject(testObj2->GetHndl());

  /*TestRB(Vector3(0.0, -3.0f, 20.0));
  TestRB(Vector3(0.0, -2.0f, 20.0));
  TestRB(Vector3(0.0, -1.0f, 20.0));*/

  TestRB(Vector3(0.0f, 1.2f, 20.0f));
  /*TestRB(Vector3(0.5, -2.6f, 20.0));
  TestRB(Vector3(-0.6f, -1.4f, 20.0));
  TestRB(Vector3(0.0, -0.2f, 20.0));
  TestRB(Vector3(1.2f, 1.0f, 20.0));
  TestRB(Vector3(0.0, 2.2f, 20.0));
  TestRB(Vector3(0.0, 3.4f, 20.0));
  TestRB(Vector3(0.0, 4.6f, 20.0));

  TestRB(Vector3(-3.0, -1, 20.0));
  TestRB(Vector3(-3.0, 2, 20.0));
  TestRB(Vector3(-3.0, 4, 20.0));

  TestRB(Vector3(3.0, -1, 20.0));
  TestRB(Vector3(3.0, 2, 20.0));
  TestRB(Vector3(3.0, 4, 20.0));*/

  Object *spotLight = SF_CREATE_OBJECT("sl");
  spotLight->AttachComponent(SF_CREATE_COMP(SpotLight));
  spotLight->GetComponent<SpotLight>()->SetIntensity(100);
  spotLight->GetComponent<SpotLight>()->SetRange(200);
  spotLight->GetComponent<SpotLight>()->SetOuterAngle(100);
  spotLight->GetComponent<SpotLight>()->SetInnerAngle(100);
  SceneManager::Instance()->GetScene().AddObject(spotLight->GetHndl());
  SceneManager::Instance()->GetScene().m_sceneProperties.SetIbl(true);
}