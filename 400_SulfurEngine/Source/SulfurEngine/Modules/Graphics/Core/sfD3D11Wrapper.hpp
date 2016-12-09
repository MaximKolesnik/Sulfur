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
      if (m_resource)
      {
        m_resource->Release();
        m_resource = nullptr;
      }
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
      if (m_resource)
      {
        m_resource->Release();
        m_resource = nullptr;
      }
    }

    RESOURCE_TYPE* GetD3DResource() const
    {
      return m_resource;
    }

  protected:
    RESOURCE_TYPE *m_resource;

  };

  // Resource array wrapper
  template <typename RESOURCE_TYPE, typename DESCRIPTION_TYPE = void>
  class D3D11ResourceArrayWrapper
  {

  public:
    typedef D3D11ResourceArrayWrapper<RESOURCE_TYPE, DESCRIPTION_TYPE> WrapperBase;

  public:
    D3D11ResourceArrayWrapper()
      : m_resource(nullptr), m_resourceCount(0)
    {
    }

    virtual void Init(RESOURCE_TYPE **resource, UINT32 count)
    {
      m_resource = resource;
      m_resourceCount = count;
      m_resource[0]->GetDesc(&m_description);
    }

    virtual void Free()
    {
      if (m_resourceCount > 0)
      {
        for (UINT32 i = 0; i < m_resourceCount; ++i)
          m_resource[i]->Release();

        delete[] m_resource;
        m_resource = nullptr;
        m_resourceCount = 0;
      }
    }

    RESOURCE_TYPE** GetD3DResource() const
    {
      return m_resource;
    }

    const DESCRIPTION_TYPE& GetDescription() const
    {
      return m_description;
    }

  protected:
    RESOURCE_TYPE **m_resource;
    UINT32 m_resourceCount;
    DESCRIPTION_TYPE m_description;

  };

  // Resource array wrapper without description
  template <typename RESOURCE_TYPE>
  class D3D11ResourceArrayWrapper<RESOURCE_TYPE, void>
  {

  public:
    typedef D3D11ResourceArrayWrapper<RESOURCE_TYPE> WrapperBase;

  public:
    D3D11ResourceArrayWrapper()
      : m_resource(nullptr), m_resourceCount(0)
    {
    }

    virtual void Init(RESOURCE_TYPE **resource, UINT32 count)
    {
      m_resource = resource;
      m_resourceCount = count;
    }

    virtual void Free()
    {
      if (m_resourceCount > 0)
      {
        for (UINT32 i = 0; i < m_resourceCount; ++i)
          m_resource[i]->Release();

        delete[] m_resource;
        m_resource = nullptr;
        m_resourceCount = 0;
      }
    }

    RESOURCE_TYPE** GetD3DResource() const
    {
      return m_resource;
    }

  protected:
    RESOURCE_TYPE **m_resource;
    UINT32 m_resourceCount;

  };
  
}