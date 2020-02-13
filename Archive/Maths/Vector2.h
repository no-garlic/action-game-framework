
#ifndef __MATH_VECTOR2_H_
#define __MATH_VECTOR2_H_

#include "Type.h"
#include "Str.h"
#include "Lexical.h"

class Matrix;

class Vector2 : public D3DXVECTOR2
{
public:
    Vector2();
    Vector2(const float *f);
    Vector2(float x, float y);
    Vector2(const float2 &v);
    Vector2(const float3 &v);
    Vector2(const float4 &v);

    // Vector length
    float Length() const;
    float LengthSq() const;

    // Set the vector
    Vector2 &Set(float x, float y);

    // Dot product
    float Dot(const Vector2 &v1) const;
    float Dot() const { return Dot(*this); }

    // Add vectors
    Vector2 &Add(const Vector2 &v1, const Vector2 &v2);
    Vector2 &Add(const Vector2 &v2) { return Add(*this, v2); }

    // Scale the vector
    Vector2 &Scale(const Vector2 &v, float s);
    Vector2 &Scale(float s) { return Scale(*this, s); }

    // Subtract another vector
    Vector2 &Subtract(const Vector2 &v1, const Vector2 &v2);
    Vector2 &Subtract(const Vector2 &v2) { return Subtract(*this, v2); }

    // Interpolate between vectors
    Vector2 &Lerp(const Vector2 &v1, const Vector2 &v2, float s);
    Vector2 &Lerp(const Vector2 &v2, float s) { return Lerp(*this, v2, s); }

    // Normalize the vector
    Vector2 &Normalize(const Vector2 &v1);
    Vector2 &Normalize() { return Normalize(*this); }

    // Maximize the vector
    Vector2 &Maximize(const Vector2 &v1, const Vector2 &v2);
    Vector2 &Maximize(const Vector2 &v2) { return Maximize(*this, v2); }

    // Minimize the vector
    Vector2 &Minimize(const Vector2 &v1, const Vector2 &v2);
    Vector2 &Minimize(const Vector2 &v2) { return Minimize(*this, v2); }

    // Transform the vector
    Vector2 &TransformCoord(const Vector2 &v, const Matrix &m);
    Vector2 &TransformCoord(const Matrix &m) { return TransformCoord(*this, m); }
    Vector2 &TransformNormal(const Vector2 &v, const Matrix &m);
    Vector2 &TransformNormal(const Matrix &m) { return TransformNormal(*this, m); }

    // Returns the z component of the cross product
    float CCW(const Vector2 &v1) const;

    // Interpolate points
    Vector2 &BaryCentric(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float f, float g);
    Vector2 &CatmullRom(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, const Vector2 &v4, float s);
    Vector2 &Hermite(const Vector2 &v1, const Vector2 &t1, const Vector2 &v2, const Vector2 &t2, float s);

    // Clamp
    Vector2 &Clamp(Vector2 &v1, float c);

    // Print the vector to a string
    String ToString() const
    {
        StringStream ss;
        ss.precision(6);
        ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
        ss << "[" << (*this)[0] + 0.0000001f << ", " << (*this)[1] + 0.0000001f << "]";
        return ss.str();
    }

    // Parse the string and set the vector
    bool FromString(const String &str);

};

#endif //__MATH_VECTOR2_H_

