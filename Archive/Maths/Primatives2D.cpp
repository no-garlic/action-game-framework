
#include "Primatives2D.h"
#include "Mathematics.h"

void Box2D::GetNearestPoint(const Vector2 test_point, Vector2 &point) const
{
    // Check if the test point is within the x 
    // range of the box.
    if (IsBetween(test_point.x, MinPt.x, MaxPt.x))
    {
        point.x = test_point.x;
        point.y = (abs(test_point.y - MinPt.y) < abs(test_point.y - MaxPt.y)) ? MinPt.y : MaxPt.y;
        return;
    }

    // Check if the test point is within the y
    // range of the box.
    if (IsBetween(test_point.y, MinPt.y, MaxPt.y))
    {
        point.x = (abs(test_point.x - MinPt.x) < abs(test_point.x - MaxPt.x)) ? MinPt.x : MaxPt.x;
        point.y = test_point.y;
        return;
    }

    // Now find which ocrner quadrant that the
    // point lies in and then return that corner point.
    if (test_point.x < MinPt.x)
    {
        if (test_point.y < MinPt.y)
        {
            point = MinPt;
        }
        else
        {
            point.x = MinPt.x;
            point.y = MaxPt.y;
        }
    }
    else
    {
        if (test_point.y < MinPt.y)
        {
            point.x = MaxPt.x;
            point.y = MinPt.y;
        }
        else
        {
            point = MaxPt;
        }
    }
}

