/******************************************************************************/
/*!
\par     Sulfur
\file    sfContact.hpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/10/2016

\brief

All content � 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#pragma once

#include "Math\sfVector3.hpp"
namespace Sulfur
{
  namespace Physics
  {
    struct ColliderData;

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
      Vector3 m_contactPoint;
      Vector3 m_tangent1;
      Vector3 m_tangent2;

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