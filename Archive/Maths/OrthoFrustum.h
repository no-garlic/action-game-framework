
#ifndef __MATH_ORTHO_FRUSTUM_H_
#define __MATH_ORTHO_FRUSTUM_H_

#include "Frustum.h"
#include "Sphere.h"

class OrthoFrustum : public Frustum
{
public:
    OrthoFrustum();

    void Set(const Sphere &focus, const Vector3 &dir, float length);

protected:
    virtual void RecalculateFrustum() const;

    Sphere  m_Focus;
    Vector3    m_Dir;
    float   m_Length;

};

#endif // __MATH_ORTHO_FRUSTUM_H_
