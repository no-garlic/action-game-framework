
#include "Ray.h"

Ray::Ray(const Vector3 &d, const Vector3 &p, const Matrix &m)
{
    position.TransformCoord(p, m);
    direction.TransformNormal(d, m);
}

Ray::Ray(const Ray &ray, const Matrix &m)
{
    position.TransformCoord(ray.position, m);
    direction.TransformNormal(ray.direction, m);
}
