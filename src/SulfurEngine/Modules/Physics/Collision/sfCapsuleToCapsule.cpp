#include "sfSphereToBox.hpp"
#include "Modules\Physics\Data\sfColliderData.hpp"
#include "Components\sfTransform.hpp"
#include "Math\sfVector3.hpp"

namespace Sulfur
{
  namespace Physics
  {
    static void FindClosestPoints(const Vector3 &p1, const Vector3 &p2,
      const Vector3 &q1, const Vector3 &q2, Vector3 &closestP, Vector3 &closestQ)
    {
      Vector3 dp = p2 - p1;
      Vector3 dq = q2 - q1;
      Vector3 r = p1 - q1;
      Real a = dp.LengthSq();
      Real e = dq.LengthSq();
      Real f = dq.Dot(r);

      if (a <= SF_EPSILON && e <= SF_EPSILON)
      {
        closestP = p1;
        closestQ = q1;
      }
      else
      {
        Real pt, qt;
        if (a <= SF_EPSILON)
        {
          pt = Real(0.0);
          qt = f / e;
          qt = MathUtils::Clamp(qt, Real(0.0), Real(1.0));
        }
        else
        {
          Real c = dp.Dot(r);

          if (e <= SF_EPSILON)
          {
            qt = Real(0.0);
            pt = MathUtils::Clamp(-c / a, Real(0.0), Real(1.0));
          }
          else
          {
            Real b = dp.Dot(dq);
            Real den = a * e - b * b;

            if (den != Real(0.0))
              pt = MathUtils::Clamp((b * f - c * e) / den, Real(0.0), Real(1.0));
            else
            {
              if (f < 0)
                pt = (q1 - p1).Dot(dp) / a;
              else
                pt = (q2 - p1).Dot(dp) / a;

              pt = (pt + Real(1.0)) / 2;
            }

            qt = (b * pt + f) / e;

            if (qt < Real(0.0))
            {
              qt = Real(0.0);
              pt = MathUtils::Clamp(-c / a, Real(0.0), Real(1.0));
            }
            else if (qt > Real(1.0))
            {
              qt = Real(1.0);
              pt = MathUtils::Clamp((b - c) / a, Real(0.0), Real(1.0));
            }
          }
        }

        closestP = p1 + dp * pt;
        closestQ = q1 + dq * qt;
      }
    }

    void CapsuleToCapsule(Contacts &contacts, ColliderData *colliderA,
      ColliderData *colliderB)
    {
      Transform *transA = SF_GET_COMP_TYPE(Transform, colliderA->m_transformHndl);
      Transform *transB = SF_GET_COMP_TYPE(Transform, colliderB->m_transformHndl);

      Vector3 posA = transA->GetTranslation() + colliderA->m_offset;
      Vector3 posB = transB->GetTranslation() + colliderB->m_offset;

      Real lineLengthA = transA->GetScale()[0] * colliderA->m_lineLength;
      Real lineLengthB = transB->GetScale()[0] * colliderB->m_lineLength;
      Real radiusA = std::max(transA->GetScale()[1], transA->GetScale()[2])
        * colliderA->m_lineLength * colliderA->m_radius;
      Real radiusB = std::max(transB->GetScale()[1], transB->GetScale()[2])
        * colliderB->m_lineLength* colliderB->m_radius;

      Vector3 cLineAP1 = Vector3(-lineLengthA / 2, Real(0.0), Real(0.0));
      Vector3 cLineAP2 = Vector3(lineLengthA / 2, Real(0.0), Real(0.0));
      Vector3 cLineBP1 = Vector3(-lineLengthB / 2, Real(0.0), Real(0.0));
      Vector3 cLineBP2 = Vector3(lineLengthB / 2, Real(0.0), Real(0.0));

      transA->GetRotation().Rotate(cLineAP1);
      transA->GetRotation().Rotate(cLineAP2);
      transB->GetRotation().Rotate(cLineBP1);
      transB->GetRotation().Rotate(cLineBP2);

      cLineAP1 += posA;
      cLineAP2 += posA;
      cLineBP1 += posB;
      cLineBP2 += posB;

      //Find closests points;
      Vector3 closestA;
      Vector3 closestB;

      FindClosestPoints(cLineAP1, cLineAP2, cLineBP1, cLineBP2, closestA, closestB);

      Real dist = (closestB - closestA).Length();
      if (dist > colliderA->m_radius + colliderB->m_radius)
        return;

      Contact c;

      c.m_colliderA = colliderA;
      c.m_colliderB = colliderB;
      c.m_contactNormal = (closestB - closestA).Normalized();
      c.m_contactPoint = closestA + (closestB - closestA) * Real(0.5);
      c.m_penetration = colliderA->m_radius + colliderB->m_radius - dist;

      contacts.push_back(c);
    }
  }
}