
#include "PerspectiveFrustum.h"
#include "Sphere.h"
#include "Box.h"
#include "Matrix.h"

PerspectiveFrustum::PerspectiveFrustum()
    : Frustum()
{
    m_view_mat.Identity();
    m_proj_mat.Identity();
}

void PerspectiveFrustum::SetViewMatrix(const Matrix &m)
{
    m_view_mat = m;
    m_dirty = true;
}

void PerspectiveFrustum::SetProjMatrix(const Matrix &m)
{
    m_proj_mat = m;
    m_dirty = true;
}

void PerspectiveFrustum::RecalculateFrustum() const
{
    // Proceed if one of the matricies have changed
    if (!m_dirty)
        return;

    // Create the clipping matrix
    Matrix m;
    D3DXMatrixMultiply(&m, &m_view_mat, &m_proj_mat);

    // Left clipping plane
    m_planes[Frustum::LEFT].a = (m._14 + m._11);
    m_planes[Frustum::LEFT].b = (m._24 + m._21);
    m_planes[Frustum::LEFT].c = (m._34 + m._31);
    m_planes[Frustum::LEFT].d = -(m._44 + m._41);

    // Right clipping plane
    m_planes[Frustum::RIGHT].a = (m._14 - m._11);
    m_planes[Frustum::RIGHT].b = (m._24 - m._21);
    m_planes[Frustum::RIGHT].c = (m._34 - m._31);
    m_planes[Frustum::RIGHT].d = -(m._44 - m._41);

    // Top clipping plane
    m_planes[Frustum::TOP].a = (m._14 - m._12);
    m_planes[Frustum::TOP].b = (m._24 - m._22);
    m_planes[Frustum::TOP].c = (m._34 - m._32);
    m_planes[Frustum::TOP].d = -(m._44 - m._42);

    // Bottom clipping plane
    m_planes[Frustum::BOTTOM].a = (m._14 + m._12);
    m_planes[Frustum::BOTTOM].b = (m._24 + m._22);
    m_planes[Frustum::BOTTOM].c = (m._34 + m._32);
    m_planes[Frustum::BOTTOM].d = -(m._44 + m._42);

    // Near clipping plane
    m_planes[Frustum::FRONT].a = (m._13);
    m_planes[Frustum::FRONT].b = (m._23);
    m_planes[Frustum::FRONT].c = (m._33);
    m_planes[Frustum::FRONT].d = -(m._43);

    // Far clipping plane
    m_planes[Frustum::BACK].a = (m._14 - m._13);
    m_planes[Frustum::BACK].b = (m._24 - m._23);
    m_planes[Frustum::BACK].c = (m._34 - m._33);
    m_planes[Frustum::BACK].d = -(m._44 - m._43);

    // Normalize the 6 planes
    for (int i = 0; i < 6; i++)
        m_planes[i].Normalize();

    m_dirty = false;
}

