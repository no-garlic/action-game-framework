
#ifndef __MATH_PLANE_H_
#define __MATH_PLANE_H_

#include "Type.h"
#include "Str.h"

class Matrix;
class Vector3;
class Vector4;
class Line;
class Ray;
class Sphere;
class Box;

class Plane : public D3DXPLANE
{
public:
    Plane();
    Plane(const float *f);
    Plane(float a, float b, float c, float d);
    Plane(const Vector3 &normal, float d);
    Plane(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);

    // The Normal
    const Vector3 &GetNormal() const;
    void SetNormal(const Vector3 &normal);

    // The distance
    float GetDistance() const;
    void SetDistance(float distance);

    // Get the point on this plane that is the closest to the origin.
    Vector3 GetPoint() const;

    enum Classification
    {
        FRONT       = 1,    // Only in the front space.
        BACK        = 2,    // Only in the back space.
        COINCIDENT  = 3,    // Parts in both the front and back spaces.
        COPLANAR    = 4     // On the plane that divides the front and back spaces.
    };

    // Determines which side of the plane the given point is on.
    Classification WhichSide(const Vector3 &p) const;

    // Determines which side of the plane a point is on with the given threshold tolerance value.
    Classification WhichSide(const Vector3 &p, float threshold) const;

    // Determines which side of the plane an array of points is on.
    Classification WhichSide(int num_points, const Vector3 *points) const;
    Classification WhichSide(const Box &box) const;

    // Determines which side of the plane that the specified sphere is on.
    Classification WhichSide(const Sphere &sphere) const;
    bool IsBehind(const Sphere &sphere) const;
    bool IsInFront(const Sphere &sphere) const;

    // Gets the intersection between this plane and the given plane.
    Line Intersect(const Plane &plane) const;

    // Gets the intersection between this plane and the given line.
    Vector3 Intersect(const Line &line) const;

    // Gets the intersection between this plane and the given line.
    bool Intersect(bool twoSided, const Line &line, Vector3 &intersectionPoint) const;
    bool Intersect(bool twoSided, const Ray  &ray, Vector3 &intersectionPoint) const;

    // Gets the intersection between this plane and the given line.
    bool Intersect(bool twoSided, const Line &line, Vector3 &intersectionPoint, float &t) const;
    bool Intersect(bool twoSided, const Ray  &ray, Vector3 &intersectionPoint, float &t) const;

    // Gets the intersection between this plane and the given line.
    Vector3 Intersect(const Line &line, float &t) const;

    // Returns the distance from the plane to the given point. This assumes the normal is of unit length.
    float Distance(const Vector3 &point) const;

    // Returns the position of the given point when projected onto the plane.
    Vector3 Project(const Vector3 &point) const;

    // Dot Product
    float Dot(const Vector4 &v) const;
    float DotCoord(const Vector3 &v) const;
    float DotNormal(const Vector3 &v) const;

    // Set the plane
    Plane &Set(const Vector3 &normal, float distance);

    // Derive the plane
    Plane &FromPointNormal(const Vector3 &point, const Vector3 &normal);
    Plane &FromPoints(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);

    // Nromalize the plane
    Plane &Normalize();

    // Scale the plane
    Plane &Scale(float s);

    // Transform the plane
    Plane &Transform(const Matrix &m);

    // Print the plane
    String ToString() const
    {
        StringStream ss;
        ss.precision(2);
        ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
        ss << "[" << (*this)[0] + 0.001f << ", " << (*this)[1] + 0.001f << ", " << (*this)[2] + 0.001f << ", " << (*this)[3] + 0.001f << "]";
        return ss.str();
    }
};

#endif // __MATH_PLANE_H_

