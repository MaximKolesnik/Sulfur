#include "sfCore.hpp"

// Modules
#include "Modules/Input/sfInputManager.hpp"
#include "Modules/Window/sfWindowManager.hpp"
#include "Modules/Graphics/sfGraphicsManager.hpp"
#include "Modules/Resource/sfResourceManager.hpp"
#include "Modules/Time/sfTime.hpp"
#include "Managers/TaskManager/sfTaskManager.hpp"
#include "Logger/sfLogger.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"
#include "Modules/Scene/sfSceneManager.hpp"
#include "Modules/Graphics/Debug/sfDebugDraw.hpp"
#include "Managers\EventManager\sfEventManager.hpp"

// Factories
#include "Factories/sfComponentFactory.hpp"
#include "Factories/sfObjectFactory.hpp"

//Test
#include "Components/sfCamera.hpp"
#include "Components\sfRigidBody.hpp"
#include "Components\sfMeshRenderer.hpp"
#include "Components\sfTransform.hpp"
#include "Components\sfSpotLight.hpp"
#include "Components\sfBoxCollider.hpp"

namespace Sulfur
{
  Core::Core()
  {
  }

  Core::~Core()
  {
  }

  void Core::StartUp(HWND windowHandle)
  {
    Logger::Instance()->Initialize();
    ComponentFactory::Instance()->Initialize();
    TaskManager::Instance()->Initialize();

    // Start engine in existing window
    if (windowHandle != nullptr)
      m_window = WindowManager::Instance()->AddWindow(windowHandle);
    
    // Create a new window for the engine
    else
    {
      WindowDescription description;
      description.Width = 1280;
      description.Height = 720;
      description.Title = "Sulfur Engine";

      m_window = WindowManager::Instance()->NewWindow(description);
      m_window->RegisterCallbackOnClose(this, &Core::OnWindowClose);
    }

    InputManager::Instance()->Init(m_window);
    GraphicsManager::Instance()->Init(*m_window);
    m_running = true;

    TaskManager* tm = TaskManager::Instance();
    tm->AddNode("UpdateTransforms");
    tm->AddNode("TriggerEventsEndFrame");
    tm->AddNode("IntegrateBodies");
    tm->AddNode("BroadPhase");
    tm->AddNode("PostAndCleanup");
    tm->SetStartingTask("UpdateTransforms");
    tm->SetDependency("IntegrateBodies", "UpdateTransforms");
    tm->SetDependency("BroadPhase", "IntegrateBodies");
    tm->SetDependency("PostAndCleanup", "BroadPhase");
    tm->SetDependency("TriggerEventsEndFrame", "PostAndCleanup");
    tm->CompleteGraph();

    std::ifstream file("Resources\\TestScene.ssc", std::ios_base::binary);
    if (file.is_open())
      Serialization::Deserialize(file, SceneManager::Instance()->GetScene());

    Object *cameraObj = SF_CREATE_OBJECT("Camera");
    SceneManager::Instance()->GetScene().AddObject(cameraObj->GetHndl());

    Camera *camera = ComponentFactory::Instance()->CreateComponent<Camera>();
    cameraObj->AttachComponent(camera);
    SceneManager::Instance()->GetScene().SetCameraObject(cameraObj->GetHndl());

    Object *testObj1 = SF_CREATE_OBJECT("testObj1");
    testObj1->GetComponent<Transform>()->SetTranslation(Vector3(0.0, 7.0, 20.0));
    //testObj1->GetComponent<Transform>()->SetRotationEulerXZY(0.0, 45.0, 0.0);
    testObj1->GetComponent<Transform>()->Update();
    RigidBody *rb1 = SF_CREATE_COMP(RigidBody);
    rb1->SetDynamicState(Physics::RB_Static);
    testObj1->AttachComponent(rb1);
    MeshRenderer *mesh = SF_CREATE_COMP(MeshRenderer);
    mesh->SetMesh("Models\\cube.fbx");
    testObj1->AttachComponent(mesh);
    testObj1->AttachComponent(SF_CREATE_COMP(BoxCollider));
    SceneManager::Instance()->GetScene().AddObject(testObj1->GetHndl());
    SceneManager::Instance()->GetScene().m_sceneProperties.SetIbl(true);
    
    Object *testObj2 = SF_CREATE_OBJECT("testObj2");
    testObj2->GetComponent<Transform>()->SetTranslation(Vector3(0.0, -5.0, 20.0));
    testObj2->GetComponent<Transform>()->SetScale(Vector3(5.0, 1.0, 5.0));
    testObj2->GetComponent<Transform>()->Update();
    RigidBody *rb2 = SF_CREATE_COMP(RigidBody);
    rb2->SetDynamicState(Physics::RB_Static);
    testObj2->AttachComponent(rb2);
    MeshRenderer *mesh2 = SF_CREATE_COMP(MeshRenderer);
    mesh2->SetMesh("Models\\cube.fbx");
    testObj2->AttachComponent(mesh2);
    testObj2->AttachComponent(SF_CREATE_COMP(BoxCollider));
    SceneManager::Instance()->GetScene().AddObject(testObj2->GetHndl());

    Object *testObj3 = SF_CREATE_OBJECT("testObj3");
    testObj3->GetComponent<Transform>()->SetTranslation(Vector3(10.0, 0.0, 20.0));
    testObj3->GetComponent<Transform>()->Update();
    RigidBody *rb3 = SF_CREATE_COMP(RigidBody);
    rb3->SetDynamicState(Physics::RB_Static);
    testObj3->AttachComponent(rb3);
    MeshRenderer *mesh3 = SF_CREATE_COMP(MeshRenderer);
    mesh3->SetMesh("Models\\cube.fbx");
    testObj3->AttachComponent(mesh3);
    testObj3->AttachComponent(SF_CREATE_COMP(BoxCollider));
    SceneManager::Instance()->GetScene().AddObject(testObj3->GetHndl());

    Object *testObj4 = SF_CREATE_OBJECT("testObj4");
    testObj4->GetComponent<Transform>()->SetTranslation(Vector3(-10.0, -2.0, 20.0));
    testObj4->GetComponent<Transform>()->Update();
    RigidBody *rb4 = SF_CREATE_COMP(RigidBody);
    rb4->SetDynamicState(Physics::RB_Static);
    testObj4->AttachComponent(rb4);
    MeshRenderer *mesh4 = SF_CREATE_COMP(MeshRenderer);
    mesh4->SetMesh("Models\\cube.fbx");
    testObj4->AttachComponent(mesh4);
    testObj4->AttachComponent(SF_CREATE_COMP(BoxCollider));
    SceneManager::Instance()->GetScene().AddObject(testObj4->GetHndl());

    Object *spotLight = SF_CREATE_OBJECT("sl");
    spotLight->AttachComponent(SF_CREATE_COMP(SpotLight));
    spotLight->GetComponent<SpotLight>()->SetIntensity(100);
    spotLight->GetComponent<SpotLight>()->SetRange(200);
    spotLight->GetComponent<SpotLight>()->SetOuterAngle(100);
    spotLight->GetComponent<SpotLight>()->SetInnerAngle(100);
    SceneManager::Instance()->GetScene().AddObject(spotLight->GetHndl());

    /*Object *testObj = SF_CREATE_OBJECT("testEvent");
    EventManager::Instance()->PushEvent(IEntity::&OnTestEvent, OnTestEventData(testObj->GetHndl(), "Test"));*/
  }

  void Core::GameLoop(void)
  {

    while (m_running)
      Frame();
  }

  void Core::Frame(void)
  {
    WindowManager::Instance()->Update();
    InputManager::Instance()->Update();
    TaskManager::Instance()->RunTasks();
    GraphicsManager::Instance()->Update();
    ObjectFactory::Instance()->EndFrameCleanUp();
    Time::Instance()->WaitForFrameRate();
  }

  void Core::ShutDown(void)
  {
    
  }

  void Core::OnWindowClose()
  {
    m_running = false;
  }

}
