
#ifndef __MATH_VECTOR4_H_
#define __MATH_VECTOR4_H_

#include "Type.h"
#include "Str.h"
#include "Lexical.h"

class Vector2;
class Vector3;
class Vector4;
class Matrix;

extern Vector4 Vec4Identity;

class Vector4 : public D3DXVECTOR4
{
public:
    Vector4();
    Vector4(const float *f);
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector3 &vec, float w);
    Vector4(const float2 &v);
    Vector4(const float3 &v);
    Vector4(const float4 &v);

    // Length
    float Length() const;
    float LengthSq()const;

    // Set the vector
    Vector4 &Set(float x, float y, float z, float w);

    // Dot Product
    float Dot(const Vector4 &v) const;
    float Dot() const { return Dot(*this); }

    // Add vectors
    Vector4 &Add(const Vector4 &v1, const Vector4 &v2);
    Vector4 &Add(const Vector4 &v2) { return Add(*this, v2); }

    // Scale teh vector
    Vector4 &Scale(const Vector4 &v, float s);
    Vector4 &Scale(float s) { return Scale(*this, s); }

    // subtract vectors
    Vector4 &Subtract(const Vector4 &v1, const Vector4 &v2);
    Vector4 &Subtract(const Vector4 &v2) { return Subtract(*this, v2); }

    // Interpolate vectors
    Vector4 &Lerp(const Vector4 &v1, const Vector4 &v2, float s);
    Vector4 &Lerp(const Vector4 &v2, float s) { return Lerp(*this, v2, s); }

    // Normalize vectors
    Vector4 &Normalize(const Vector4 &v1);
    Vector4 &Normalize() { return Normalize(*this); }

    // Maximize the vector
    Vector4 &Maximize(const Vector4 &v1, const Vector4 &v2);
    Vector4 &Maximize(const Vector4 &v2) { return Maximize(*this, v2); }

    // Minimize the vector
    Vector4 &Minimize(const Vector4 &v1, const Vector4 &v2);
    Vector4 &Minimize(const Vector4 &v2) { return Minimize(*this, v2); }

    // Cross Product
    Vector4 &Cross(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3);
    Vector4 &Cross(const Vector4 &v2, const Vector4 &v3) { return Cross(*this, v2, v3); }

    // Transformation
    Vector4 &Transform(const Vector2 &v, const Matrix &m);
    Vector4 &Transform(const Vector3 &v, const Matrix &m);
    Vector4 &Transform(const Vector4 &v, const Matrix &m);

    // Interpolation
    Vector4 &BaryCentric(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3, float f, float g);
    Vector4 &CatmullRom(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3, const Vector4 &v4, float s);
    Vector4 &Hermite(const Vector4 &v1, const Vector4 &t1, const Vector4 &v2, const Vector4 &t2, float s);

    // Parse the string and set the vector
    bool FromString(const String &str);

    // Print the vector to a string
    inline String ToString() const
    {
        StringStream ss;
        ss.precision(6);
        ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
        ss << "[" << (*this)[0] + 0.0000001f << ", " << (*this)[1] + 0.0000001f << ", " << (*this)[2] + 0.0000001f << ", " << (*this)[3] + 0.0000001f << "]";
        return ss.str();
    }
};

#endif // __MATH_VECTOR4_H_

