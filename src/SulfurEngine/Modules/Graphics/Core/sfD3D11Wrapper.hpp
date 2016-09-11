/******************************************************************************/
/*!
\par     Sulfur
\file    sfD3D11ResourceWrapper.cpp
\author  Dylan Norris
\par     DP email: d.norris@digipen.edu
\date    9/5/2016

\brief   Base Direct3D resource wrapper class

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

namespace Sulfur
{
	
  template <typename RESOURCE_TYPE, typename DESCRIPTION_TYPE = void>
  class D3D11ResourceWrapper
  {

  public:
    typedef D3D11ResourceWrapper<RESOURCE_TYPE, DESCRIPTION_TYPE> WrapperBase;
    
  public:
    D3D11ResourceWrapper()
      : m_resource(nullptr)
    {
    }

    virtual void Init(RESOURCE_TYPE *resource)
    {
      m_resource = resource;
      m_resource->GetDesc(&m_description);
    }

    virtual void Free()
    {
      if (m_resource) m_resource->Release();
    }

    RESOURCE_TYPE* GetD3DResource() const
    {
      return m_resource;
    }

    const DESCRIPTION_TYPE& GetDescription() const
    {
      return m_description;
    }

  protected:
    RESOURCE_TYPE *m_resource;
    DESCRIPTION_TYPE m_description;

  };

  // Resource without description specialization
  template <typename RESOURCE_TYPE>
  class D3D11ResourceWrapper<RESOURCE_TYPE, void>
  {

  public:
    typedef D3D11ResourceWrapper<RESOURCE_TYPE> WrapperBase;

  public:
    D3D11ResourceWrapper()
      : m_resource(nullptr)
    {
    }

    virtual void Init(RESOURCE_TYPE *resource)
    {
      m_resource = resource;
    }

    virtual void Free()
    {
      if (m_resource) m_resource->Release();
    }

    RESOURCE_TYPE* GetD3DResource() const
    {
      return m_resource;
    }

  protected:
    RESOURCE_TYPE *m_resource;

  };
  
}