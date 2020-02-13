
#include "Vector2.h"
#include "Matrix.h"

Vector2::Vector2()                  : D3DXVECTOR2() {}
Vector2::Vector2(const float *f)    : D3DXVECTOR2(f) {}
Vector2::Vector2(float x, float y)  : D3DXVECTOR2(x, y) {}
Vector2::Vector2(const float2 &v)   : D3DXVECTOR2(v.x, v.y) {}
Vector2::Vector2(const float3 &v)   : D3DXVECTOR2(v.x, v.y) {}
Vector2::Vector2(const float4 &v)   : D3DXVECTOR2(v.x, v.y) {}

bool Vector2::FromString(const String &str)
{
    String s = str + ")";
    String buffer;
    int  count = 0;
    float vec[] = { 0.0f, 0.0f };

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

            if (count < 2)
                return false;

            this->x = vec[0];
            this->y = vec[1];

            return true;
        }

        if (c == ' ')
            continue;

        if (c == ',' || c == ';')
        {
            if (count >= 2)
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

Vector2 &Vector2::Clamp(Vector2 &v1, float c)
{
    (*this)[0] = Min<float>(c, v1[0]);
    (*this)[1] = Min<float>(c, v1[1]);
    return *this;
}

Vector2 &Vector2::Set(float x, float y)
{
    (*this)[0] = x;
    (*this)[1] = y;
    return *this;
}

Vector2 &Vector2::Add(const Vector2 &v1, const Vector2 &v2)
{
    D3DXVec2Add(this, &v1, &v2);
    return *this;
}

Vector2 &Vector2::Scale(const Vector2 &v, float s)
{
    D3DXVec2Scale(this, &v, s);
    return *this;
}

Vector2 &Vector2::Subtract(const Vector2 &v1, const Vector2 &v2)
{
    D3DXVec2Subtract(this, &v1, &v2);
    return *this;
}

Vector2 &Vector2::Lerp(const Vector2 &v1, const Vector2 &v2, float s)
{
    D3DXVec2Lerp(this, &v1, &v2, s);
    return *this;
}

Vector2 &Vector2::Maximize(const Vector2 &v1, const Vector2 &v2)
{
    D3DXVec2Maximize(this, &v1, &v2);
    return *this;
}

Vector2 &Vector2::Minimize(const Vector2 &v1, const Vector2 &v2)
{
    D3DXVec2Minimize(this, &v1, &v2);
    return *this;
}

float Vector2::CCW(const Vector2 &v) const
{
    return D3DXVec2CCW(this, &v);
}

float Vector2::Dot(const Vector2 &v) const
{
    return D3DXVec2Dot(this, &v);
}

float Vector2::Length() const
{
    return D3DXVec2Length(this);
}

float Vector2::LengthSq() const
{
    return D3DXVec2LengthSq(this);
}

Vector2 &Vector2::BaryCentric(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, float f, float g)
{
#if XBOX
    D3DXVec2BaryCentric(this, &v1, &v2, const_cast<Vector2*>(&v3), f, g);
#else
    D3DXVec2BaryCentric(this, &v1, &v2, &v3, f, g);
#endif
    return *this;
}

Vector2 &Vector2::CatmullRom(const Vector2 &v1, const Vector2 &v2, const Vector2 &v3, const Vector2 &v4, float s)
{
    D3DXVec2CatmullRom(this, &v1, &v2, &v3, &v4, s);
    return *this;
}

Vector2 &Vector2::Hermite(const Vector2 &v1, const Vector2 &t1, const Vector2 &v2, const Vector2 &t2, float s)
{
    D3DXVec2Hermite(this, &v1, &t1, &v2, &t2, s);
    return *this;
}

Vector2 &Vector2::TransformCoord(const Vector2 &v, const Matrix &m)
{
    D3DXVec2TransformCoord(this, &v, &m);
    return *this;
}

Vector2 &Vector2::TransformNormal(const Vector2 &v, const Matrix &m)
{
    D3DXVec2TransformNormal(this, &v, &m);
    return *this;
}


