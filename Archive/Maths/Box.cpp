
#include "Box.h"
#include "Vector3.h"
#include "Frustum.h"
#include "Sphere.h"
#include "Line.h"
#include "Ray.h"

Box::Box(uint num_points, const Vector3 *points)
{
    if (num_points == 0)
        return;

    MinPt = points[0];
    MaxPt = points[0];

    for (uint i = 1; i < num_points; i++)
    {
        MinPt.x = Min<float>(MinPt.x, points[i].x);
        MinPt.y = Min<float>(MinPt.y, points[i].y);
        MinPt.z = Min<float>(MinPt.z, points[i].z);
        MaxPt.x = Max<float>(MaxPt.x, points[i].x);
        MaxPt.y = Max<float>(MaxPt.y, points[i].y);
        MaxPt.z = Max<float>(MaxPt.z, points[i].z);
    }
}

Vector3 Box::GetCenter() const
{
    return Vector3(MinPt + MaxPt) * 0.5f;
}

Vector3 Box::GetSize() const
{
    return MaxPt - MinPt;
}

Vector3 Box::Clamp(const Vector3 &point) const
{
    Vector3 p(point);

    ::Clamp<float>(p.x, MinPt.x, MaxPt.x);
    ::Clamp<float>(p.y, MinPt.y, MaxPt.y);
    ::Clamp<float>(p.z, MinPt.z, MaxPt.z);

    return p;
}

void Box::Maximize(const Box &box)
{
    MinPt.x = Min<float>(MinPt.x, box.MinPt.x);
    MinPt.y = Min<float>(MinPt.y, box.MinPt.y);
    MinPt.z = Min<float>(MinPt.z, box.MinPt.z);
    MaxPt.x = Max<float>(MaxPt.x, box.MaxPt.x);
    MaxPt.y = Max<float>(MaxPt.y, box.MaxPt.y);
    MaxPt.z = Max<float>(MaxPt.z, box.MaxPt.z);
}

void Box::Minimize(const Box &box)
{
    MinPt.x = Max<float>(MinPt.x, box.MinPt.x);
    MinPt.y = Max<float>(MinPt.y, box.MinPt.y);
    MinPt.z = Max<float>(MinPt.z, box.MinPt.z);
    MaxPt.x = Min<float>(MaxPt.x, box.MaxPt.x);
    MaxPt.y = Min<float>(MaxPt.y, box.MaxPt.y);
    MaxPt.z = Min<float>(MaxPt.z, box.MaxPt.z);
}

bool Box::IsWithin(const Vector3 &point) const
{
    return (point.x >= MinPt.x && point.x <= MaxPt.x && point.y >= MinPt.y && point.y <= MaxPt.y && point.z >= MinPt.z && point.z <= MaxPt.z);
}

bool Box::IsWithin(const Box &box) const
{
    return ((box.MinPt.x < MaxPt.x) && (box.MinPt.y < MaxPt.y) && (box.MinPt.z < MaxPt.z)) &&
        ((box.MaxPt.x > MinPt.x) && (box.MaxPt.y > MinPt.y) && (box.MaxPt.z > MinPt.z));
}

bool Box::IsWithin(const Frustum &frustum) const
{
    const float *boxExtents = &MinPt[0];
    const Plane *planes = frustum.GetPlanes();
    bool intersects = false;

    for (int f = 0; ; ++f)
    {
        if (f == 6)
            return true;

        const Vector3 &planeNormal = planes[f].GetNormal();

        // Use corner of aabb closest to the plane to test
        const Vector3 nvertex(boxExtents[planeNormal.x >= 0 ? Frustum::TOP : Frustum::FRONT],
            boxExtents[planeNormal.y >= 0 ? Frustum::BOTTOM : Frustum::LEFT],
            boxExtents[planeNormal.z >= 0 ? Frustum::BACK : Frustum::RIGHT]);

        // is point on back side of the plane?
        if (planes[f].GetNormal().Dot(nvertex) < planes[f].GetDistance())
        {
            // whole box is outside the frustum
            return false;
        }

        // if already overlapping, just continue checking for possible back side of a plane
        if (intersects)
            continue;

        // use corner of aabb farthest from the plane to test
        const Vector3 pvertex(boxExtents[planeNormal.x >= 0 ? Frustum::FRONT : Frustum::TOP],
            boxExtents[planeNormal.y >= 0 ? Frustum::LEFT : Frustum::BOTTOM],
            boxExtents[planeNormal.z >= 0 ? Frustum::RIGHT : Frustum::BACK]);

        // if point on back side of the plane, then it must be overlapping
        if (planes[f].GetNormal().Dot(pvertex) < planes[f].GetDistance())
            intersects = true;

        // possible front side box, continue testing other planes
    }

    return true;
}

void Box::CalcCorners(Vector3 *vertex) const
{
    vertex[0] = MinPt;
    vertex[1] = Vector3(MaxPt.x, MinPt.y, MinPt.z);
    vertex[2] = Vector3(MaxPt.x, MaxPt.y, MinPt.z);
    vertex[3] = Vector3(MinPt.x, MaxPt.y, MinPt.z);
    vertex[4] = Vector3(MinPt.x, MinPt.y, MaxPt.z);
    vertex[5] = Vector3(MaxPt.x, MinPt.y, MaxPt.z);
    vertex[6] = MaxPt;
    vertex[7] = Vector3(MinPt.x, MaxPt.y, MaxPt.z);
}

bool Box::TestCollision(const Sphere &sphere) const
{
    const Vector3 boxCenter = this->GetCenter();
    const Vector3 sphereCenter = sphere.GetCenter();

    float sphereRadius = sphere.GetRadius();
    float s, d = 0;

    if (sphereCenter.x < MinPt.x)
    {
        s = sphereCenter.x - MinPt.x;
        d += s*s;
    }
    else if (sphereCenter.x > MaxPt.x)
    {
        s = sphereCenter.x - MaxPt.x;
        d += s*s;
    }

    if (sphereCenter.y < MinPt.y)
    {
        s = sphereCenter.y - MinPt.y;
        d += s*s;
    }
    else if (sphereCenter.y > MaxPt.y)
    {
        s = sphereCenter.y - MaxPt.y;
        d += s*s;
    }

    if (sphereCenter.z < MinPt.z)
    {
        s = sphereCenter.z - MinPt.z;
        d += s*s;
    }
    else if (sphereCenter.z > MaxPt.z)
    {
        s = sphereCenter.z - MaxPt.z;
        d += s*s;
    }

    return d <= sphereRadius * sphereRadius;
}

bool Box::TestCollision(const Box &box) const
{
    return (box.MaxPt.x >= MinPt.x && box.MinPt.x <= MaxPt.x &&
        box.MaxPt.y >= MinPt.y && box.MinPt.y <= MaxPt.y &&
        box.MaxPt.z >= MinPt.z && box.MinPt.z <= MaxPt.z);
}

bool Box::TestCollision(const Line &line) const
{
    float min_t, max_t;
    return GetMinMaxCollision(line, min_t, max_t);
}

bool Box::TestCollision(const Ray &ray) const
{
    float min_t, max_t;
    if (GetMinMaxCollision(ray, min_t, max_t))
    {
        // Test max against zero for a ray
        if (max_t < 0.0f)
            return false;

        return true;
    }

    return false;
}

void Box::Zero()
{
    MinPt.Set(0.0f, 0.0f, 0.0f);
    MaxPt.Set(0.0f, 0.0f, 0.0f);
}

bool Box::IsEmpty() const
{
    return (MinPt.x == MaxPt.x && MinPt.y == MaxPt.y && MinPt.z == MaxPt.z);
}

bool Box::GetMinMaxCollision(const Line &line, float &min_t, float &max_t) const
{
    // Get the new min/max limits
    Vector3 localMin = MinPt - line.GetPosition();
    Vector3 localMax = MaxPt - line.GetPosition();

    const Vector3 &lineDir = line.GetDirection();

    // Test X coordinate
    if (lineDir.x != 0.0f)
    {
        // Get the two collision points
        float testT1 = localMin.x / lineDir.x;
        float testT2 = localMax.x / lineDir.x;

        if (testT2 > testT1)
        {
            min_t = testT1;
            max_t = testT2;
        }
        else
        {
            min_t = testT2;
            max_t = testT1;
        }
    }
    else
    {
        // Test if no intersection on the x axis occurs
        if ((localMin.x > 0) == (localMax.x > 0))
        {
            return false;
        }

        // Assign a large value to the maximum time
        min_t = FLOAT_MIN;
        max_t = FLOAT_MAX;
    }

    // Test Y coordinate
    if (lineDir.y != 0.0f)
    {
        //Get the two collision points
        float testT1 = localMin.y / lineDir.y;
        float testT2 = localMax.y / lineDir.y;

        if (testT2 > testT1)
        {
            min_t = Max<float>(testT1, min_t);
            max_t = Min<float>(testT2, max_t);
        }
        else
        {
            min_t = Max<float>(testT2, min_t);
            max_t = Min<float>(testT1, max_t);
        }
    }
    else
    {
        // Test if no intersection on the y axis occurs
        if ((localMin.y > 0) == (localMax.y > 0))
        {
            return false;
        }

        // Don't change the min/max times
    }

    // Test Z coordinate
    if (lineDir.z != 0.0f)
    {
        //Get the two collision points
        float testT1 = localMin.z / lineDir.z;
        float testT2 = localMax.z / lineDir.z;

        if (testT2 > testT1)
        {
            min_t = Max<float>(testT1, min_t);
            max_t = Min<float>(testT2, max_t);
        }
        else
        {
            min_t = Max<float>(testT2, min_t);
            max_t = Min<float>(testT1, max_t);
        }
    }
    else
    {
        // Test if no intersection on the z axis occurs
        if ((localMin.z > 0) == (localMax.z > 0))
        {
            return false;
        }

        // Don't change the min/max times
    }

    // If the max is less than the min, no collision occured
    if (max_t < min_t)
    {
        return false;
    }

    // If the min an max are at the limits, the direction must have 
    // been a zero vector
    if (min_t == FLOAT_MIN && max_t == FLOAT_MAX)
    {
        min_t = 0.0f;
        max_t = 0.0f;
    }

    return true;
}

