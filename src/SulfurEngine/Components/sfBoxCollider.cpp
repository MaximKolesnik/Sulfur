#include "sfBoxCollider.hpp"

namespace Sulfur
{
  BoxCollider::BoxCollider(void)
  {

  }

  BoxCollider::~BoxCollider(void)
  {

  }

  void BoxCollider::Initialize(void)
  {

  }

  BoxCollider* BoxCollider::Clone(void) const
  {
    SF_CRITICAL_ERR("BoxCollider clone not implemented");
    return nullptr;
  }

  void BoxCollider::Update(void)
  {

  }

  void BoxCollider::DrawDebug(DebugDraw *draw) const
  {

  }
}