
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

Vector4 Vec4Identity(0.0f, 0.0f, 0.0f, 1.0f);

Vector4::Vector4()                                   : D3DXVECTOR4() {}
Vector4::Vector4(const float *f)                     : D3DXVECTOR4(f) {}
Vector4::Vector4(float x, float y, float z, float w) : D3DXVECTOR4(x, y, z, w) {}
Vector4::Vector4(const Vector3 &vec, float w)        : D3DXVECTOR4(vec.x, vec.y, vec.z, w) {}
Vector4::Vector4(const float2 &v)                    : D3DXVECTOR4(v.x, v.y, 0, 0) {}
Vector4::Vector4(const float3 &v)                    : D3DXVECTOR4(v.x, v.y, v.z, 0) {}
Vector4::Vector4(const float4 &v)                    : D3DXVECTOR4(v.x, v.y, v.z, v.w) {}

bool Vector4::FromString(const String &str)
{
    String s = str + ")";
    String buffer;
    int  count = 0;
    float vec[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    for (uint i = 0; i < s.size(); i++)
    {
        char c = s.at(i);

        if (c == '(' || c == '[')
        {
            continue;
        }

        if (c == ']' || c == ')')
        {
            if (!buffer.empty())
            {
                vec[count] = LexicalCast<float, String>(buffer);
                count++;
            }

            if (count < 4)
                return false;

            this->x = vec[0];
            this->y = vec[1];
            this->z = vec[2];
            this->w = vec[3];

            return true;
        }

        if (c == ' ')
            continue;

        if (c == ',' || c == ';')
        {
            if (count >= 4)
                return false;

            if (buffer.empty())
                vec[count] = 0.0f;
            else
                vec[count] = LexicalCast<float, String>(buffer);
            buffer.clear();
            count++;
            continue;
        }

        buffer += c;;
    }

    return false;
}

Vector4 &Vector4::Set(float x, float y, float z, float w)
{
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
    (*this)[3] = w;
    return *this;
}

Vector4 &Vector4::Add(const Vector4 &v1, const Vector4 &v2)
{
    D3DXVec4Add(this, &v1, &v2);
    return *this;
}

Vector4 &Vector4::Scale(const Vector4 &v, float s)
{
    D3DXVec4Scale(this, &v, s);
    return *this;
}

Vector4 &Vector4::Subtract(const Vector4 &v1, const Vector4 &v2)
{
    D3DXVec4Subtract(this, &v1, &v2);
    return *this;
}

Vector4 &Vector4::Lerp(const Vector4 &v1, const Vector4 &v2, float s)
{
    D3DXVec4Lerp(this, &v1, &v2, s);
    return *this;
}

Vector4 &Vector4::Normalize(const Vector4 &v1)
{
    D3DXVec4Normalize(this, &v1);
    return *this;
}

Vector4 &Vector4::Maximize(const Vector4 &v1, const Vector4 &v2)
{
    D3DXVec4Maximize(this, &v1, &v2);
    return *this;
}

Vector4 &Vector4::Minimize(const Vector4 &v1, const Vector4 &v2)
{
    D3DXVec4Minimize(this, &v1, &v2);
    return *this;
}

float Vector4::Dot(const Vector4 &v) const
{
    return D3DXVec4Dot(this, &v);
}

float Vector4::Length() const
{
    return D3DXVec4Length(this);
}

float Vector4::LengthSq() const
{
    return D3DXVec4LengthSq(this);
}

Vector4 &Vector4::BaryCentric(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3, float f, float g)
{
    D3DXVec4BaryCentric(this, &v1, &v2, &v3, f, g);
    return *this;
}

Vector4 &Vector4::CatmullRom(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3, const Vector4 &v4, float s)
{
    D3DXVec4CatmullRom(this, &v1, &v2, &v3, &v4, s);
    return *this;
}

Vector4 &Vector4::Hermite(const Vector4 &v1, const Vector4 &t1, const Vector4 &v2, const Vector4 &t2, float s)
{
    D3DXVec4Hermite(this, &v1, &t1, &v2, &t2, s);
    return *this;
}

Vector4 &Vector4::Cross(const Vector4 &v1, const Vector4 &v2, const Vector4 &v3)
{
    D3DXVec4Cross(this, &v1, &v2, &v3);
    return *this;
}

Vector4 &Vector4::Transform(const Vector2 &v, const Matrix &m)
{
    D3DXVec2Transform(this, &v, &m);
    return *this;
}

Vector4 &Vector4::Transform(const Vector3 &v, const Matrix &m)
{
    D3DXVec3Transform(this, &v, &m);
    return *this;
}

Vector4 &Vector4::Transform(const Vector4 &v, const Matrix &m)
{
    D3DXVec4Transform(this, &v, &m);
    return *this;
}


