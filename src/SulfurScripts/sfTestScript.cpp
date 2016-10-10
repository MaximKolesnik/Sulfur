#include <windows.h>

#include "sfTestScript.hpp"
#include "Factories\sfObjectFactory.hpp"
#include "Components\sfTransform.hpp"
#include "SystemTable\sfSystemTable.hpp"

SF_INIT_SCRIPT(TestScript)

using namespace Sulfur;

TestScript::~TestScript()
{

}

void TestScript::Initialize()
{
}


void TestScript::Update()
{
	HNDL obj = GetOwner();
	HNDL transHndl = g_SystemTable->ObjFactory->GetObject(obj)->GetComponentHandle<Transform>();
	Transform *t = g_SystemTable->CompFactory->GetComponent<Transform>(transHndl);
	t->SetTranslation(Vector3(0.0,0.0,4.0)); 
	t->SetRotationEulerXZY(0.0f, 45.0f, 0.0f);
}
