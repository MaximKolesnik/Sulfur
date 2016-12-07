#pragma once

#include "Math\sfVector3.hpp"
#include "Math\sfVector4.hpp"
#include "Math\sfMatrix3.hpp"
#include "Math\sfMatrix4.hpp"
#include "Math\sfQuaternion.hpp"

#include "Math\Geometry\sfShapes.hpp"

namespace Sulfur
{
  namespace Physics
  {
    class Model;

    class SupportShape
    {
    public:

      virtual Vector3 GetCenter() const = 0;
      virtual Vector3 Support(const Vector3& direction) const = 0;

      Vector3 GetCenter(const std::vector<Vector3>& points, 
        const Matrix4& transform) const;
      Vector3 Support(const Vector3& direction, const std::vector<Vector3>& points, 
        const Matrix4& transform) const;
    };

    class PointsSupportShape : public SupportShape
    {
    public:
      PointsSupportShape();
      Vector3 GetCenter() const override;
      Vector3 Support(const Vector3& direction) const override;

      std::vector<Vector3> m_localSpacePoints;
      Vector3 m_scale;
      Quaternion m_rotation;
      Vector3 m_translation;
    };

    class SphereSupportShape : public SupportShape
    {
    public:
      Vector3 GetCenter() const override;
      Vector3 Support(const Vector3& direction) const override;

      Geometry::Sphere m_sphere;
    };

    class ObbSupportShape : public SupportShape
    {
    public:

      Vector3 GetCenter() const override;
      Vector3 Support(const Vector3& direction) const override;

      Vector3 m_scale;
      Quaternion m_rotation;
      Vector3 m_translation;
    };

    namespace VoronoiRegion
    {
      enum Type {
        Point0, Point1, Point2, Point3,
        Edge01, Edge02, Edge03, Edge12, Edge13, Edge23,
        Triangle012, Triangle013, Triangle023, Triangle123,
        Tetrahedra0123,
        Unknown
      };
      static const char* Names[] = { "Point0", "Point1", "Point2", "Point3",
        "Edge01", "Edge02", "Edge03", "Edge12", "Edge13", "Edge23",
        "Triangle012", "Triangle013", "Triangle023", "Triangle123",
        "Tetrahedra0123",
        "Unknown" };
    }

    class Gjk
    {
    public:

      // Point Tests
      static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
        size_t& newSize, int newIndices[4]);
      static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0);
      static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0);

      // Edge Tests
      static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, 
        const Vector3& p0, const Vector3& p1, size_t& newSize, int newIndices[4]);
      static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0, 
        const Vector3& p1);
      static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0, 
        const Vector3& p1);

      // Triangle Tests
      static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0,
        const Vector3& p1, const Vector3& p2, size_t& newSize, int newIndices[4]);
      static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0, 
        const Vector3& p1, const Vector3& p2);
      static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0, 
        const Vector3& p1, const Vector3& p2);

      // Tetrahedron Tests
      static VoronoiRegion::Type IdentifyVoronoiRegion(const Vector3& q, const Vector3& p0, 
        const Vector3& p1, const Vector3& p2, const Vector3& p3, size_t& newSize, 
        int newIndices[4]);
      static Vector3 FindClosestPoint(const Vector3& q, const Vector3& p0, 
        const Vector3& p1, const Vector3& p2, const Vector3& p3);
      static Vector3 ComputeSearchDirection(const Vector3& q, const Vector3& p0, 
        const Vector3& p1, const Vector3& p2, const Vector3& p3);

      // Simple structure that contains all information for a point in Gjk.
      struct CsoPoint
      {
        Vector3 m_pointA;
        Vector3 m_pointB;
        Vector3 m_csoPoint;
      };

      // Returns true if the shapes intersect.
      static bool Intersect(const SupportShape* shapeA, const SupportShape* shapeB,
        size_t maxIterations, CsoPoint& closestPoint, Real epsilon, std::vector<CsoPoint> &simplex);
      // Finds the point furthest in the given direction on the CSO (and the relevant points from each object)
      static CsoPoint ComputeSupport(const SupportShape* shapeA, const SupportShape* shapeB,
        const Vector3& direction);

      static VoronoiRegion::Type TetrahedronTestTriangle(const Vector3 &q, 
        const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3);
      static void Gjk::TetrahedronBarcentric(const Vector3 &q, const Vector3& p0, 
        const Vector3& p1, const Vector3& p2, const Vector3& p3,Real &u, 
        Real &v, Real &w, Real &t);
      static Gjk::CsoPoint FindClosestPoint(const Vector3 &q, const std::vector<CsoPoint> &simplex);
      static void ReduceSimplex(const Vector3 &q, std::vector<CsoPoint> &simplex);
    };
  }
}