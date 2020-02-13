
#ifndef __MATH_VECTOR3_H_
#define __MATH_VECTOR3_H_

#include "Type.h"
#include "Str.h"
#include "Lexical.h"

class Vector2;
class Vector4;
class Plane;
class Matrix;

inline D3DXVECTOR3 *D3DXVec3Set(D3DXVECTOR3 *pVec, float x, float y, float z)
{
    (*pVec)[0] = x;
    (*pVec)[1] = y;
    (*pVec)[2] = z;
    return pVec;
}

class Vector3 : public D3DXVECTOR3
{
public:
    Vector3();
    Vector3(const float *f);
    Vector3(const D3DVECTOR &v);
    Vector3(float x, float y, float z);
    Vector3(const float2 &v);
    Vector3(const float3 &v);
    Vector3(const float4 &v);

    // Dot product
    float Dot(const Vector3 &v) const;
    float Dot() const { return Dot(*this); }

    // Length
    float Length() const;
    float LengthSq() const;
    Vector3 &Set(float x, float y, float z);

    // Add vectors
    Vector3 &Add(const Vector3 &v1, const Vector3 &v2);
    Vector3 &Add(const Vector3 &v2) { return Add(*this, v2); }

    // Scale the vector
    Vector3 &Scale(const Vector3 &v, float s);
    Vector3 &Scale(float s) { return Scale(*this, s); }

    // Subtract vectors
    Vector3 &Subtract(const Vector3 &v1, const Vector3 &v2);
    Vector3 &Subtract(const Vector3 &v2) { return Subtract(*this, v2); }

    // Interpolate
    Vector3 &Lerp(const Vector3 &v1, const Vector3 &v2, float s);
    Vector3 &Lerp(const Vector3 &v2, float s) { return Lerp(*this, v2, s); }

    // Normalize
    Vector3 &Normalize(const Vector3 &v1);
    Vector3 &Normalize() { return Normalize(*this); }

    // Maximize
    Vector3 &Maximize(const Vector3 &v1, const Vector3 &v2);
    Vector3 &Maximize(const Vector3 &v2) { return Maximize(*this, v2); }

    // Minimize
    Vector3 &Minimize(const Vector3 &v1, const Vector3 &v2);
    Vector3 &Minimize(const Vector3 &v2) { return Minimize(*this, v2); }

    // Cross Product
    Vector3 &Cross(const Vector3 &v1, const Vector3 &v2);
    Vector3 &Cross(const Vector3 &v2) { return Cross(*this, v2); }

    // Transform the vector
    Vector3 &TransformCoord(const Vector3 &v, const Matrix &m);
    Vector3 &TransformCoord(const Matrix &m) { return TransformCoord(*this, m); }

    // Transform the normal
    Vector3 &TransformNormal(const Vector3 &v, const Matrix &m);
    Vector3 &TransformNormal(const Matrix &m) { return TransformNormal(*this, m); }

    // Test intersection between a plane and a line
    Vector3 &PlaneIntersectLine(const Plane &p, const Vector3 &v1, const Vector3 &v2);

    // Interpolate
    Vector3 &BaryCentric(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, float f, float g);
    Vector3 &CatmullRom(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, float s);
    Vector3 &Hermite(const Vector3 &v1, const Vector3 &t1, const Vector3 &v2, const Vector3 &t2, float s);

    // Project and Unproject
    Vector3 &Project(const Vector3 &v, VIEWPORT *viewport, const Matrix &projection,
        const Matrix &view, const Matrix &world);
    Vector3 &Unproject(const Vector3 &v, VIEWPORT *viewport, const Matrix &projection,
        const Matrix &view, const Matrix &world);

    // Clamp
    Vector3 &Clamp(Vector3 &v1, float c);

    Vector3 &MatrixToEulerRH(const Matrix &m);
    Vector3 &MatrixToEulerLH(const Matrix &m);

    // Make the vector by parsing a string
    bool FromString(const String &str);

    // Print the vector to a string
    String ToString() const;

};

// Create a vec3 and normalize it
inline Vector3 Vec3N(float x, float y, float z)
{
    Vector3 v(x, y, z);
    v.Normalize();
    return v;
}

#endif //__MATH_VECTOR3_H_

