
#include "Sphere.h"
#include "Frustum.h"
#include "Box.h"
#include "Line.h"
#include "LineSegment.h"
#include "Ray.h"

void Sphere::Maximize(const Sphere &s1, const Sphere &s2)
{
    Vector3 vec;
    D3DXVec3Subtract(&vec, &(s1.center), &(s2.center));

    float vec_len = D3DXVec3Length(&vec);

    if (vec_len < 0.001f && vec_len > -0.001f)
    {
        radius = Max<float>(s1.radius, s2.radius);
        center = s1.center;
        return;
    }

    float rad = (s1.radius + s2.radius + vec_len) / 2.0f;
    if (rad < s1.radius)
    {
        (*this) = s1;
        return;
    }
    else if (rad < s2.radius)
    {
        (*this) = s2;
        return;
    }

    float lerp = (rad - s1.radius) / vec_len;
    D3DXVec3Lerp(&vec, &(s1.center), &(s2.center), lerp);

    center = vec;
    radius = rad;
}

void Sphere::TransformAndScale(const Matrix &matrix)
{
    center.Add(matrix.GetTAxis());
    radius *= matrix.GetScaleMax();
}

bool Sphere::CheckVisibility(const Frustum &frustum) const
{
    const Plane *planes = frustum.GetPlanes();

    return
        (
        (!planes[0].IsBehind(*this)) &&
            (!planes[1].IsBehind(*this)) &&
            (!planes[2].IsBehind(*this)) &&
            (!planes[3].IsBehind(*this)) &&
            (!planes[4].IsBehind(*this)) &&
            (!planes[5].IsBehind(*this))
            );
}

bool Sphere::TestCollision(const Vector3 &point) const
{
    return ((Vector3(point - center).LengthSq() - radius * radius) < 1.0e-4f * 1.0e-4f);
}

bool Sphere::TestCollision(const Plane &plane) const
{
    return ((fabs(plane.Distance(center)) - radius) < 1.0e-4f);
}

bool Sphere::TestCollision(const Box &box) const
{
    return box.TestCollision(*this);
}

bool Sphere::TestCollision(const Sphere &sphere) const
{
    return ((Vector3(center - sphere.center).LengthSq() - (radius + sphere.radius) * (radius + sphere.radius)) < 1.0e-4f);
}

bool Sphere::TestCollision(const Line &line) const
{
    return ((line.DistanceSquared(center) - radius * radius) < 1.0e-4f);
}

bool Sphere::TestCollision(const LineSegment &lineSegment) const
{
    return ((lineSegment.DistanceSquared(center) - radius * radius) < 1.0e-4f);
}

bool Sphere::TestCollision(const Ray &ray) const
{
#if XBOX
    return (D3DXSphereBoundProbe(&center, radius, const_cast<Vector3*>(&ray.position), const_cast<Vector3*>(&ray.direction)) == TRUE);
#else
    return (D3DXSphereBoundProbe(&center, radius, &ray.position, &ray.direction) == TRUE);
#endif
}

