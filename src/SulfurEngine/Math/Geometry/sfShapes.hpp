#pragma once

#include "Math\sfVector3.hpp"
#include "Math\sfVector4.hpp"
#include "Math\sfMatrix4.hpp"
#include "Math\sfMatrix3.hpp"

namespace Sulfur
{
  namespace Geometry
  {
    class LineSegment
    {
    public:
      LineSegment();
      LineSegment(const Vector3 &start, const Vector3 &end);

      Vector3 mStart;
      Vector3 mEnd;
    };

    class Ray
    {
    public:
      Ray();
      Ray(const Vector3 &start, const Vector3 &dir);

      // Transforms this ray to another space by the given matrix 4
      Ray Transform(const Matrix4& transform) const;
      // Returns the a point at the given t value.
      Vector3 GetPoint(Real t) const;

      Vector3 mStart;
      Vector3 mDirection;
    };

    Matrix3 ComputeCovarianceMatrix(const std::vector<Vector3>& points);
    Matrix3 ComputeJacobiRotation(const Matrix3& matrix);
    void ComputeEigenValuesAndVectors(const Matrix3& covariance, Vector3& eigenValues, Matrix3& eigenVectors, int maxIterations);

    class Sphere
    {
    public:
      Sphere();
      Sphere(const Vector3& center, Real radius);

      void ComputeCentroid(const std::vector<Vector3>& points);
      void ComputeRitter(const std::vector<Vector3>& points);
      void ComputePCA(const std::vector<Vector3>& points);

      // Does the sphere contain the given point?
      bool ContainsPoint(const Vector3& point);

      Vector3 GetCenter() const;
      Real GetRadius() const;

      // See if this aabb is equal to another (with epsilon). Used for unit testing.
      bool Compare(const Sphere& rhs, Real epsilon) const;

      Vector3 mCenter;
      Real mRadius;
    };

    class Aabb
    {
    public:
      Aabb();
      Aabb(const Vector3& min, const Vector3& max);

      static Aabb BuildFromCenterAndHalfExtents(const Vector3& center, const Vector3& halfExtents);

      // Computes the volume of this aabb.
      Real GetVolume() const;
      // Computes the surface area of this aabb.
      Real GetSurfaceArea() const;

      // Does this aabb completely contain the given aabb (not an intersection test).
      bool Contains(const Aabb& aabb) const;
      // Expand the to include the given point.
      void Expand(const Vector3& point);
      // Combine the two aabbs into a new one
      static Aabb Combine(const Aabb& lhs, const Aabb& rhs);
      // See if this aabb is equal to another (with epsilon). Used for unit testing.
      bool Compare(const Aabb& rhs, Real epsilon) const;

      void Transform(const Vector3& scale, const Matrix3& rotation, const Vector3& translation);

      Vector3 GetMin() const;
      Vector3 GetMax() const;
      Vector3 GetCenter() const;
      Vector3 GetHalfSize() const;

      Vector3 mMin;
      Vector3 mMax;
    };

    class Triangle
    {
    public:
      Triangle();
      Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);

      Vector3 mPoints[3];
    };

    class Plane
    {
    public:
      Plane();
      // Create the plane from a triangle
      Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2);
      // Create the plane from a point and a normal
      Plane(const Vector3& normal, const Vector3& point);

      // Create a plane from a triangle. The plane's normal should be normalized.
      void Set(const Vector3& p0, const Vector3& p1, const Vector3& p2);
      // Create a plane from a point and a normal. The plane's normal should be normalized.
      void Set(const Vector3& normal, const Vector3& point);

      // Get the normal of this plane.
      Vector3 GetNormal() const;
      // Get the 'd' value (the distance from the origin) which can be computed as Dot(origin - pointOnPlane, normal)
      Real GetDistance() const;


      Vector4 mData;
    };

    class Frustum
    {
    public:

      void Set(const Vector3& lbn, const Vector3& rbn, const Vector3& rtn, const Vector3& ltn,
        const Vector3& lbf, const Vector3& rbf, const Vector3& rtf, const Vector3& ltf);

      // Returns the start of an array of the 6 planes of the frustum.
      Vector4* GetPlanes() const;

      // The normals all point inwards (towards the centroid of the frustum).
      Plane mPlanes[6];

      // To easily draw the frustum the original points it was constructed from are stored.
      Vector3 mPoints[8];
    };
  }
}