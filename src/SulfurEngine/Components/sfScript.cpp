/******************************************************************************/
/*!
\par     Sulfur
\file    sfScript.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    10/14/2016

\brief   Script component

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfScript.hpp"
#include "Factories/sfComponentFactory.hpp"
#include <Python.h>

namespace Sulfur
{

Script::Script()
{

}

void Script::Initialize(void)
{

}

Script* Script::Clone(void) const
{
  Script *newScript = SF_CREATE_COMP(Script);
  return newScript;
}

void Script::Update(void)
{

}

}
