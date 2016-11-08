#pragma once

#include "Math\sfVector3.hpp"
namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

    struct ContactPoint
    {
      Vector3 m_point;

      union
      {
        struct
        {
          INT32 m_incidentFeature;
          INT32 m_witnessFeature;
        } m_features;
        UINT64 m_value;
      };
    };

    struct ContactKey
    {
      ContactKey(ColliderData *cA, ColliderData *cB)
      {
        if (cA < cB)
        {
          m_a = cA;
          m_b = cB;
        }
        else
        {
          m_a = cB;
          m_b = cA;
        }
      }

      ColliderData *m_a;
      ColliderData *m_b;
    };

    struct Contact
    {
      ColliderData *m_colliderA;
      ColliderData *m_colliderB;

      Vector3 m_contactNormal;
      ContactPoint m_contactPoint;
      Vector3 m_tangent;

      Real m_penetration;

      Real m_massNormal;
      Real m_massTangent1;
      Real m_massTangent2;

      Vector3 Pn = Vector3::c_zero, Pt = Vector3::c_zero;

      Real m_bias;
    };

    typedef std::vector<Contact> Contacts;
  }
}