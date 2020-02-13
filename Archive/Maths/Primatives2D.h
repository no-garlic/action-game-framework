
#ifndef __MATH_PRIMATIVES_2D_H_
#define __MATH_PRIMATIVES_2D_H_

#include "Type.h"
#include "Vector2.h"
#include "Vector3.h"

struct Circle2D;
struct Box2D;

struct Circle2D
{
    Circle2D() : center(0.0f, 0.0f), radius(0.0f) {}
    Circle2D(const Vector2 &center, float radius) : center(center), radius(radius) {}

    Vector2  center;
    float radius;
};

struct Box2D
{
    Box2D() : MinPt(0.0f, 0.0f), MaxPt(0.0f, 0.0f) {}
    Box2D(const Vector2 &MinPt, const Vector2 &MaxPt) : MinPt(MinPt), MaxPt(MaxPt) {}
    Box2D(float x1, float x2, float y1, float y2) { MinPt.Set(x1, y1); MaxPt.Set(x2, y2); }
    Box2D(const Circle2D &circle)
    {
        MinPt.Set(circle.center.x - circle.radius, circle.center.y - circle.radius);
        MaxPt.Set(circle.center.x + circle.radius, circle.center.y + circle.radius);
    }

    Box2D &Maximize(const Box2D &box)
    {
        MinPt.x = Min<float>(MinPt.x, box.MinPt.x);  MinPt.y = Min<float>(MinPt.y, box.MinPt.y);
        MaxPt.x = Max<float>(MaxPt.x, box.MaxPt.x);  MaxPt.y = Max<float>(MaxPt.y, box.MaxPt.y);
        return *this;
    }

    Box2D &Maximize(const Circle2D &circle)
    {
        Box2D box(circle);
        MinPt.x = Min<float>(MinPt.x, box.MinPt.x);  MinPt.y = Min<float>(MinPt.y, box.MinPt.y);
        MaxPt.x = Max<float>(MaxPt.x, box.MaxPt.x);  MaxPt.y = Max<float>(MaxPt.y, box.MaxPt.y);
        return *this;
    }

    Box2D &Translate(const Vector2 &trans)
    {
        MinPt += trans;
        MaxPt += trans;
        return *this;
    }

    bool IsWithin(const Vector2 &pos) const
    {
        return (pos.x >= MinPt.x && pos.y >= MinPt.y && pos.x <= MaxPt.x && pos.y <= MaxPt.y);
    }

    void GetNearestPoint(const Vector2 test_point, Vector2 &point) const;

    Vector2 MinPt;
    Vector2 MaxPt;

};

#endif // __MATH_PRIMATIVES_2D_H_

