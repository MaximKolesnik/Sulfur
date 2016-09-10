/******************************************************************************/
/*!
\par     Sulfur
\file    sfSerialization.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/7/2016

\brief   Serialization functions

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once

namespace Sulfur
{
	
  class Serialization
  {
  
  public:
    template <typename T>
    static void Serialize(std::ostream& str, const T& value);

    template <typename T>
    static void Deserialize(std::istream& str, T& value);

  };
  
}