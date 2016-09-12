/******************************************************************************/
/*!
\par     Sulfur
\file    sfTransform.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    8/23/2016

\brief   Transform component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

/*******************************************************************************
Maxim Kolesnik: TODO: Write Clone() as soon as object and component factory
                      are done.
*******************************************************************************/

#include "sfTransform.hpp"
#include "../Error/sfError.hpp"

namespace Sulfur
{

  Transform::Transform(void) : IEntity()
  {
  }

  Transform::~Transform(void)
  {

  }

  void Transform::Initialize(void)
  {

  }

  Transform* Transform::Clone(void) const
  {
    SF_CRITICAL_ERR("Transform::Clone() is not implemented");
    return nullptr;
  }
}