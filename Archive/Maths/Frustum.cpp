
#include "Frustum.h"
#include "Sphere.h"
#include "Box.h"
#include "Matrix.h"

Frustum::Frustum()
{
}

Plane *Frustum::GetPlanes()
{
    if (m_dirty)
        RecalculateFrustum();

    return m_planes;
}

const Plane *Frustum::GetPlanes() const
{
    if (m_dirty)
        RecalculateFrustum();

    return m_planes;
}

int Frustum::Intersects(const Sphere &sphere, const Matrix &world_matrix) const
{
    Sphere s = sphere;
    s.TransformAndScale(world_matrix);
    return Intersects(s);
}

int Frustum::Intersects(const Sphere &sphere) const
{
    if (m_dirty)
        RecalculateFrustum();

    int val = Frustum::IN_FRUSTUM;
    for (int i = 0; i < 6; i++)
    {
        float distance = (m_planes[i].a * sphere.center.x + m_planes[i].b * sphere.center.y +
            m_planes[i].c * sphere.center.z) - m_planes[i].d;

        bool front = (distance + sphere.radius >= 0.0f);
        bool back = (distance - sphere.radius <= 0.0f);

        if (front && back)
            val = Frustum::PARTIAL;
        else if (back)
            return Frustum::OUT_FRUSTUM;
    }

    return val;
}

int Frustum::Intersects(const Box &box) const
{
    if (m_dirty)
        RecalculateFrustum();

    bool is_partial = false;
    for (int i = 0; i < 6; i++)
    {
        Plane::Classification pc = m_planes[i].WhichSide(box);

        if (pc == Plane::BACK)
            return Frustum::OUT_FRUSTUM;

        if (pc == Plane::FRONT)
            continue;

        is_partial = true;
    }

    return (is_partial ? Frustum::PARTIAL : Frustum::IN_FRUSTUM);
}

int Frustum::Intersects(const Box &box, const Matrix &world_matrix) const
{
    if (m_dirty)
        RecalculateFrustum();

    Box transformedBox(box);
    transformedBox.MinPt.TransformCoord(box.MinPt, world_matrix);
    transformedBox.MaxPt.TransformCoord(box.MaxPt, world_matrix);

    return Intersects(transformedBox);
}

String Frustum::ToString() const
{
    if (m_dirty)
        RecalculateFrustum();

    StringStream ss;

    ss << "LEFT:    " << m_planes[Frustum::LEFT].ToString() << std::endl;
    ss << "RIGHT:   " << m_planes[Frustum::RIGHT].ToString() << std::endl;
    ss << "TOP:     " << m_planes[Frustum::TOP].ToString() << std::endl;
    ss << "BOTTOM:  " << m_planes[Frustum::BOTTOM].ToString() << std::endl;
    ss << "FRONT:   " << m_planes[Frustum::FRONT].ToString() << std::endl;
    ss << "BACK:    " << m_planes[Frustum::BACK].ToString() << std::endl;

    return ss.str();
}

