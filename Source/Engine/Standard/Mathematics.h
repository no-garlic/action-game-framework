
#ifndef __STD_MATH_H_
#define __STD_MATH_H_

#include "Type.h"

const float FLOAT_EPSILON = 0.001f;
const float FLOAT_MAX = 3.402823466e+38F;
const float FLOAT_MIN = -3.402823466e+38F;
const float PI = 3.141592654F;
const float PI_DIV_2 = PI * 0.5f;
const float PI_DIV_4 = PI * 0.25f;
const float PI_TIMES_2 = PI * 2.0f;
const float ONE_BY_PI = 0.318309886f;

#define ToRadian( degree ) ((degree) * (PI / 180.0f))
#define ToDegree( radian ) ((radian) * (180.0f / PI))

inline bool IsPowerOf2(unsigned int num)
{
    for (uint i = 0; i < 32; i++)
    {
        if (num == 2)
            return true;

        num = num >> 1;
    }

    return false;
}

template <class T>
static void Clamp(T &a, const T &min, const T &max)
{
    if (a < min)
        a = min;
    if (a > max)
        a = max;
}

template<class T>
void Swap(T &a, T &b)
{
    const T temp = a;
    a = b;
    b = temp;
}

template<class T>
bool IsBetween(const T &a, const T &b, const T &c)
{
    return (b <= a && a <= c);
}

inline int Round(float f)
{
    int a;

    __asm
    {
        fld f
        fistp a
    };

    return a;
}

inline float FastSqrt(float x)
{
    float xhalf = 0.5f * x;
    float xprime;

    // transfer to integer register, causes memory access
    long i = *((long *)&x);   

    // poor man's exponent
    i = 0x5F3759DF - (i >> 1);

    // transfer back to floating point register
    xprime = *((float*)&i);
    xprime = xprime * (1.5f - xhalf * xprime * xprime);

    return xprime * x;
}

struct BitField
{
    BitField() { m_Data = 0; }
    BitField(uint data) { m_Data = data; }

    inline void Set(uint data) { m_Data = data; }
    inline uint Get() const { return m_Data; }

    inline operator uint() { return m_Data; }

    inline void Add(uint data) { m_Data = (m_Data | data); }
    inline void Remove(uint data) { m_Data = (m_Data & ~data); }

    inline void Add(BitField data) { m_Data = (m_Data | data.m_Data); }
    inline void Remove(BitField data) { m_Data = (m_Data & ~data.m_Data); }

    inline bool Test(BitField data) { return ((m_Data & data) != 0x0); }
    inline bool TestAll(BitField data) { return ((m_Data & data) == data); }

private:
    uint m_Data;

};

#endif // __STD_MATH_H_
