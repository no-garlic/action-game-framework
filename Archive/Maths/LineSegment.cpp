
#include "LineSegment.h"
#include "Sphere.h"
#include "Plane.h"
#include "Matrix.h"

LineSegment::LineSegment(const Vector3 &point1, const Vector3 &point2)
{
    startPoint = point1;
    endPoint = point2;

    direction = Vector3(endPoint - startPoint).Normalize();
}

LineSegment::LineSegment(const LineSegment &lineSegment)
{
    *this = lineSegment;
}

LineSegment::LineSegment(const Vector3 points[2])
{
    startPoint = points[0];
    endPoint = points[1];

    direction = Vector3(endPoint - startPoint).Normalize();
}

bool LineSegment::InSegment(const Vector3 &testPoint) const
{
    // Only check to see if the point is between the two points, use IsColinear to determine if it is actually on the line.
    return ((Vector3(startPoint - testPoint).Dot(direction) * Vector3(endPoint - testPoint).Dot(direction)) <= 0);
}

bool LineSegment::IsColinear(const Vector3 &p, float threshold) const
{
    return DistanceSquared(p) < threshold;
}

void LineSegment::Normalize(void)
{
    direction.Normalize();
}

float LineSegment::Distance(const Vector3 &testPoint) const
{
    return sqrtf(DistanceSquared(testPoint));
}

void LineSegment::Translate(const Vector3 &delta)
{
    SetStartPoint(startPoint + delta);
    SetEndPoint(endPoint + delta);
}

void LineSegment::Rotate(const Matrix &m)
{
    SetStartPoint(startPoint.TransformCoord(m));
    SetEndPoint(endPoint.TransformCoord(m));
}

bool LineSegment::TestCollision(const Plane &plane) const
{
    // If the distance from the point to the plane vary in sign a negative result will be returned
    return (plane.GetNormal().Dot(plane.GetPoint() - startPoint) * plane.GetNormal().Dot(plane.GetPoint() - endPoint) <= 0);
}

bool LineSegment::TestCollision(const Sphere &sphere) const
{
    return (sphere.TestCollision(*this));
}

