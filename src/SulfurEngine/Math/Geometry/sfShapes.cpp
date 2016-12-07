#include "sfShapes.hpp"
#include "sfGeometry.hpp"

namespace Sulfur
{
  namespace Geometry
  {
    LineSegment::LineSegment()
    {
      m_start = m_end = Vector3::c_zero;
    }

    LineSegment::LineSegment(const Vector3 &start, const Vector3 &end)
    {
      m_start = start;
      m_end = end;
    }

    Ray::Ray()
    {
      m_start = m_direction = Vector3::c_zero;
    }

    Ray::Ray(const Vector3 &start, const Vector3 &dir)
    {
      m_start = start;
      m_direction = dir;
    }

    Ray Ray::Transform(const Matrix4& transform) const
    {
      Ray transformedRay;
      transformedRay.m_start = transform * m_start;
      transformedRay.m_direction = TransformNormal(transform, m_direction);
      return transformedRay;
    }

    Vector3 Ray::GetPoint(Real t) const
    {
      return m_start + m_direction * t;
    }

    Matrix3 ComputeCovarianceMatrix(const std::vector<Vector3>& points)
    {
      Matrix3 result;
      result.ZeroOut();

      size_t count = points.size();
      Vector3 centroid(0, 0, 0);
      for (size_t i = 0; i < count; ++i)
        centroid += points[i];

      centroid /= (Real)count;

      for (size_t i = 0; i < count; ++i)
      {
        Matrix3 current;
        Vector3 p = points[i] - centroid;

        for (int j = 0; j < 3; ++j)
          for (int m = j; m < 3; ++m)
            current[j][m] = current[m][j] = p[j] * p[m];
        result += current;
      }

      return result * (1 / (Real)count);
    }

    Matrix3 ComputeJacobiRotation(const Matrix3& matrix)
    {
      Matrix3 result;
      int p = 0;
      int q = 1;
      Real c, s;

      result.SetIdentity();

      for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
          if (i == j)
            continue;
          if (MathUtils::Abs(matrix[i][j]) > MathUtils::Abs(matrix[p][q]))
          {
            p = i;
            q = j;
          }

        }

      if (MathUtils::Abs(matrix[p][q]) > Real(0.0001))
      {
        Real r = (matrix[q][q] - matrix[p][p]) / (Real(2.0) * matrix[p][q]);
        Real t;
        if (r >= Real(0.0))
          t = Real(1.0) / (r + MathUtils::Sqrt(Real(1.0) + r * r));
        else
          t = Real(-1.0) / (-r + MathUtils::Sqrt(Real(1.0) + r * r));

        c = Real(1.0) / MathUtils::Sqrt(Real(1.0) + t * t);
        s = t * c;
      }
      else
      {
        c = Real(1.0);
        s = Real(0.0);
      }

      result[p][p] = c;
      result[p][q] = s;
      result[q][p] = -s;
      result[q][q] = c;

      return result;
    }

    void ComputeEigenValuesAndVectors(const Matrix3& covariance, Vector3& eigenValues, Matrix3& eigenVectors, int maxIterations)
    {
      eigenVectors.SetIdentity();
      Matrix3 jacobi;
      Matrix3 covMatrix = covariance;
      Real prevNorm = SF_REAL_MAX;

      for (int i = 0; i < maxIterations; ++i)
      {
        jacobi = ComputeJacobiRotation(covMatrix);

        eigenVectors = eigenVectors * jacobi;
        covMatrix = (jacobi.Transposed() * covMatrix) * jacobi;

        Real norm = Real(0.0);
        for (int i = 0; i < 3; ++i)
          for (int j = 0; j < 3; ++j)
          {
            if (i == j)
              continue;
            norm += covMatrix[i][j] * covMatrix[i][j];
          }

        if (norm >= prevNorm)
          break;
        prevNorm = norm;
      }

      eigenValues[0] = covMatrix[0][0];
      eigenValues[1] = covMatrix[1][1];
      eigenValues[2] = covMatrix[2][2];
    }


    //-----------------------------------------------------------------------------Sphere
    Sphere::Sphere()
    {
      m_center = Vector3::c_zero;
      m_radius = 0;
    }

    Sphere::Sphere(const Vector3& center, Real radius)
    {
      m_center = center;
      m_radius = radius;
    }

    void Sphere::ComputeCentroid(const std::vector<Vector3>& points)
    {
      size_t minX = 0, maxX = 0;
      size_t minY = 0, maxY = 0;
      size_t minZ = 0, maxZ = 0;

      size_t count = points.size();
      for (size_t i = 1; i < count; ++i)
      {
        if (points[i].GetX() < points[minX].GetX())
          minX = i;
        if (points[i].GetX() > points[maxX].GetX())
          maxX = i;
        if (points[i].GetY() < points[minY].GetY())
          minY = i;
        if (points[i].GetY() > points[maxY].GetY())
          maxY = i;
        if (points[i].GetZ() < points[minZ].GetZ())
          minZ = i;
        if (points[i].GetZ() > points[maxZ].GetZ())
          maxZ = i;
      }

      Real centerX = (points[maxX].GetX() + points[minX].GetX()) * Real(0.5);
      Real centerY = (points[maxY].GetY() + points[minY].GetY()) * Real(0.5);
      Real centerZ = (points[maxZ].GetZ() + points[minZ].GetZ()) * Real(0.5);

      m_center = Vector3(centerX, centerY, centerZ);

      //Furthest point
      Real maxDistSq = LengthSq(points[0] - m_center);
      for (int i = 1; i < count; ++i)
      {
        Real thisDistance = LengthSq(points[i] - m_center);

        if (thisDistance > maxDistSq)
          maxDistSq = thisDistance;
      }

      m_radius = sqrtf(maxDistSq);
    }

    void Sphere::ComputeRitter(const std::vector<Vector3>& points)
    {
      size_t minX = 0, maxX = 0;
      size_t minY = 0, maxY = 0;
      size_t minZ = 0, maxZ = 0;

      size_t count = points.size();
      for (size_t i = 1; i < count; ++i)
      {
        if (points[i].GetX() < points[minX].GetX())
          minX = i;
        if (points[i].GetX() > points[maxX].GetX())
          maxX = i;
        if (points[i].GetY() < points[minY].GetY())
          minY = i;
        if (points[i].GetY() > points[maxY].GetY())
          maxY = i;
        if (points[i].GetZ() < points[minZ].GetZ())
          minZ = i;
        if (points[i].GetZ() > points[maxZ].GetZ())
          maxZ = i;
      }

      Real distSqX = LengthSq(points[maxX] - points[minX]);
      Real distSqY = LengthSq(points[maxY] - points[minY]);
      Real distSqZ = LengthSq(points[maxZ] - points[minZ]);

      //Get max spread
      size_t min, max;

      if (distSqX > distSqY && distSqX > distSqZ)
      {
        min = minX;
        max = maxX;
      }
      else if (distSqY > distSqX && distSqY > distSqZ)
      {
        min = minY;
        max = maxY;
      }
      else
      {
        min = minZ;
        max = maxZ;
      }

      m_center = (points[max] + points[min]) * Real(0.5);
      m_radius = Length(points[max] - m_center);

      //Expand sphere
      for (int i = 0; i < count; ++i)
      {
        Vector3 centerToPoint = points[i] - m_center;
        if (centerToPoint.LengthSq() <= m_radius * m_radius)
          continue;

        Vector3 onSphere = m_center - (m_radius * centerToPoint.Normalized());

        m_center = (points[i] + onSphere) * Real(0.5);
        m_radius = Length(points[i] - onSphere) * Real(0.5);
      }
    }

    void Sphere::ComputePCA(const std::vector<Vector3>& points)
    {
      Matrix3 covariance = ComputeCovarianceMatrix(points);
      Matrix3 eigenVectors;
      Vector3 eigenValues;

      ComputeEigenValuesAndVectors(covariance, eigenValues, eigenVectors, 50);

      Vector3 e;
      int maxC = 0;
      Real maxF, maxE = fabs(eigenValues.GetX());

      if ((maxF = fabs(eigenValues.GetY())) > maxE)
      {
        maxC = 1;
        maxE = maxF;
      }
      if ((maxF = fabs(eigenValues.GetZ())) > maxE)
      {
        maxC = 2;
        maxE = maxF;
      }

      e.SetX(eigenVectors[0][maxC]);
      e.SetY(eigenVectors[1][maxC]);
      e.SetZ(eigenVectors[2][maxC]);

      //Find extreme points
      size_t min, max;
      size_t count = points.size();
      Real minProj = SF_REAL_MAX;
      Real maxProj = -SF_REAL_MAX;

      for (size_t i = 0; i < count; ++i)
      {
        Real proj = Dot(e, points[i]);
        if (proj < minProj)
        {
          min = i;
          minProj = proj;
        }
        if (proj > maxProj)
        {
          max = i;
          maxProj = proj;
        }
      }

      //Compute initial sphere
      Real dist = Length(points[max] - points[min]);
      m_radius = dist * Real(0.5);
      m_center = (points[max] + points[min]) * Real(0.5);

      //Expand sphere
      for (int i = 0; i < count; ++i)
      {
        Vector3 centerToPoint = points[i] - m_center;
        if (centerToPoint.LengthSq() <= m_radius * m_radius)
          continue;

        Vector3 onSphere = m_center - (m_radius * centerToPoint.Normalized());

        m_center = (points[i] + onSphere) * Real(0.5);
        m_radius = Length(points[i] - onSphere) * Real(0.5);
      }
    }


    bool Sphere::ContainsPoint(const Vector3& point)
    {
      return PointSphere(point, m_center, m_radius);
    }

    Vector3 Sphere::GetCenter() const
    {
      return m_center;
    }

    Real Sphere::GetRadius() const
    {
      return m_radius;
    }

    bool Sphere::Compare(const Sphere& rhs, Real epsilon) const
    {
      Real posDiff = Length(m_center - rhs.m_center);
      Real radiusDiff = MathUtils::Abs(m_radius - rhs.m_radius);

      return posDiff < epsilon && radiusDiff < epsilon;
    }

    Aabb::Aabb()
    {
      //set the aabb to an initial bad value (where the min is smaller than the max)
      m_min.Splat(SF_REAL_MAX);
      m_max.Splat(-SF_REAL_MAX);
    }

    Aabb::Aabb(const Vector3& min, const Vector3& max)
    {
      m_min = min;
      m_max = max;
    }

    Aabb Aabb::BuildFromCenterAndHalfExtents(const Vector3& center, const Vector3& halfExtents)
    {
      return Aabb(center - halfExtents, center + halfExtents);
    }

    Real Aabb::GetVolume() const
    {
      return (m_max[0] - m_min[0]) * (m_max[1] - m_min[1]) * (m_max[2] - m_min[2]);
    }

    Real Aabb::GetSurfaceArea() const
    {
      Real length = m_max[0] - m_min[0];
      Real width = m_max[1] - m_min[1];
      Real height = m_max[2] - m_min[2];

      return 2 * length * height + 2 * width * height + 2 * length * width;
    }

    bool Aabb::Contains(const Aabb& aabb) const
    {
      if (m_max[0] <= aabb.m_max[0] || m_min[0] >= aabb.m_min[0])
        return false;
      if (m_max[1] <= aabb.m_max[1] || m_min[1] >= aabb.m_min[1])
        return false;
      if (m_max[2] <= aabb.m_max[2] || m_min[2] >= aabb.m_min[2])
        return false;

      return true;
    }

    void Aabb::Expand(const Vector3& point)
    {
      for (int i = 0; i < 3; ++i)
      {
        m_min[i] = std::min(m_min[i], point[i]);
        m_max[i] = std::max(m_max[i], point[i]);
      }
    }

    Aabb Aabb::Combine(const Aabb& lhs, const Aabb& rhs)
    {
      Aabb result;
      for (int i = 0; i < 3; ++i)
      {
        result.m_min[i] = std::min(lhs.m_min[i], rhs.m_min[i]);
        result.m_max[i] = std::max(lhs.m_max[i], rhs.m_max[i]);
      }
      return result;
    }

    bool Aabb::Compare(const Aabb& rhs, Real epsilon) const
    {
      Real pos1Diff = Length(m_min - rhs.m_min);
      Real pos2Diff = Length(m_max - rhs.m_max);

      return pos1Diff < epsilon && pos2Diff < epsilon;
    }

    void Aabb::Transform(const Vector3& scale, const Matrix3& rotation, const Vector3& translation)
    {
      Vector3 hSize = GetHalfSize();
      hSize = scale * hSize;

      Vector3 rotatedHSize;
      rotatedHSize.SetX(fabs(rotation[0][0] * hSize.GetX()) + fabs(rotation[0][1] * hSize.GetY()) 
        + fabs(rotation[0][2] * hSize.GetZ()));
      rotatedHSize.SetY(fabs(rotation[1][0] * hSize.GetX()) + fabs(rotation[1][1] * hSize.GetY()) 
        + fabs(rotation[1][2] * hSize.GetZ()));
      rotatedHSize.SetZ(fabs(rotation[2][0] * hSize.GetX()) + fabs(rotation[2][1] * hSize.GetY()) 
        + fabs(rotation[2][2] * hSize.GetZ()));

      hSize = rotatedHSize;

      Vector3 center = GetCenter();

      center = translation + rotation * (scale * center);

      m_min = center - hSize;
      m_max = center + hSize;
    }

    Vector3 Aabb::GetMin() const
    {
      return m_min;
    }

    Vector3 Aabb::GetMax() const
    {
      return m_max;
    }

    Vector3 Aabb::GetCenter() const
    {
      return (m_min + m_max) * Real(0.5);
    }

    Vector3 Aabb::GetHalfSize() const
    {
      return (m_max - m_min) * Real(0.5);
    }

    Triangle::Triangle()
    {
      m_points[0] = m_points[1] = m_points[2] = Vector3::c_zero;
    }

    Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
      m_points[0] = p0;
      m_points[1] = p1;
      m_points[2] = p2;
    }

    Plane::Plane()
    {
      m_data = Vector4::c_zero;
    }

    Plane::Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
      Set(p0, p1, p2);
    }

    Plane::Plane(const Vector3& normal, const Vector3& point)
    {
      Set(normal, point);
    }

    void Plane::Set(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
      Vector3 normal = Cross(p1 - p0, p2 - p0);

      if (normal.LengthSq() != 0)
        normal.Normalize();

      m_data.Set(normal.GetX(), normal.GetY(), normal.GetZ(), -Dot(normal, p0));
    }

    void Plane::Set(const Vector3& normal, const Vector3& point)
    {
      Vector3 normalizedNormal = Normalized(normal);

      m_data.Set(normalizedNormal.GetX(), normalizedNormal.GetY(), normalizedNormal.GetZ(),
        -Dot(normalizedNormal, point));

      SF_ASSERT(normalizedNormal.Dot(point) + m_data[3] == 0, "Plane is not correct");
    }

    Vector3 Plane::GetNormal() const
    {
      return Vector3(m_data.GetX(), m_data.GetY(), m_data.GetZ());
    }

    Real Plane::GetDistance() const
    {
      return m_data.GetW();
    }

    Plane Plane::Transformed(const Matrix4 &transform) const
    {
      Plane p;
      p.m_data = m_data;

      p.m_data = transform.Inverted().Transposed() * p.m_data;

      Real l = p.GetNormal().Length();

      p.m_data = p.m_data / l;

      return p;
    }

    void Frustum::Set(const Vector3& lbn, const Vector3& rbn, const Vector3& rtn, const Vector3& ltn,
      const Vector3& lbf, const Vector3& rbf, const Vector3& rtf, const Vector3& ltf)
    {
      m_points[0] = lbn;
      m_points[1] = rbn;
      m_points[2] = rtn;
      m_points[3] = ltn;
      m_points[4] = lbf;
      m_points[5] = rbf;
      m_points[6] = rtf;
      m_points[7] = ltf;

      //left
      m_planes[0].Set(lbf, ltf, lbn);
      //right
      m_planes[1].Set(rbn, rtf, rbf);
      //top
      m_planes[2].Set(ltn, ltf, rtn);
      //bot
      m_planes[3].Set(rbn, lbf, lbn);
      //near
      m_planes[4].Set(lbn, ltn, rbn);
      //far
      m_planes[5].Set(rbf, rtf, lbf);
    }

    Vector4* Frustum::GetPlanes() const
    {
      return (Vector4*)m_planes;
    }

  }
}