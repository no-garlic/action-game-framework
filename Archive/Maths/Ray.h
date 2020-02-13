
#ifndef __MATH_RAY_H_
#define __MATH_RAY_H_

#include "Type.h"
#include "Line.h"
#include "Vector3.h"
#include "Matrix.h"

struct Triangle
{
    Vector3 p0;
    Vector3 p1;
    Vector3 p2;

    String ToString()
    {
        StringStream ss;
        ss.precision(2);
        ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
        ss << "p0: " << p0.ToString() << "p1: " << p1.ToString() << "p2: " << p2.ToString();
        return ss.str();
    }
};

class Ray : public Line
{
public:
    Ray() {}
    Ray(const Vector3 &d, const Vector3 &p) : Line(d, p) {}
    Ray(const Vector3 &d, const Vector3 &p, const Matrix &m);
    Ray(const Ray &ray, const Matrix &m);

    inline bool IntersectsTriangle(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2, float &u, float &v, float &t) const
    {
        // find vectors for two edges sharing p0 (using cw winding).  To
        // change winding order, swap edge1 nad edge2.
        Vector3 edge1 = p1 - p0;
        Vector3 edge2 = p2 - p0;

        // begin calculating determinant - also used to calculate U parameter
        Vector3 pvec;
        pvec.Cross(direction, edge2);

        // if determinant is near zero, ray lies in plane of triangle
        float det = edge1.Dot(pvec);
        if (det < FLOAT_EPSILON)
            return false;

        // calculate distance from p0 to ray origin
        Vector3 tvec = position - p0;

        // calculate U parameter and test bounds
        u = tvec.Dot(pvec);
        if (u < 0.0 || u > det)
            return false;

        // prepare to test V parameter
        Vector3 qvec;
        qvec.Cross(tvec, edge1);

        // calculate V parameter and test bounds
        v = (const_cast<Vector3 &>(direction)).Dot(qvec);
        if (v < 0.0 || u + v > det)
            return false;

        // calculate t, scale parameters, ray intersects triangle
        t = edge2.Dot(qvec);
        float inv_det = 1.0f / det;
        t *= inv_det;
        u *= inv_det;
        v *= inv_det;

        // test if t is within the ray boundary (when t >= 0)
        return t >= 0.0f;
    }

    inline bool IntersectsTriangle(const Vector3 &p0, const Vector3 &p1, const Vector3 &p2,
        Vector3 &point, Vector3 &bary, float &distance) const
    {
        // Find vectors for two edges sharing p0
        Vector3 edge0 = p1 - p0;
        Vector3 edge1 = p2 - p0;

        // Begin calculating determinant - also used to calculate U parameter
        Vector3 pvec;
        pvec.Cross(direction, edge1);

        // If determinant is near zero, ray lies in plane of triangle
        float det = edge0.Dot(pvec);
        if (det < FLOAT_EPSILON)
            return false;

        // Calculate distance from p0 to ray origin
        Vector3 tvec = position - p0;

        // Calculate U parameter and test bounds
        bary.y = tvec.Dot(pvec);
        if (bary.y < 0.0f || bary.y > det)
            return false;

        // Prepare to test V parameter
        Vector3 qvec = tvec.Cross(edge0);

        // Calculate V parameter and test bounds
        bary.z = direction.Dot(qvec);
        if (bary.z < 0.0f || bary.y + bary.z > det)
            return false;

        // Calculate t, scale parameters, ray intersects triangle
        distance = edge1.Dot(qvec);
        float inv_det = 1.0f / det;
        distance *= inv_det;
        bary.y *= inv_det;
        bary.z *= inv_det;
        bary.x = 1.0f - (bary.y + bary.z);
        point = position + direction * distance;

        return true;
    }

    inline bool IntersectsTriangle(const Triangle &tri, Vector3 &point, Vector3 &bary, float &distance) const
    {
        return IntersectsTriangle(tri.p0, tri.p1, tri.p2, point, bary, distance);
    }

    inline bool IntersectsTriangle(const Triangle &tri, float &u, float &v, float &t) const
    {
        return IntersectsTriangle(tri.p0, tri.p1, tri.p2, u, v, t);
    }
};

#endif // __MATH_RAY_H_

