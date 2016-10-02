/******************************************************************************/
/*!
\par     Sulfur
\file    sfClass.cpp
\author  
\par     DP email: 
\date    9/5/2016

\brief   

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/
#include "sfResourcePath.hpp"

namespace Sulfur
{

SF_REGISTER_BASE_TYPE(ResourcePath)

template <>
UINT32 Serialization::SerializedSize<ResourcePath>(const ResourcePath& value)
{
  return Serialization::SerializedSize<std::string>(value);
}

template <>
void Serialization::Serialize<ResourcePath>(std::ostream& str, const ResourcePath& value)
{
  Serialization::Serialize<std::string>(str, value);
}

template <>
void Serialization::Deserialize<ResourcePath>(std::istream& str, ResourcePath& value)
{
  Serialization::Deserialize<std::string>(str, value);
}

}
