
#ifndef __MATH_PERSPECTIVE_FRUSTUM_H_
#define __MATH_PERSPECTIVE_FRUSTUM_H_

#include "Frustum.h"

class PerspectiveFrustum : public Frustum
{
public:
    PerspectiveFrustum();

    void SetViewMatrix(const Matrix &m);
    void SetProjMatrix(const Matrix &m);

protected:
    virtual void RecalculateFrustum() const;

    mutable Matrix m_view_mat;
    mutable Matrix m_proj_mat;

};

#endif // __MATH_PERSPECTIVE_FRUSTUM_H_

