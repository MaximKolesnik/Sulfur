/******************************************************************************/
/*!
\par     Sulfur
\file    sfClass.hpp
\author  
\par     DP email: 
\date    9/5/2016

\brief   

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Types/sfIEntity.hpp"
#include "Modules/Graphics/Scene/sfMesh.hpp"

namespace Sulfur
{
	
SF_REFLECTED_CLASS_DERIVED(MeshRenderer, IEntity)

public:
  virtual void Initialize(void) override final;
  virtual MeshRenderer* Clone(void) const override final;
  virtual void Update(void) override final {};

  Mesh* GetMesh();
  void SetMesh(Mesh* mesh);

private:
  Mesh *m_mesh;
  
};
  
}