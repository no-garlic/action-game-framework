
#ifndef __MATH_FRUSTUM_H_
#define __MATH_FRUSTUM_H_

#include "Type.h"
#include "Matrix.h"
#include "Plane.h"

class Sphere;
class Matrix;
class Box;

class Frustum
{
public:
    Frustum();

    enum FrustumPlane
    {
        FRONT   = 0,    // The FRONT plane of the frustum
        LEFT    = 1,    // The LEFT  plane of the frustum
        RIGHT   = 2,    // The RIGHT plane of the frustum
        TOP     = 3,    // The TOP plane of the frustum
        BOTTOM  = 4,    // The BOTTOM plane of the frustum
        BACK    = 5     // The BACK plane of the frustum
    };

    enum FrustumIntersect
    {
        IN_FRUSTUM  = 0,    // The object is totally within the frustum
        OUT_FRUSTUM = 1,    // The object is totally out of the frustum
        PARTIAL     = 2     // The object intersects one of the planes
    };

    // Get the planes array from the frustum
    Plane *GetPlanes();
    const Plane *GetPlanes() const;

    // Collision tests
    virtual int Intersects(const Sphere &sphere, const Matrix &world_matrix) const;
    virtual int Intersects(const Box &box, const Matrix &world_matrix) const;
    virtual int Intersects(const Sphere &sphere) const;
    virtual int Intersects(const Box &box) const;

    // Create a string debugging the frustum planes
    virtual String ToString() const;

protected:
    virtual void RecalculateFrustum() const = 0;

    mutable Plane m_planes[6];    // The planes of the frustum
    mutable bool  m_dirty;        // Have the matricies changed

};

#endif // __MATH_FRUSTUM_H_

