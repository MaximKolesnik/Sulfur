/******************************************************************************/
/*!
\par     Sulfur
\file    sfWindowDescription.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Window description

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include <string>

namespace Sulfur
{
	
struct WindowDescription
{
  UINT32 Width;
  UINT32 Height;
  std::string Title;
};
  
}