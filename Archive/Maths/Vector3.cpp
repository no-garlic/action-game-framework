
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Plane.h"
#include "Matrix.h"

Vector3::Vector3()                          : D3DXVECTOR3() {}
Vector3::Vector3(const float *f)            : D3DXVECTOR3(f) {}
Vector3::Vector3(const D3DVECTOR &v)        : D3DXVECTOR3(v) {}
Vector3::Vector3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}
Vector3::Vector3(const float2 &v)           : D3DXVECTOR3(v.x, v.y, 0) {}
Vector3::Vector3(const float3 &v)           : D3DXVECTOR3(v.x, v.y, v.z) {}
Vector3::Vector3(const float4 &v)           : D3DXVECTOR3(v.x, v.y, v.z) {}

String Vector3::ToString() const
{
    StringStream ss;
    ss.precision(6);
    ss.setf(std::ios_base::fixed, std::ios_base::floatfield);
    ss << "[" << (*this)[0] + 0.0000001f << ", " << (*this)[1] + 0.0000001f << ", " << (*this)[2] + 0.0000001f << "]";
    return ss.str();
}

bool Vector3::FromString(const String &str)
{
    String s = str + ")";
    String buffer;
    int  count = 0;
    float vec[] = { 0.0f, 0.0f, 0.0f };

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

            if (count < 3)
                return false;

            this->x = vec[0];
            this->y = vec[1];
            this->z = vec[2];

            return true;
        }

        if (c == ' ')
            continue;

        if (c == ',' || c == ';')
        {
            if (count >= 3)
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

Vector3 &Vector3::Clamp(Vector3 &v1, float c)
{
    (*this)[0] = Min<float>(c, v1[0]);
    (*this)[1] = Min<float>(c, v1[1]);
    (*this)[2] = Min<float>(c, v1[2]);
    return *this;
}

Vector3 &Vector3::Set(float x, float y, float z)
{
    (*this)[0] = x;
    (*this)[1] = y;
    (*this)[2] = z;
    return *this;
}

Vector3 &Vector3::Add(const Vector3 &v1, const Vector3 &v2)
{
    D3DXVec3Add(this, &v1, &v2);
    return *this;
}

Vector3 &Vector3::Scale(const Vector3 &v, float s)
{
    D3DXVec3Scale(this, &v, s);
    return *this;
}

Vector3 &Vector3::Subtract(const Vector3 &v1, const Vector3 &v2)
{
    D3DXVec3Subtract(this, &v1, &v2);
    return *this;
}

Vector3 &Vector3::Lerp(const Vector3 &v1, const Vector3 &v2, float s)
{
    D3DXVec3Lerp(this, &v1, &v2, s);
    return *this;
}

Vector3 &Vector3::Normalize(const Vector3 &v1)
{
    D3DXVec3Normalize(this, &v1);
    return *this;
}

Vector3 &Vector3::Maximize(const Vector3 &v1, const Vector3 &v2)
{
    D3DXVec3Maximize(this, &v1, &v2);
    return *this;
}

Vector3 &Vector3::Minimize(const Vector3 &v1, const Vector3 &v2)
{
    D3DXVec3Minimize(this, &v1, &v2);
    return *this;
}

float Vector3::Dot(const Vector3 &v) const
{
    return D3DXVec3Dot(this, &v);
}

float Vector3::Length() const
{
    return D3DXVec3Length(this);
}

float Vector3::LengthSq() const
{
    return D3DXVec3LengthSq(this);
}

//       vectors

Vector3 &Vector3::BaryCentric(const Vector3 &v1, const Vector3 &v2,
    const Vector3 &v3, float f, float g)
{
    D3DXVec3BaryCentric(this, &v1, &v2, &v3, f, g);
    return *this;
}

Vector3 &Vector3::CatmullRom(const Vector3 &v1, const Vector3 &v2,
    const Vector3 &v3, const Vector3 &v4, float s)
{
    D3DXVec3CatmullRom(this, &v1, &v2, &v3, &v4, s);
    return *this;
}

Vector3 &Vector3::Hermite(const Vector3 &v1, const Vector3 &t1, const Vector3 &v2,
    const Vector3 &t2, float s)
{
    D3DXVec3Hermite(this, &v1, &t1, &v2, &t2, s);
    return *this;
}

Vector3 &Vector3::Cross(const Vector3 &v1, const Vector3 &v2)
{
    D3DXVec3Cross(this, &v1, &v2);
    return *this;
}

Vector3 &Vector3::PlaneIntersectLine(const Plane &p, const Vector3 &v1, const Vector3 &v2)
{
    // TODO
    // This may break - D3DX function can set p1 as NULL
    D3DXPlaneIntersectLine(this, &p, &v1, &v2);
    return *this;
}

//       into w = 1.

Vector3 &Vector3::TransformCoord(const Vector3 &v, const Matrix &m)
{
    D3DXVec3TransformCoord(this, &v, &m);
    return *this;
}

Vector3 &Vector3::TransformNormal(const Vector3 &v, const Matrix &m)
{
    D3DXVec3TransformNormal(this, &v, &m);
    return *this;
}

Vector3 &Vector3::Project(const Vector3 &v, VIEWPORT *viewport, const Matrix &projection, const Matrix &view, const Matrix &world)
{
    D3DXVec3Project(this, &v, viewport, &projection, &view, &world);
    return *this;
}

Vector3 &Vector3::Unproject(const Vector3 &v, VIEWPORT *viewport, const Matrix &projection, const Matrix &view, const Matrix &world)
{
    D3DXVec3Unproject(this, &v, viewport, &projection, &view, &world);
    return *this;
}

Vector3 &Vector3::MatrixToEulerRH(const Matrix &m)
{
    Vector3 &euler = *this;
    if (m._21 > 0.998f)
    {
        // North pole
        euler[0] = atan2(m._13, m._33);
        euler[1] = D3DX_PI / 2;
        euler[2] = 0.0f;
        return euler;
    }
    if (m._21 < -0.998f)
    {
        // South pole
        euler[0] = atan2(m._13, m._33);
        euler[1] = -D3DX_PI / 2;
        euler[2] = 0.0f;
        return euler;
    }
    euler[0] = atan2(-m._31, m._11);
    euler[1] = asin(m._21);
    euler[2] = atan2(-m._23, m._22);
    return euler;
}

Vector3 &Vector3::MatrixToEulerLH(const Matrix &m)
{
    Vector3 &euler = *this;
    if (m._21 > 0.998f)
    {
        // North pole
        euler[0] = 0.0f;
        euler[1] = atan2(m._13, m._33);
        euler[2] = D3DX_PI / 2;
        return euler;
    }
    if (m._21 < -0.998f)
    {
        // South pole
        euler[0] = 0.0f;
        euler[1] = atan2(m._13, m._33);
        euler[2] = -D3DX_PI / 2;
        return euler;
    }
    euler[0] = -atan2(-m._23, m._22);
    euler[1] = atan2(-m._31, m._11);
    euler[2] = asin(m._21);
    return euler;
}

