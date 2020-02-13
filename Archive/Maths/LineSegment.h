
#ifndef __MATH_LINE_SEGMENT_H_
#define __MATH_LINE_SEGMENT_H_

#include "Type.h"
#include "Mathematics.h"
#include "Vector3.h"

class Sphere;
class Plane;
class Matrix;

class LineSegment
{
public:
    LineSegment() {}
    LineSegment(const Vector3 &point1, const Vector3 &point2);
    LineSegment(const LineSegment &lineSegment);
    LineSegment(const Vector3 points[2]);

    // Manage the direction and position of the line
    const Vector3 &GetStartPoint() const { return startPoint; }
    const Vector3 &GetEndPoint()   const { return endPoint; }
    const Vector3 &GetDirection()  const { return direction; }
    inline void SetStartPoint(const Vector3 &newPoint);
    inline void SetEndPoint(const Vector3 &newPoint);

    // Distance functions
    float Distance(const Vector3 &point) const;
    inline float DistanceSquared(const Vector3 &testPoint) const;

    // Test the points
    bool InSegment(const Vector3 &testPoint) const;
    bool IsColinear(const Vector3 &p, float threshold = FLOAT_EPSILON) const;

    // Transform this line.
    void Translate(const Vector3 &delta);
    void Rotate(const Matrix &m);

    // Normalizes the direction of this line,
    void Normalize();

    // Tests for a collision between this line and the given primitive.
    // (Collisions are two sided so two collision points may occur)
    bool TestCollision(const Plane &plane) const;
    bool TestCollision(const Sphere &sphere) const;

private:
    Vector3 direction;
    Vector3 startPoint;
    Vector3 endPoint;

};

inline void LineSegment::SetEndPoint(const Vector3 &newEndPoint)
{
    endPoint = newEndPoint;
    direction = Vector3(endPoint - startPoint).Normalize();
}

inline void LineSegment::SetStartPoint(const Vector3 &newStartPoint)
{
    startPoint = newStartPoint;
    direction = Vector3(endPoint - startPoint).Normalize();
}

inline float LineSegment::DistanceSquared(const Vector3 &testPoint) const
{
    Vector3 displacement = testPoint - startPoint;
    float projectedDistance = displacement.Dot(direction);

    // Test the projection
    if (projectedDistance > 0.0f)
    {
        // Test if the point is behind the end point
        if (projectedDistance > Vector3(endPoint - startPoint).Dot(direction))
        {
            // If the point is behind the end point, get the distance squared from the end point
            return Vector3(testPoint - endPoint).LengthSq();
        }
        else
        {
            // If between both lines, get the offset^2 from the line
            return (displacement.LengthSq() - projectedDistance*projectedDistance);
        }
    }
    else
    {
        // If behind the point, get the distance^2 from the point
        return displacement.LengthSq();
    }
}

#endif // __MATH_LINE_SEGMENT_H_

