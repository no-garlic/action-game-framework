
#ifndef __MATH_LINE_H_
#define __MATH_LINE_H_

#include "Type.h"
#include "Mathematics.h"
#include "Vector3.h"

class Sphere;
class Plane;
class Matrix;

class Line
{
public:
    Line() {}
    Line(const Vector3 &d, const Vector3 &p) : direction(d), position(p) { direction.Normalize(); }

    // Manage the direction and position of the line
    void SetDirection(const Vector3 &direction) { this->direction = direction; }
    void SetPosition(const Vector3 &position) { this->position = position; }
    const Vector3 &GetDirection() const { return direction; }
    const Vector3 &GetPosition() const { return position; }

    // Determines if the given point is on this ray.
    bool IsColinear(const Vector3 &p, float threshold = FLOAT_EPSILON) const;

    // Tranform this line.
    Line &Translate(const Vector3 &delta);
    Line &Rotate(const Matrix &m);
    Line &SelfRotate(const Matrix &m);
    Line &Scale(float s);

    // Normalizes the direction of this line,
    Line &Normalize();

    // Calculates the perpendicular distance from this line to the given argument.
    float Distance(const Vector3 &point) const;
    float Distance(const Line &line) const;
    float DistanceSquared(const Vector3 &testPoint) const;

    // Tests for a collision between this line and the given primitive.
    // (Collisions are two sided so two collision points may occur)
    bool TestCollision(const Sphere &sphere) const;
    bool TestCollision(const Plane &plane) const;
    bool TestCollision(const Sphere &sphere, Vector3 &collisionPoint) const;
    bool TestCollision(const Sphere &sphere, Vector3 &collisionPoint1, Vector3 &collisionPoint2) const;
    bool TestCollision(const Sphere &sphere, float &distance1, float &distance2) const;

    Vector3 position;
    Vector3 direction;

};

#endif // __MATH_LINE_H_

