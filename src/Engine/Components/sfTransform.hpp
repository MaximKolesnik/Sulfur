#pragma once

#include "../Types/sfIEntity.h"
#include "../Types/sfTypes.hpp"
#include "../Math/sfVector3.hpp"
#include "../Math/sfQuaternion.hpp"
#include "../Factories/sfComponentFactory.hpp"

namespace Sulfur
{
  class Transform : public IEntity
  {
  public:
    Transform(void);
    virtual ~Transform(void);

    virtual void Initialize(void) override;
    virtual void Update(Real dt) override;
    virtual Transform* Clone(void) const override;

  private:
    Vector3    m_scale;
    Vector3    m_translation;
    Quaternion m_rotation;
  };

  //REGISTER_COMPONENT(Transform)
}