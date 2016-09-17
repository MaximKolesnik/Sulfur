/******************************************************************************/
/*!
\par     Sulfur
\file    sfCamera.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/16/2016

\brief   Camera component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfIEntity.hpp"
#include "Math/sfMatrix4.hpp"

namespace Sulfur
{
	
enum ProjectionType
{
  ORTHOGRAPHIC,
  PERSPECTIVE
};

SF_REGISTER_ENUM_TYPE(ProjectionType, "Orthographic", "Perspective")
BASE_TYPE_SPECIALIZATION(ProjectionType)


SF_REFLECTED_CLASS_DERIVED(Camera, IEntity)
  
public:
  virtual void Initialize(void) override final;
  virtual Camera* Clone(void) const override final;
    
private:
  Matrix4 m_viewMatrix;
  Matrix4 m_projectionMatrix;

  SF_PRIVATE_PROPERTY(ProjectionType, projectionType, ProjectionType, "Projection type")
  SF_PRIVATE_PROPERTY(Real, fieldOfView, FieldOfView, "Field of view")
  SF_PRIVATE_PROPERTY(Real, nearPlane, NearPlane, "Near plane")
  SF_PRIVATE_PROPERTY(Real, farPlane, FarPlane, "Far plane")

};
  
}