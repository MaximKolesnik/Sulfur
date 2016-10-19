#include "sfShapes.hpp"
#include "sfGeometry.hpp"

namespace Sulfur
{
  namespace Geometry
  {
    LineSegment::LineSegment()
    {
      mStart = mEnd = Vector3::c_zero;
    }

    LineSegment::LineSegment(const Vector3 &start, const Vector3 &end)
    {
      mStart = start;
      mEnd = end;
    }

    Ray::Ray()
    {
      mStart = mDirection = Vector3::c_zero;
    }

    Ray::Ray(const Vector3 &start, const Vector3 &dir)
    {
      mStart = start;
      mDirection = dir;
    }

    Ray Ray::Transform(const Matrix4& transform) const
    {
      Ray transformedRay;
      transformedRay.mStart = transform * mStart;
      transformedRay.mDirection = TransformNormal(transform, mDirection);
      return transformedRay;
    }

    Vector3 Ray::GetPoint(Real t) const
    {
      return mStart + mDirection * t;
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
      mCenter = Vector3::c_zero;
      mRadius = 0;
    }

    Sphere::Sphere(const Vector3& center, Real radius)
    {
      mCenter = center;
      mRadius = radius;
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

      mCenter = Vector3(centerX, centerY, centerZ);

      //Furthest point
      Real maxDistSq = LengthSq(points[0] - mCenter);
      for (int i = 1; i < count; ++i)
      {
        Real thisDistance = LengthSq(points[i] - mCenter);

        if (thisDistance > maxDistSq)
          maxDistSq = thisDistance;
      }

      mRadius = sqrtf(maxDistSq);
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

      mCenter = (points[max] + points[min]) * Real(0.5);
      mRadius = Length(points[max] - mCenter);

      //Expand sphere
      for (int i = 0; i < count; ++i)
      {
        Vector3 centerToPoint = points[i] - mCenter;
        if (centerToPoint.LengthSq() <= mRadius * mRadius)
          continue;

        Vector3 onSphere = mCenter - (mRadius * centerToPoint.Normalized());

        mCenter = (points[i] + onSphere) * Real(0.5);
        mRadius = Length(points[i] - onSphere) * Real(0.5);
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
      mRadius = dist * Real(0.5);
      mCenter = (points[max] + points[min]) * Real(0.5);

      //Expand sphere
      for (int i = 0; i < count; ++i)
      {
        Vector3 centerToPoint = points[i] - mCenter;
        if (centerToPoint.LengthSq() <= mRadius * mRadius)
          continue;

        Vector3 onSphere = mCenter - (mRadius * centerToPoint.Normalized());

        mCenter = (points[i] + onSphere) * Real(0.5);
        mRadius = Length(points[i] - onSphere) * Real(0.5);
      }
    }


    bool Sphere::ContainsPoint(const Vector3& point)
    {
      return PointSphere(point, mCenter, mRadius);
    }

    Vector3 Sphere::GetCenter() const
    {
      return mCenter;
    }

    Real Sphere::GetRadius() const
    {
      return mRadius;
    }

    bool Sphere::Compare(const Sphere& rhs, Real epsilon) const
    {
      Real posDiff = Length(mCenter - rhs.mCenter);
      Real radiusDiff = MathUtils::Abs(mRadius - rhs.mRadius);

      return posDiff < epsilon && radiusDiff < epsilon;
    }

    Aabb::Aabb()
    {
      //set the aabb to an initial bad value (where the min is smaller than the max)
      mMin.Splat(SF_REAL_MAX);
      mMax.Splat(-SF_REAL_MAX);
    }

    Aabb::Aabb(const Vector3& min, const Vector3& max)
    {
      mMin = min;
      mMax = max;
    }

    Aabb Aabb::BuildFromCenterAndHalfExtents(const Vector3& center, const Vector3& halfExtents)
    {
      return Aabb(center - halfExtents, center + halfExtents);
    }

    Real Aabb::GetVolume() const
    {
      return (mMax.GetX() - mMin.GetX()) * (mMax.GetY() - mMin.GetY()) 
        * (mMax.GetZ() - mMin.GetZ());
    }

    Real Aabb::GetSurfaceArea() const
    {
      Real length = mMax.GetX() - mMin.GetX();
      Real width = mMax.GetY() - mMin.GetY();
      Real height = mMax.GetZ() - mMin.GetZ();

      return 2 * length * height + 2 * width * height + 2 * length * width;
    }

    bool Aabb::Contains(const Aabb& aabb) const
    {
      if (mMax.GetX() <= aabb.mMax.GetX() || mMin.GetX() >= aabb.mMin.GetX())
        return false;
      if (mMax.GetY() <= aabb.mMax.GetY() || mMin.GetY() >= aabb.mMin.GetY())
        return false;
      if (mMax.GetZ() <= aabb.mMax.GetZ() || mMin.GetZ() >= aabb.mMin.GetZ())
        return false;

      return true;
    }

    void Aabb::Expand(const Vector3& point)
    {
      for (int i = 0; i < 3; ++i)
      {
        mMin[i] = std::min(mMin[i], point[i]);
        mMax[i] = std::max(mMax[i], point[i]);
      }
    }

    Aabb Aabb::Combine(const Aabb& lhs, const Aabb& rhs)
    {
      Aabb result;
      for (int i = 0; i < 3; ++i)
      {
        result.mMin[i] = std::min(lhs.mMin[i], rhs.mMin[i]);
        result.mMax[i] = std::max(lhs.mMax[i], rhs.mMax[i]);
      }
      return result;
    }

    bool Aabb::Compare(const Aabb& rhs, Real epsilon) const
    {
      Real pos1Diff = Length(mMin - rhs.mMin);
      Real pos2Diff = Length(mMax - rhs.mMax);

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

      mMin = center - hSize;
      mMax = center + hSize;
    }

    Vector3 Aabb::GetMin() const
    {
      return mMin;
    }

    Vector3 Aabb::GetMax() const
    {
      return mMax;
    }

    Vector3 Aabb::GetCenter() const
    {
      return (mMin + mMax) * Real(0.5);
    }

    Vector3 Aabb::GetHalfSize() const
    {
      return (mMax - mMin) * Real(0.5);
    }

    Triangle::Triangle()
    {
      mPoints[0] = mPoints[1] = mPoints[2] = Vector3::c_zero;
    }

    Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
      mPoints[0] = p0;
      mPoints[1] = p1;
      mPoints[2] = p2;
    }

    Plane::Plane()
    {
      mData = Vector4::c_zero;
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

      mData.Set(normal.GetX(), normal.GetY(), normal.GetZ(), Dot(normal, p0));
    }

    void Plane::Set(const Vector3& normal, const Vector3& point)
    {
      Vector3 normalizedNormal = Normalized(normal);

      mData.Set(normalizedNormal.GetX(), normalizedNormal.GetY(), normalizedNormal.GetZ(),
        Dot(normalizedNormal, point));
    }

    Vector3 Plane::GetNormal() const
    {
      return Vector3(mData.GetX(), mData.GetY(), mData.GetZ());
    }

    Real Plane::GetDistance() const
    {
      return mData.GetW();
    }


    void Frustum::Set(const Vector3& lbn, const Vector3& rbn, const Vector3& rtn, const Vector3& ltn,
      const Vector3& lbf, const Vector3& rbf, const Vector3& rtf, const Vector3& ltf)
    {
      mPoints[0] = lbn;
      mPoints[1] = rbn;
      mPoints[2] = rtn;
      mPoints[3] = ltn;
      mPoints[4] = lbf;
      mPoints[5] = rbf;
      mPoints[6] = rtf;
      mPoints[7] = ltf;

      //left
      mPlanes[0].Set(lbf, ltf, lbn);
      //right
      mPlanes[1].Set(rbn, rtf, rbf);
      //top
      mPlanes[2].Set(ltn, ltf, rtn);
      //bot
      mPlanes[3].Set(rbn, lbf, lbn);
      //near
      mPlanes[4].Set(lbn, ltn, rbn);
      //far
      mPlanes[5].Set(rbf, rtf, lbf);
    }

    Vector4* Frustum::GetPlanes() const
    {
      return (Vector4*)mPlanes;
    }

  }
}