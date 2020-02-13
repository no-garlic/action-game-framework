
#ifndef __MATH_BOX_H_
#define __MATH_BOX_H_

#include "Type.h"
#include "Vector3.h"

class Frustum;
class Sphere;
class Line;
class Ray;

class Box
{
public:
    Box() {}
    Box(const Vector3 &min_pt, const Vector3 max_pt) : MinPt(min_pt), MaxPt(max_pt) {}
    Box(uint num_points, const Vector3 *points);

    // Get the extents of the box
    const Vector3 &GetMin() const { return MinPt; }
    const Vector3 &GetMax() const { return MaxPt; }

    // Set the extents of the box
    void SetMin(const Vector3 &min_pt) { MinPt = min_pt; }
    void SetMax(const Vector3 &max_pt) { MaxPt = max_pt; }
    void SetHeights(float h_min, float h_max) { MinPt.z = h_min; MaxPt.z = h_max; }

    // Get the center of the box
    Vector3 GetCenter() const;
    Vector3 GetSize() const;

    // Get the size of the box
    float GetWidth()  const { return MaxPt.x - MinPt.x; }
    float GetHeight() const { return MaxPt.y - MinPt.y; }
    float GetDepth()  const { return MaxPt.z - MinPt.z; }

    // Clamp the point to the extents of the box
    Vector3 Clamp(const Vector3 &point) const;

    // Maximize of minimize the box
    void Maximize(const Box &box);
    void Minimize(const Box &box);

    // Test if the item is within hte box
    bool IsWithin(const Vector3 &point) const;
    bool IsWithin(const Box &box) const;
    bool IsWithin(const Frustum &frustum) const;

    // Calculate the corners of the box (must supply a Vector3[8] array)
    void CalcCorners(Vector3 *vertex) const;

    // Test for a collision with the box
    bool TestCollision(const Sphere &sphere) const;
    bool TestCollision(const Box &box) const;
    bool TestCollision(const Line &line) const;
    bool TestCollision(const Ray &ray) const;

    // Create a zero size box at the origin
    void Zero();
    bool IsEmpty() const;

    // Get the min and max collision points of the box with the line
    bool GetMinMaxCollision(const Line &line, float &min_t, float &max_t) const;

    // The min and max extents of the box
    Vector3 MinPt;
    Vector3 MaxPt;

};

#endif // __MATH_BOX_H_

