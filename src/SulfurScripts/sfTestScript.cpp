#include <windows.h>

#include "sfTestScript.hpp"
SF_INCLUDE_SCRIPT(sfTestScript)
#include "Factories\sfObjectFactory.hpp"
#include "Components\sfTransform.hpp"

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
	HNDL transHndl = Engine->ObjFactory->GetObject(obj)->GetComponentHandle<Transform>();
	Transform *t = Engine->CompFactory->GetComponent<Transform>(transHndl);
	t->SetTranslation(Vector3(0.0,0.0,6.0)); 
	t->SetRotationEulerXZY(0.0f, 45.0f, 0.0f);   
}
