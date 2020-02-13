
#ifndef __CYLINDER_H_
#define __CYLINDER_H_

#include "libStandard.h"
#include "Vector3.h"

class Cylinder
{
public:
    inline Cylinder() : m_Radius(0.0f), m_Length(0.0f) {}
    inline Cylinder(float radius, float length) : m_Radius(radius), m_Length(length) {}

    inline void SetRadius(float radius) { m_Radius = radius; }
    inline void SetLength(float length) { m_Length = length; }

    inline float GetRadius() const { return m_Radius; }
    inline float GetLength() const { return m_Length; }

private:
    float m_Radius;
    float m_Length;

};

#endif // __CYLINDER_H_

