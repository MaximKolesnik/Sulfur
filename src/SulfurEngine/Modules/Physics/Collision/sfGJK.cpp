/******************************************************************************/
/*!
\par     Sulfur
\file    sfGJK.cpp
\author  Maxim Kolesnik
\par     DP email: maxim.kolesnik@digipen.edu
\date    11/25/2016

\brief

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
*/
/******************************************************************************/

#include "sfGJK.hpp"

#include "Math\Geometry\sfGeometry.hpp"

namespace Sulfur
{
  namespace Physics
  {
    
    Vector3 SupportShape::GetCenter(const std::vector<Vector3>& points, 
      const Matrix4& transform) const
    {
      Vector3 center = Vector3::c_zero;

      if (!points.size())
        return center;

      for (auto &p : points)
        center += p;
      return transform * (center / (Real)points.size());
    }

    Vector3 SupportShape::Support(const Vector3& direction, const std::vector<Vector3>& points, 
      const Matrix4& transform) const
    {
      if (!points.size())
        return Vector3::c_zero;

      Vector3 localDir = transform.Inverted().TransformNormal(direction);
      Real furthestDistance = localDir.Dot(points[0]);
      const Vector3 *result = &points[0];
      size_t numPoints = points.size();

      Real tempDot;
      for (size_t i = 1; i < numPoints; ++i)
      {
        tempDot = localDir.Dot(points[i]);
        if (tempDot > furthestDistance)
        {
          furthestDistance = tempDot;
          result = &points[i];
        }
      }

      return transform * *result;
    }

    PointsSupportShape::PointsSupportShape()
    {
      m_scale = Vector3(Real(1.0), Real(1.0), Real(1.0));
      m_rotation.SetEuler(Real(0.0), Real(0.0), Real(0.0));
      m_translation = Vector3::c_zero;
    }

    Vector3 PointsSupportShape::GetCenter() const
    {
      Matrix4 transform; 
      transform.SetTransformation(m_rotation, m_scale, m_translation);
      return SupportShape::GetCenter(m_localSpacePoints, transform);
    }

    Vector3 PointsSupportShape::Support(const Vector3& direction) const
    {
      Matrix4 transform; 
      transform.SetTransformation(m_rotation, m_scale, m_translation);
      return SupportShape::Support(direction, m_localSpacePoints, transform);
    }

    Vector3 SphereSupportShape::GetCenter() const
    {
      return m_sphere.m_center;
    }

    Vector3 SphereSupportShape::Support(const Vector3& direction) const
    {
      return direction.Normalized() * m_sphere.m_radius + m_sphere.m_center;
    }

    Vector3 ObbSupportShape::GetCenter() const
    {
      return m_translation;
    }

    Vector3 ObbSupportShape::Support(const Vector3& direction) const
    {
      Vector3 localDir = m_rotation.Inverted().Rotated(direction);
      Vector3 result = m_translation;

      Matrix3 rotMat = m_rotation.GetMatrix3();
      for (int i = 0; i < 3; ++i)
        result += MathUtils::GetSign(localDir[i]) * (m_scale[i] / 2) * rotMat.GetColumn(i);

      return result;
    }

    VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
      size_t& newSize, int newIndices[4])
    {
      return VoronoiRegion::Point0;
    }

    Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0)
    {
      return p0;
    }

    Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0)
    {
      return q - p0;
    }

    // Edge Tests
    VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, const Vector3& p1,
      size_t& newSize, int newIndices[4])
    {
      Vector3 n = p1 - p0;
      Real v = Dot(q - p0, n) / Dot(n, n);
      Real u = 1 - v;

      if (u > 0 && v > 0)
      {
        newSize = 2;
        return VoronoiRegion::Edge01;
      }
      else if (u <= 0)
      {
        newSize = 1;
        newIndices[0] = 1;
        return VoronoiRegion::Point1;
      }
      else
      {
        newSize = 1;
        return VoronoiRegion::Point0;
      }

      return VoronoiRegion::Unknown;
    }

    Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1)
    {
      Real u, v;
      Geometry::BarycentricCoordinates(q, p0, p1, u, v);

      if (v <= 0)
        return p0;
      else if (u <= 0)
        return p1;

      return u * p0 + v * p1;
    }

    Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0, 
      const Vector3& p1)
    {
      return q - FindClosestPoint(q, p0, p1);
    }

    // Triangle Tests
    VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, 
      const Vector3& p1, const Vector3& p2,
      size_t& newSize, int newIndices[4])
    {
      Real v1, u1, v2, u2, v3, u3;

      Geometry::BarycentricCoordinates(q, p0, p1, u1, v1);
      Geometry::BarycentricCoordinates(q, p2, p0, u2, v2);
      if (v1 <= 0 && u2 <= 0)
        return VoronoiRegion::Point0;

      Geometry::BarycentricCoordinates(q, p1, p2, u3, v3);
      if (v2 <= 0 && u3 <= 0)
      {
        newIndices[0] = 2;
        return VoronoiRegion::Point2;
      }
      if (v3 <= 0 && u1 <= 0)
      {
        newIndices[0] = 1;
        return VoronoiRegion::Point1;
      }

      Real tu, tv, tw;
      Geometry::BarycentricCoordinates(q, p0, p1, p2, tu, tv, tw);

      if (tu > 0 && tv > 0 && tw > 0)
      {
        newSize = 3;
        return VoronoiRegion::Triangle012;
      }
      else if (tw <= 0 && u1 > 0 && v1 > 0)
      {
        newSize = 2;
        return VoronoiRegion::Edge01;
      }
      else if (tv <= 0 && u2 > 0 && v2 > 0)
      {
        newSize = 2;
        newIndices[1] = 2;
        return VoronoiRegion::Edge02;
      }
      else
      {
        newSize = 2;
        newIndices[0] = 1; newIndices[1] = 2;
        return VoronoiRegion::Edge12;
      }
    }

    Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, 
      const Vector3& p2)
    {
      Real v1, u1, v2, u2, v3, u3;

      Geometry::BarycentricCoordinates(q, p0, p1, u1, v1);
      Geometry::BarycentricCoordinates(q, p2, p0, u2, v2);

      if (v1 <= 0 && u2 <= 0)
        return p0;

      Geometry::BarycentricCoordinates(q, p1, p2, u3, v3);
      if (v2 <= 0 && u3 <= 0)
        return p2;
      if (v3 <= 0 && u1 <= 0)
        return p1;

      Real tu, tv, tw;
      Geometry::BarycentricCoordinates(q, p0, p1, p2, tu, tv, tw);

      if (tu > 0 && tv > 0 && tw > 0)
        return tu * p0 + tv * p1 + tw * p2;
      else if (tw <= 0 && u1 > 0 && v1 > 0)
        return FindClosestPoint(q, p0, p1);
      else if (tv <= 0 && u2 > 0 && v2 > 0)
        return FindClosestPoint(q, p0, p2);
      else
        return FindClosestPoint(q, p1, p2);
    }

    Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0, 
      const Vector3& p1, const Vector3& p2)
    {
      return q - FindClosestPoint(q, p0, p1, p2);
    }

    // Tetrahedron Tests
    VoronoiRegion::Type Gjk::IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, 
      const Vector3& p1, const Vector3& p2, const Vector3& p3,
      size_t& newSize, int newIndices[4])
    {
      Real u01, v01, u02, v02, u12, v12,
        u03, v03, u13, v13, u23, v23;

      Geometry::BarycentricCoordinates(q, p0, p1, u01, v01);
      Geometry::BarycentricCoordinates(q, p0, p2, u02, v02);
      Geometry::BarycentricCoordinates(q, p1, p2, u12, v12);
      Geometry::BarycentricCoordinates(q, p0, p3, u03, v03);
      Geometry::BarycentricCoordinates(q, p1, p3, u13, v13);
      Geometry::BarycentricCoordinates(q, p2, p3, u23, v23);

      if (v01 < 0 && v02 < 0 && v03 < 0)
        return VoronoiRegion::Point0;
      if (u01 < 0 && v12 < 0 && v13 < 0)
      {
        newIndices[0] = 1;
        return VoronoiRegion::Point1;
      }
      if (u12 < 0 && u02 < 0 && v23 < 0)
      {
        newIndices[0] = 2;
        return VoronoiRegion::Point2;
      }
      if (u03 < 0 && u13 < 0 && u23 < 0)
      {
        newIndices[0] = 3;
        return VoronoiRegion::Point3;
      }

      Real tu012, tv012, tw012,
        tu013, tv013, tw013,
        tu123, tv123, tw123,
        tu023, tv023, tw023;

      Geometry::BarycentricCoordinates(q, p0, p1, p2, tu012, tv012, tw012);
      Geometry::BarycentricCoordinates(q, p0, p1, p3, tu013, tv013, tw013);
      Geometry::BarycentricCoordinates(q, p1, p2, p3, tu123, tv123, tw123);
      Geometry::BarycentricCoordinates(q, p0, p2, p3, tu023, tv023, tw023);

      if (u01 > 0 && v01 > 0 && tw013 < 0 && tw012 < 0)
      {
        newSize = 2;
        return VoronoiRegion::Edge01;
      }
      if (u12 > 0 && v12 > 0 && tu012 < 0 && tw123 < 0)
      {
        newSize = 2;
        newIndices[0] = 1; newIndices[1] = 2;
        return VoronoiRegion::Edge12;
      }
      if (u02 > 0 && v02 > 0 && tv012 < 0 && tw023 < 0)
      {
        newSize = 2;
        newIndices[1] = 2;
        return VoronoiRegion::Edge02;
      }
      if (u03 > 0 && v03 > 0 && tv023 < 0 && tv013 < 0)
      {
        newSize = 2;
        newIndices[1] = 3;
        return VoronoiRegion::Edge03;
      }
      if (u13 > 0 && v13 > 0 && tu013 < 0 && tv123 < 0)
      {
        newSize = 2;
        newIndices[0] = 1; newIndices[1] = 3;
        return VoronoiRegion::Edge13;
      }
      if (u23 > 0 && v23 > 0 && tu123 < 0 && tu023 < 0)
      {
        newSize = 2;
        newIndices[0] = 2; newIndices[1] = 3;
        return VoronoiRegion::Edge23;
      }

      VoronoiRegion::Type result = TetrahedronTestTriangle(q, p0, p1, p2, p3);
      if (result == VoronoiRegion::Triangle012)
      {
        newSize = 3;
        return result;
      }
      if (result == VoronoiRegion::Triangle013)
      {
        newSize = 3;
        newIndices[2] = 3;
        return result;
      }
      if (result == VoronoiRegion::Triangle023)
      {
        newSize = 3;
        newIndices[1] = 2; newIndices[2] = 3;
        return result;
      }
      if (result == VoronoiRegion::Triangle123)
      {
        newSize = 3;
        newIndices[0] = 1; newIndices[1] = 2; newIndices[2] = 3;
        return result;
      }

      newSize = 4;
      return VoronoiRegion::Tetrahedra0123;
    }

    Vector3 Gjk::FindClosestPoint(const Vector3& q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3)
    {
      Real u01, v01, u02, v02, u12, v12,
        u03, v03, u13, v13, u23, v23;

      Geometry::BarycentricCoordinates(q, p0, p1, u01, v01);
      Geometry::BarycentricCoordinates(q, p0, p2, u02, v02);
      Geometry::BarycentricCoordinates(q, p1, p2, u12, v12);
      Geometry::BarycentricCoordinates(q, p0, p3, u03, v03);
      Geometry::BarycentricCoordinates(q, p1, p3, u13, v13);
      Geometry::BarycentricCoordinates(q, p2, p3, u23, v23);

      if (v01 < 0 && v02 < 0 && v03 < 0)
        return p0;
      if (u01 < 0 && v12 < 0 && v13 < 0)
        return p1;
      if (u12 < 0 && u02 < 0 && v23 < 0)
        return p2;
      if (u03 < 0 && u13 < 0 && u23 < 0)
        return p3;

      Real tu012, tv012, tw012,
        tu013, tv013, tw013,
        tu123, tv123, tw123,
        tu023, tv023, tw023;

      Geometry::BarycentricCoordinates(q, p0, p1, p2, tu012, tv012, tw012);
      Geometry::BarycentricCoordinates(q, p0, p1, p3, tu013, tv013, tw013);
      Geometry::BarycentricCoordinates(q, p1, p2, p3, tu123, tv123, tw123);
      Geometry::BarycentricCoordinates(q, p0, p2, p3, tu023, tv023, tw023);

      if (u01 > 0 && v01 > 0 && tw013 < 0 && tw012 < 0)
        return FindClosestPoint(q, p0, p1);
      if (u12 > 0 && v12 > 0 && tu012 < 0 && tw123 < 0)
        return FindClosestPoint(q, p1, p2);
      if (u02 > 0 && v02 > 0 && tv012 < 0 && tw023 < 0)
        return FindClosestPoint(q, p0, p2);
      if (u03 > 0 && v03 > 0 && tv023 < 0 && tv013 < 0)
        return FindClosestPoint(q, p0, p3);
      if (u13 > 0 && v13 > 0 && tu013 < 0 && tv123 < 0)
        return FindClosestPoint(q, p1, p3);
      if (u23 > 0 && v23 > 0 && tu123 < 0 && tu023 < 0)
        return FindClosestPoint(q, p2, p3);

      VoronoiRegion::Type result = TetrahedronTestTriangle(q, p0, p1, p2, p3);
      if (result == VoronoiRegion::Triangle012)
        return FindClosestPoint(q, p0, p1, p2);
      if (result == VoronoiRegion::Triangle013)
        return FindClosestPoint(q, p0, p1, p3);
      if (result == VoronoiRegion::Triangle023)
        return FindClosestPoint(q, p0, p2, p3);
      if (result == VoronoiRegion::Triangle123)
        return FindClosestPoint(q, p1, p2, p3);

      Real u, v, w, t;
      TetrahedronBarcentric(q, p0, p1, p2, p3, u, v, w, t);

      return u * p0 + v * p1 + w * p2 + t * p3;
    }

    Vector3 Gjk::ComputeSearchDirection(const Vector3& q, const Vector3& p0, 
      const Vector3& p1, const Vector3& p2, const Vector3& p3)
    {
      return q - FindClosestPoint(q, p0, p1, p2, p3);
    }

    bool Gjk::Intersect(const SupportShape* shapeA, const SupportShape* shapeB, 
      size_t maxIterations, CsoPoint& closestPoint, Real epsilon, std::vector<CsoPoint> &simplex)
    {
      Vector3 origin = Vector3::c_zero;
      Vector3 searchDir = shapeA->GetCenter() - shapeB->GetCenter();
      if (searchDir == Vector3::c_zero)
        searchDir.Set(-1, 0, 0);

      simplex.clear();
      simplex.push_back(ComputeSupport(shapeA, shapeB, searchDir));

      for (size_t i = 0; i < maxIterations; ++i)
      {
        ReduceSimplex(origin, simplex);

        closestPoint = FindClosestPoint(origin, simplex);
        if (closestPoint.m_csoPoint.LengthSq() < epsilon)
          return true;

        searchDir = -closestPoint.m_csoPoint;

        CsoPoint supPoint = ComputeSupport(shapeA, shapeB, searchDir);

        if (supPoint.m_csoPoint.Dot(searchDir) -
          closestPoint.m_csoPoint.Dot(searchDir) < epsilon)
          return false;

        simplex.push_back(supPoint);
      }

      return false;
    }

    Gjk::CsoPoint Gjk::ComputeSupport(const SupportShape* shapeA, const SupportShape* shapeB,
      const Vector3& direction)
    {
      CsoPoint result;

      result.m_pointA = shapeA->Support(direction);
      result.m_pointB = shapeB->Support(-direction);
      result.m_csoPoint = result.m_pointA - result.m_pointB;

      return result;
    }

    VoronoiRegion::Type Gjk::TetrahedronTestTriangle(const Vector3 &q, const Vector3& p0,
      const Vector3& p1, const Vector3& p2, const Vector3& p3)
    {
      VoronoiRegion::Type result = VoronoiRegion::Unknown;
      Real bestDot = SF_REAL_MAX;

      Vector3 normal = Cross(p1 - p0, p3 - p0);
      Real dot = Dot(q - p0, normal) * Dot(p2 - p0, normal);

      if (dot < 0.0f && dot < bestDot)
      {
        bestDot = dot;
        result = VoronoiRegion::Triangle013;
      }

      normal = Cross(p2 - p0, p3 - p0);
      dot = Dot(q - p0, normal) * Dot(p1 - p0, normal);

      if (dot < 0.0f && dot < bestDot)
      {
        bestDot = dot;
        result = VoronoiRegion::Triangle023;
      }

      normal = Cross(p2 - p1, p3 - p1);
      dot = Dot(q - p1, normal) * Dot(p0 - p1, normal);

      if (dot < 0.0f && dot < bestDot)
      {
        bestDot = dot;
        result = VoronoiRegion::Triangle123;
      }

      normal = Cross(p1 - p0, p2 - p0);
      dot = Dot(q - p0, normal) * Dot(p3 - p0, normal);

      if (dot < 0.0f && dot < bestDot)
        return VoronoiRegion::Triangle012;

      return result;
    }

    void Gjk::TetrahedronBarcentric(const Vector3 &q, const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
      Real &u, Real &v, Real &w, Real &t)
    {
      Matrix3 mat;
      mat.SetColumn(0, p0 - p3);
      mat.SetColumn(1, p1 - p3);
      mat.SetColumn(2, p2 - p3);

      Vector3 result = mat.Inverted() * (q - p3);
      u = result[0];
      v = result[1];
      w = result[2];
      t = 1 - u - v - w;
    }

    Gjk::CsoPoint Gjk::FindClosestPoint(const Vector3 &q, const std::vector<CsoPoint> &simplex)
    {
      CsoPoint result;
      if (simplex.size() == 1)
      {
        result.m_csoPoint = FindClosestPoint(q, simplex[0].m_csoPoint);
        result.m_pointA = simplex[0].m_pointA;
        result.m_pointB = simplex[0].m_pointB;
      }
      else if (simplex.size() == 2)
      {
        Real u, v;

        result.m_csoPoint = FindClosestPoint(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint);

        Geometry::BarycentricCoordinates(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint, u, v);
        result.m_pointA = u * simplex[0].m_pointA + v * simplex[1].m_pointA;
        result.m_pointB = u * simplex[0].m_pointB + v * simplex[1].m_pointB;
      }
      else if (simplex.size() == 3)
      {
        Real u, v, w;

        result.m_csoPoint = FindClosestPoint(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint, 
          simplex[2].m_csoPoint);

        Geometry::BarycentricCoordinates(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint,
          simplex[2].m_csoPoint, u, v, w);

        result.m_pointA = u * simplex[0].m_pointA + v * simplex[1].m_pointA + w * simplex[2].m_pointA;
        result.m_pointB = u * simplex[0].m_pointB + v * simplex[1].m_pointB + w * simplex[2].m_pointB;
      }
      else if (simplex.size() == 4)
      {
        Real u, v, w, t;

        result.m_csoPoint = FindClosestPoint(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint, 
          simplex[2].m_csoPoint, simplex[3].m_csoPoint);

        TetrahedronBarcentric(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint,
          simplex[2].m_csoPoint, simplex[3].m_csoPoint, u, v, w, t);

        result.m_pointA = u * simplex[0].m_pointA + v * simplex[1].m_pointA + w * 
          simplex[2].m_pointA + t * simplex[3].m_pointA;
        result.m_pointB = u * simplex[0].m_pointB + v * simplex[1].m_pointB + w * 
          simplex[2].m_pointB + t * simplex[3].m_pointB;
      }

      return result;
    }

    void Gjk::ReduceSimplex(const Vector3 &q, std::vector<CsoPoint> &simplex)
    {
      size_t newSize = 1;
      int newIndices[4] = { 0, 1, 2, 3 };

      if (simplex.size() == 1)
        IdentifyVoronoiRegion(q, simplex[0].m_csoPoint, newSize, newIndices);
      else if (simplex.size() == 2)
        IdentifyVoronoiRegion(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint, newSize, newIndices);
      else if (simplex.size() == 3)
        IdentifyVoronoiRegion(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint, simplex[2].m_csoPoint, newSize, newIndices);
      else if (simplex.size() == 4)
        IdentifyVoronoiRegion(q, simplex[0].m_csoPoint, simplex[1].m_csoPoint, simplex[2].m_csoPoint, simplex[3].m_csoPoint, newSize, newIndices);

      std::vector<CsoPoint> newSimplex;
      for (int i = 0; i < newSize; ++i)
        newSimplex.push_back(simplex[newIndices[i]]);

      simplex = newSimplex;
    }
  }
}