#include "sfGeometry.hpp"
#include "sfShapes.hpp"

namespace Sulfur
{
  namespace Geometry
  {
    Vector3 ProjectPointOnPlane(const Vector3& point, const Vector3& normal, Real planeDistance)
    {
      Real t = Dot(normal, point) - planeDistance;
      Vector3 projection = point - t * normal;

      return projection;
    }

    bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b,
      Real& u, Real& v, Real epsilon)
    {
      Vector3 line = b - a;

      if (line == Vector3::c_zero) //Degenerate line
      {
        u = Real(1.0); v = Real(0.0);
        return false;
      }

      v = Dot(point - a, line) / Dot(line, line);
      u = Real(1.0) - v;

      if (u >= -epsilon && u <= Real(1.0) + epsilon)
        return true;
      return false;
    }

    bool BarycentricCoordinates(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c,
      Real& u, Real& v, Real& w, Real epsilon)
    {
      if (a == b || b == c || a == c) //Degenerate
      {
        u = Real(1.0); v = Real(0.0); w = Real(0.0);
        return false;
      }

      Vector3 height = Cross(b - a, c - a).Normalized();
      Real abcArea = TriSignedArea(a, b, c, height);

      u = TriSignedArea(point, b, c, height) / abcArea;
      v = TriSignedArea(point, c, a, height) / abcArea;
      w = 1 - u - v;

      if (v >= Real(0.0) && w >= Real(0.0) && v + w <= Real(1.0))
        return true;

      return false;
    }

    Real TriSignedArea(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& triNormal)
    {
      return Dot(Cross(p2 - p1, p3 - p1), triNormal);
    }

    IntersectionType::Type PointPlane(const Vector3& point, const Vector4& plane, Real epsilon)
    {
      Real w = Dot(Vector4(point.GetX(), point.GetY(), point.GetZ(), -1), plane);

      if (w < -epsilon)
        return IntersectionType::Outside;
      else if (w > epsilon)
        return IntersectionType::Inside;

      return IntersectionType::Coplanar;
    }

    bool PointSphere(const Vector3& point, const Vector3& sphereCenter, Real sphereRadius)
    {
      Real distanceSq = (point - sphereCenter).LengthSq();

      if (distanceSq <= sphereRadius * sphereRadius)
        return true;
      return false;
    }

    bool PointAabb(const Vector3& point, const Vector3& aabbMin, const Vector3& aabbMax)
    {
      if (point.GetX() < aabbMin.GetX() || point.GetX() > aabbMax.GetX())
        return false;
      if (point.GetY() < aabbMin.GetY() || point.GetY() > aabbMax.GetY())
        return false;
      if (point.GetZ() < aabbMin.GetZ() || point.GetZ() > aabbMax.GetZ())
        return false;

      return true;
    }

    bool RayPlane(const Vector3& rayStart, const Vector3& rayDir,
      const Vector4& plane, Real& t, Real epsilon)
    {
      Vector3 normal = Vector3(plane.GetX(), plane.GetY(), plane.GetZ());

      Real denom = Dot(normal, rayDir);
      if (abs(denom) <= epsilon) // Check if parallel
      {
        t = Real(0.0);
        return false;
      }

      t = (plane.GetW() - Dot(normal, rayStart)) / denom;

      if (t >= Real(0.0))
        return true;

      return false;
    }

    bool RayTriangle(const Vector3& rayStart, const Vector3& rayDir,
      const Vector3& triP0, const Vector3& triP1, const Vector3& triP2,
      Real& t, Real triExpansionEpsilon)
    {
      Plane triPlane(triP0, triP1, triP2);

      if (RayPlane(rayStart, rayDir, triPlane.mData, t, triExpansionEpsilon))
      {
        Vector3 interPoint = rayStart + t * rayDir;
        Real u = 0.0, v = 0.0, w = 0.0;

        if (BarycentricCoordinates(interPoint, triP0, triP1, triP2, u, v, w, triExpansionEpsilon))
          return true;
      }

      return false;
    }

    bool RaySphere(const Vector3& rayStart, const Vector3& rayDir,
      const Vector3& sphereCenter, Real sphereRadius,
      Real& t)
    {
      Real a = Dot(rayDir, rayDir);
      Real b = 2 * (Dot(rayStart, rayDir) - Dot(sphereCenter, rayDir));
      Real c = Dot(sphereCenter, sphereCenter) - sphereRadius * sphereRadius
        + Dot(rayStart, rayStart) - 2 * Dot(sphereCenter, rayStart);

      t = Real(0.0);
      Real disc = b * b - 4 * a * c;
      if (disc < 0) //no roots
        return false;
      else if (disc == 0) //1 root
      {
        Real root = (-b + sqrtf(disc)) / (2 * a);
        if (root >= 0)
        {
          t = root;
          return true;
        }
      }
      else //2 roots
      {
        Real root1 = (-b - sqrtf(disc)) / (2 * a);
        Real root2 = (-b + sqrtf(disc)) / (2 * a);

        if (root1 < 0 && root2 >= 0) //inside
        {
          t = 0;
          return true;
        }
        else if (root1 >= 0 && root2 >= 0)
        {
          t = root1;
          return true;
        }
      }

      return false;
    }

    bool RayAabb(const Vector3& rayStart, const Vector3& rayDir,
      const Vector3& aabbMin, const Vector3& aabbMax, Real& t)
    {
      t = Real(0.0);
      Real tmax = SF_REAL_MAX;

      for (int i = 0; i < 3; ++i)
      {
        if (rayDir[i] == 0)
        {
          if (rayStart[i] < aabbMin[i] || rayStart[i] > aabbMax[i])
            return false;
        }
        else
        {
          Real denom = Real(1.0) / rayDir[i];
          Real t1 = (aabbMin[i] - rayStart[i]) * denom;
          Real t2 = (aabbMax[i] - rayStart[i]) * denom;

          if (t1 > t2)
          {
            Real temp = t1;
            t1 = t2;
            t2 = temp;
          }

          t = std::fmax(t, t1);
          tmax = std::fmin(tmax, t2);
          if (t > tmax)
            return false;
        }
      }

      return true;
    }

    IntersectionType::Type PlaneTriangle(const Vector4& plane,
      const Vector3& triP0, const Vector3& triP1, const Vector3& triP2,
      Real epsilon)
    {
      //For every intersection case, except of Overlaps, 
      //result will be | into enum. In overlaping case we should have at least
      //inside and outside, which will add up to 3
      int result = IntersectionType::Type::Coplanar;

      result |= (int)PointPlane(triP0, plane, epsilon);
      result |= (int)PointPlane(triP1, plane, epsilon);
      result |= (int)PointPlane(triP2, plane, epsilon);

      return (IntersectionType::Type)result;
    }

    IntersectionType::Type PlaneSphere(const Vector4& plane,
      const Vector3& sphereCenter, Real sphereRadius)
    {
      Real dist = Dot(sphereCenter, Vector3(plane.GetX(), plane.GetY(), plane.GetZ())) 
        - plane.GetW();

      if (dist < -sphereRadius)
        return IntersectionType::Type::Outside;
      else if (dist > sphereRadius)
        return IntersectionType::Type::Inside;

      return IntersectionType::Overlaps;
    }

    IntersectionType::Type PlaneAabb(const Vector4& plane,
      const Vector3& aabbMin, const Vector3& aabbMax)
    {
      Vector3 center = (aabbMax + aabbMin) * Real(0.5);
      Vector3 extent = aabbMax - center;

      Vector3 normal(plane.GetX(), plane.GetY(), plane.GetZ());
      Vector3 normalAbs(abs(plane.GetX()), abs(plane.GetY()), abs(plane.GetZ()));

      Real r = Dot(extent, normalAbs);
      Real dist = Dot(normal, center) - plane.GetW();

      if (dist < -r)
        return IntersectionType::Type::Outside;
      else if (dist > r)
        return IntersectionType::Type::Inside;

      return IntersectionType::Type::Overlaps;
    }

    IntersectionType::Type FrustumTriangle(const Vector4 planes[6],
      const Vector3& triP0, const Vector3& triP1, const Vector3& triP2,
      Real epsilon)
    {
      int result = 0;
      IntersectionType::Type test;

      for (int i = 0; i < 6; ++i)
      {
        if ((test = PlaneTriangle(planes[i], triP0, triP1, triP2, epsilon)) == IntersectionType::Type::Outside)
          return IntersectionType::Type::Outside;

        result |= test;
      }

      return (IntersectionType::Type)result;
    }

    IntersectionType::Type FrustumSphere(const Vector4 planes[6],
      const Vector3& sphereCenter, Real sphereRadius, size_t& lastAxis)
    {
      int result = 0;
      IntersectionType::Type test;

      for (int i = 0; i < 6; ++i)
      {
        if ((test = PlaneSphere(planes[i], sphereCenter, sphereRadius)) == IntersectionType::Type::Outside)
          return IntersectionType::Type::Outside;

        result |= test;
      }

      return (IntersectionType::Type)result;
    }

    IntersectionType::Type FrustumAabb(const Vector4 planes[6],
      const Vector3& aabbMin, const Vector3& aabbMax, size_t& lastAxis)
    {
      int result = 0;
      IntersectionType::Type test;

      for (size_t i = lastAxis; i < 6; ++i)
      {
        if ((test = PlaneAabb(planes[i], aabbMin, aabbMax)) == IntersectionType::Type::Outside)
        {
          lastAxis = i;
          return IntersectionType::Type::Outside;
        }

        result |= test;
      }

      for (size_t i = 0; i < lastAxis; ++i)
      {
        if ((test = PlaneAabb(planes[i], aabbMin, aabbMax)) == IntersectionType::Type::Outside)
        {
          lastAxis = i;
          return IntersectionType::Type::Outside;
        }

        result |= test;
      }

      return (IntersectionType::Type)result;
    }

    bool SphereSphere(const Vector3& sphereCenter0, Real sphereRadius0,
      const Vector3& sphereCenter1, Real sphereRadius1)
    {
      Real distSq = (sphereCenter1 - sphereCenter0).LengthSq();
      Real rad = (sphereRadius0 + sphereRadius1);

      if (distSq <= rad * rad)
        return true;

      return false;
    }

    bool AabbAabb(const Vector3& aabbMin0, const Vector3& aabbMax0,
      const Vector3& aabbMin1, const Vector3& aabbMax1)
    {
      for (int i = 0; i < 3; ++i)
      {
        if (aabbMin1[i] > aabbMax0[i] || aabbMin0[i] > aabbMax1[i])
          return false;
      }

      return true;
    }
  }
}