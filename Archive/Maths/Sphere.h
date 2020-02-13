
#ifndef __MATH_SPHERE_H_
#define __MATH_SPHERE_H_

#include "Type.h"
#include "Str.h"
#include "Matrix.h"
#include "Vector3.h"

class Ray;
class Plane;
class Box;
class Line;
class LineSegment;
class Frustum;

class Sphere
{
public:
    Sphere() : radius(0.0f) {}
    Sphere(const Vector3 &center, float radius) : center(center), radius(radius) {}

    // Get the center and radius
    Vector3 &GetCenter() { return center; }
    const Vector3 &GetCenter() const { return center; }
    float GetRadius() const { return radius; }

    // Set the center and radius
    void SetCenter(const Vector3 &center) { this->center = center; }
    void SetRadius(float radius) { this->radius = radius; }

    // Maximise this sphere to contain the given sphere(s)
    void Maximize(const Sphere &sphere1, const Sphere &sphere2);
    void Maximize(const Sphere &sphere) { Maximize(*this, sphere); }

    // Transforms the center and scales radius the using the matrix
    void TransformAndScale(const Matrix &matrix);

    // Check if any of the sphere is visible in the given frustum
    bool CheckVisibility(const Frustum &frustum) const;

    // Reset the sphere to 0
    void Reset() { center.Set(0.0f, 0.0f, 0.0f); radius = 0.0f; }

    // Test a collision between the sphere and the given object
    bool TestCollision(const Vector3 &point) const;
    bool TestCollision(const Plane &plane) const;
    bool TestCollision(const Box &box) const;
    bool TestCollision(const Sphere &sphere) const;
    bool TestCollision(const Line &line) const;
    bool TestCollision(const Ray &ray) const;
    bool TestCollision(const LineSegment &lineSegment) const;

    // Print the sphere's data to a string
    String ToString() const
    {
        StringStream ss;
        ss.precision(2);
        ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
        ss << "(" << center[0] + 0.001f << ", " << center[1] + 0.001f << ", " << center[2] + 0.001f << ") : " << radius;
        return ss.str();
    }

    Vector3  center;
    float radius;

};

#endif // __MATH_SPHERE_H_

