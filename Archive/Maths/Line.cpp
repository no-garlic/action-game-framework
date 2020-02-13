
#include "Line.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Sphere.h"
#include "Plane.h"
#include "libStandard.h"

Line &Line::Translate(const Vector3 &delta)
{
    position.Add(delta);
    return *this;
}

Line &Line::Rotate(const Matrix &m)
{
    direction = direction.TransformCoord(m);
    ASSERT(false, "Check this");
    position = position.TransformCoord(m);
    return *this;
}

Line &Line::SelfRotate(const Matrix &m)
{
    direction = direction.TransformCoord(m);
    return *this;
}

Line &Line::Scale(float s)
{
    position.Scale(s);
    return *this;
}

Line &Line::Normalize()
{
    direction.Normalize();
    return *this;
}

float Line::Distance(const Vector3 &point) const
{
    Vector3 displacement = point - position;
    float projectedDistance = displacement.Dot(direction);

    return (float)sqrtf(displacement.LengthSq() - projectedDistance*projectedDistance / direction.LengthSq());
}

float Line::Distance(const Line &line) const
{
    Vector3 normal = direction;
    normal.Cross(line.direction);

    float normalMag;
    if ((normalMag = normal.LengthSq()) < 1.0e-4)
    {
        // The lines are parallel
        // just take the distance between the points
        return Vector3(position - line.position).Length();
    }

    // Otherwise calculate the minimum distance between
    // the lines
    normal /= (float)sqrtf(normalMag);
    return (float) ::fabs(normal.Dot(position - line.position));
}

float Line::DistanceSquared(const Vector3 &testPoint) const
{
    Vector3 displacement = testPoint;
    displacement.Subtract(position);

    float projectedDistance = displacement.Dot(direction);

    return displacement.LengthSq() - projectedDistance * projectedDistance;
}

bool Line::IsColinear(const Vector3 &p, float threshold) const
{
    return DistanceSquared(p) < threshold;
}

bool Line::TestCollision(const Sphere &sphere) const
{
    return (sphere.TestCollision(*this));
}

bool Line::TestCollision(const Plane &plane) const
{
    float planeDotNorm = plane.GetNormal().Dot(direction);
    if (planeDotNorm >= 1.0e-6f || planeDotNorm <= -1.0e-6f)
        return true;
    else
        return false;
}

bool Line::TestCollision(const Sphere &sphere, Vector3 &collisionPoint) const
{
    float distance1, distance2;
    if (TestCollision(sphere, distance1, distance2))
    {
        // Take the distance that is nearest to the reference point
        if (fabsf(distance1) < fabsf(distance2))
            collisionPoint = position + direction * distance1;
        else
            collisionPoint = position + direction * distance2;
        return true;
    }

    return false;
}

bool Line::TestCollision(const Sphere &sphere, Vector3 &collisionPoint1, Vector3 &collisionPoint2) const
{
    float distance1, distance2;
    if (TestCollision(sphere, distance1, distance2))
    {
        collisionPoint1 = position + direction * distance1;
        collisionPoint2 = position + direction * distance2;
        return true;
    }

    return false;
}

bool Line::TestCollision(const Sphere &sphere, float &distance1, float &distance2) const
{
    // Set up data to hold the ax^2+bx+c (for each x,y,z)
    Vector3 a(0.0f, 0.0f, 0.0f), b(0.0f, 0.0f, 0.0f), c(0.0f, 0.0f, 0.0f);

    float aTotal, bTotal, cTotal;
    float underSqrt, sqrtUnderSqrt;

    // Set a,b,c values
    a.x = direction.x * direction.x;
    a.y = direction.y * direction.y;
    a.z = direction.z * direction.z;

    b.x = 2 * direction.x * position.x - 2 * direction.x * sphere.center.x;
    b.y = 2 * direction.y * position.y - 2 * direction.y * sphere.center.y;
    b.z = 2 * direction.z * position.z - 2 * direction.z * sphere.center.z;

    c.x = position.x * position.x - 2 * position.x * sphere.center.x + sphere.center.x * sphere.center.x;
    c.y = position.y * position.y - 2 * position.y * sphere.center.y + sphere.center.y * sphere.center.y;
    c.z = position.z * position.z - 2 * position.z * sphere.center.z + sphere.center.z * sphere.center.z;

    // total a,b,c
    aTotal = a.x + a.y + a.z;
    bTotal = b.x + b.y + b.z;
    cTotal = c.x + c.y + c.z - (sphere.radius * sphere.radius);

    // Detect collision  ( if b^2 < 4ac)
    underSqrt = bTotal * bTotal - 4.0f * aTotal * cTotal;

    if (underSqrt < 0)
    {
        // can't find sqrt therefore no collision
        return false;
    }
    else if (underSqrt == 0)
    {
        // just touching
        distance1 = (-bTotal) / (2.0f * aTotal);
        distance2 = distance1;
        return true;
    }
    else
    {
        // Cuts with point pair to1,to2
        sqrtUnderSqrt = (float)sqrt(underSqrt);

        distance1 = (-bTotal + sqrtUnderSqrt) / (2.0f * aTotal);
        distance2 = (-bTotal - sqrtUnderSqrt) / (2.0f * aTotal);

        return true;
    }
}

