/******************************************************************************/
/*!
\par     Sulfur
\file    sfResourcePath.hpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/28/2016

\brief   String wrapper for resource path, to distinguish in the editor.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#pragma once
#include "Reflection/sfSerialization.hpp"
#include <string>

namespace Sulfur
{

  class ResourcePath : public std::string
  {

  public:
    ResourcePath()
      : std::string()
    {
    }

    ResourcePath(const std::string& copy)
      : std::string(copy)
    {
    }

    ResourcePath(const char *copy)
      : std::string(copy)
    {
    }

    ResourcePath& operator=(const std::string& rhs)
    {
      std::string::operator=(rhs);
      return *this;
    }

    ResourcePath& operator=(const char *rhs)
    {
      std::string::operator=(rhs);
      return *this;
    }

  };
  
}