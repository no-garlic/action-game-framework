
#include "OrthoFrustum.h"
#include "Sphere.h"
#include "Box.h"
#include "Matrix.h"

OrthoFrustum::OrthoFrustum()
    : Frustum()
{
    m_Focus.Reset();
    m_Dir.Set(0.0f, 1.0f, 0.0f);
    m_Length = 1.0f;
}

void OrthoFrustum::Set(const Sphere &focus, const Vector3 &dir, float length)
{
    m_Focus = focus;
    m_Dir = dir;
    m_Length = length;

    m_Dir.Normalize();

    m_dirty = true;

    RecalculateFrustum();
}

void OrthoFrustum::RecalculateFrustum() const
{
    if (!m_dirty)
        return;

    Vector3 nearPt = m_Focus.center - (m_Dir * (m_Focus.radius + m_Length));
    Vector3 farPt = m_Focus.center + (m_Dir * m_Focus.radius);

    m_planes[Frustum::FRONT] = Plane(m_Dir, 0.0f - nearPt.Length());
    m_planes[Frustum::BACK] = Plane(-m_Dir, 0.0f - farPt.Length());

    Vector3 topA = nearPt + Vector3(0.0f, m_Focus.radius, 0.0f);
    Vector3 topB = farPt + Vector3(0.0f, m_Focus.radius, 0.0f);
    Vector3 topC = topA + Vector3(1.0f, 0.0f, 0.0f);

    Vector3 bottomA = nearPt - Vector3(0.0f, m_Focus.radius, 0.0f);
    Vector3 bottomB = farPt - Vector3(0.0f, m_Focus.radius, 0.0f);
    Vector3 bottomC = bottomA - Vector3(1.0f, 0.0f, 0.0f);

    Vector3 rightA = nearPt - Vector3(m_Focus.radius, 0.0f, 0.0f);
    Vector3 rightB = farPt - Vector3(m_Focus.radius, 0.0f, 0.0f);
    Vector3 rightC = rightA - Vector3(0.0f, 1.0f, 0.0f);

    Vector3 leftA = nearPt + Vector3(m_Focus.radius, 0.0f, 0.0f);
    Vector3 leftB = farPt + Vector3(m_Focus.radius, 0.0f, 0.0f);
    Vector3 leftC = leftA + Vector3(0.0f, 1.0f, 0.0f);

    m_planes[Frustum::TOP] = Plane(topA, topC, topB);
    m_planes[Frustum::BOTTOM] = Plane(bottomA, bottomC, bottomB);
    m_planes[Frustum::LEFT] = Plane(leftA, leftC, leftB);
    m_planes[Frustum::RIGHT] = Plane(rightA, rightC, rightB);

    m_planes[Frustum::TOP].d = -m_planes[Frustum::TOP].d;
    m_planes[Frustum::BOTTOM].d = -m_planes[Frustum::BOTTOM].d;

    for (int i = 0; i < 6; i++)
        m_planes[i].Normalize();

    m_dirty = false;
}

