
#include "Type.h"
#include "Plane.h"
#include "Matrix.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Ray.h"
#include "Line.h"
#include "Sphere.h"
#include "Box.h"

Plane::Plane() : D3DXPLANE() {}
Plane::Plane(const float *f) : D3DXPLANE(f) {}
Plane::Plane(float a, float b, float c, float d) : D3DXPLANE(a, b, c, d) {}

Plane::Plane(const Vector3 &normal, float d)
    : D3DXPLANE(normal[0], normal[1], normal[2], d)
{
}

Plane::Plane(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
    FromPoints(v1, v2, v3);
}

Plane &Plane::Set(const Vector3 &normal, float distance)
{
    a = normal[0];
    b = normal[1];
    c = normal[2];
    d = distance;

    return *this;
}

const Vector3 &Plane::GetNormal() const
{
    return (*((Vector3 *)(&a)));
}

void Plane::SetNormal(const Vector3 &normal)
{
    a = normal[0];
    b = normal[1];
    c = normal[2];
}

float Plane::GetDistance() const
{
    return d;
}

void Plane::SetDistance(float distance)
{
    d = distance;
}

Vector3 Plane::GetPoint() const
{
    return GetNormal() * d;
}

Plane::Classification Plane::WhichSide(const Vector3 &p) const
{
    float test = GetNormal().Dot(p) - d;
    if (test > FLOAT_EPSILON)
        return FRONT;
    else if (test < -FLOAT_EPSILON)
        return BACK;
    else
        return COPLANAR;
}

Plane::Classification Plane::WhichSide(const Vector3 &p, float threshold) const
{
    float test = GetNormal().Dot(p) - d;
    if (test > threshold)
        return FRONT;
    else if (test < -threshold)
        return BACK;
    else
        return COPLANAR;
}

Plane::Classification Plane::WhichSide(const Box &box) const
{
    Vector3 box_corners[8];
    box.CalcCorners(box_corners);

    return WhichSide(8, box_corners);
}

Plane::Classification Plane::WhichSide(int num_points, const Vector3 *points) const
{
    bool front = false, back = false;

    for (int i = 0; i < num_points; i++)
    {
        switch (WhichSide(points[i]))
        {
        case FRONT: front = true; break;
        case BACK:  back = true; break;
        }
    }

    if (front)
        return back ? COINCIDENT : FRONT;
    else
        return back ? BACK : COPLANAR;
}

Plane::Classification Plane::WhichSide(const Sphere &sphere) const
{
    float test = GetNormal().Dot(sphere.center) - d;
    if (test >= 0)
        return (test > sphere.radius) ? FRONT : COINCIDENT;
    else
        return (-test > sphere.radius) ? BACK : COINCIDENT;
}

bool Plane::IsBehind(const Sphere &sphere) const
{
    return (d - GetNormal().Dot(sphere.center)) > sphere.radius;
}

bool Plane::IsInFront(const Sphere &sphere) const
{
    return (GetNormal().Dot(sphere.center) - d) > sphere.radius;
}

Line Plane::Intersect(const Plane &plane) const
{
    Line intercept, temp;
    Vector3 normal(*((Vector3 *)(&a)));

    intercept.SetDirection(normal.Cross(plane.GetNormal()));

    Vector3 interceptDir(intercept.GetDirection());
    temp.SetDirection(interceptDir.Cross(plane.GetNormal()));

    temp.SetPosition(plane.GetPoint());
    intercept.SetPosition(Intersect(temp));
    return intercept;
}

Vector3 Plane::Intersect(const Line &line) const
{
    float t = (d - line.GetPosition().Dot(GetNormal())) / GetNormal().Dot(line.GetDirection());
    return (line.GetPosition() + t * line.GetDirection());
}

bool Plane::Intersect(bool twoSided, const Line &line, Vector3 &intersectionPoint) const
{
    float t;
    return Intersect(twoSided, line, intersectionPoint, t);
}

bool Plane::Intersect(bool twoSided, const Ray &ray, Vector3 &intersectionPoint) const
{
    float t;
    return Intersect(twoSided, ray, intersectionPoint, t);
}

bool Plane::Intersect(bool twoSided, const Line &line, Vector3 &intersectionPoint, float &t) const
{
    // Check if line does not parallel to plane
    float planeDotNorm = GetNormal().Dot(line.GetDirection());
    if (planeDotNorm <= -FLOAT_EPSILON || twoSided && planeDotNorm >= FLOAT_EPSILON)
    {
        // find the point of intersection
        t = (d - line.GetPosition().Dot(GetNormal())) / planeDotNorm;
        intersectionPoint = (line.GetPosition() + t * line.GetDirection());
        return true;
    }

    return false;
}

bool Plane::Intersect(bool twoSided, const Ray &ray, Vector3 &intersectionPoint, float &t) const
{
    // Check if ray is not parallel to plane
    float planeDotNorm = GetNormal().Dot(ray.GetDirection());
    if (planeDotNorm <= -FLOAT_EPSILON || (twoSided && planeDotNorm >= FLOAT_EPSILON))
    {
        // Find the point of intersection
        t = (d - ray.GetPosition().Dot(GetNormal())) / planeDotNorm;
        if (t >= 0.0f)
        {
            intersectionPoint = (ray.GetPosition() + t * ray.GetDirection());
            return true;
        }
    }

    return false;
}

Vector3 Plane::Intersect(const Line &line, float &t) const
{
    // find the point of intersection
    t = (d - line.GetPosition().Dot(GetNormal())) / GetNormal().Dot(line.GetDirection());
    return (line.GetPosition() + t * line.GetDirection());
}

float Plane::Distance(const Vector3 &point) const
{
    return (point.Dot(GetNormal()) - d);
}

Vector3 Plane::Project(const Vector3 &point) const
{
    // Create a line from the point and the planes normal.
    Line line(GetNormal(), point);

    // Return the intersection between that line and the plane.
    return Intersect(line);
}

float Plane::Dot(const Vector4 &v) const
{
    return D3DXPlaneDot(this, &v);
}

float Plane::DotCoord(const Vector3 &v) const
{
    return D3DXPlaneDotCoord(this, &v);
}

float Plane::DotNormal(const Vector3 &v) const
{
    return D3DXPlaneDotNormal(this, &v);
}

Plane &Plane::FromPointNormal(const Vector3 &point, const Vector3 &normal)
{
    D3DXPlaneFromPointNormal(this, &point, &normal);
    return *this;
}

Plane &Plane::FromPoints(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
    D3DXPlaneFromPoints(this, &v1, &v2, &v3);
    return *this;
}

Plane &Plane::Normalize()
{
    D3DXPlaneNormalize(this, this);
    return *this;
}

Plane &Plane::Scale(float s)
{
#if XBOX
    d *= s;
#else
    D3DXPlaneScale(this, this, s);
#endif
    return *this;
}

Plane &Plane::Transform(const Matrix &m)
{
    D3DXPlaneTransform(this, this, &m);
    return *this;
}
