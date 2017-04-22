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

namespace Sulfur
{
	
  template <typename ResourceType>
  class IResourceImporter
  {
  
  public:
    virtual ~IResourceImporter() {}
    virtual void Load(BYTE *buffer, UINT32 bufferSize, ResourceType *resource) = 0;

  };
  
}