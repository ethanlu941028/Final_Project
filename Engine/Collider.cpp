#include <allegro5/color.h>
#include <memory>
#include <vector>

#include "Collider.hpp"
#include "Engine/Point.hpp"

namespace Engine {
    bool Collider::IsPointInBitmap(Point pnt, const std::shared_ptr<ALLEGRO_BITMAP> &bmp) {
        return al_get_pixel(bmp.get(), static_cast<int>(pnt.x), static_cast<int>(pnt.y)).a != 0;
    }
    bool Collider::IsPointInRect(Point pnt, Point rectPos, Point rectSize) {
        return (rectPos.x <= pnt.x && pnt.x < rectPos.x + rectSize.x && rectPos.y <= pnt.y && pnt.y < rectPos.y + rectSize.y);
    }
    bool Collider::IsRectOverlap(Point r1Min, Point r1Max, Point r2Min, Point r2Max) {
        return (r1Max.x > r2Min.x && r2Max.x > r1Min.x && r1Max.y > r2Min.y && r2Max.y > r1Min.y);
    }
    bool Collider::IsCircleOverlap(Point c1, float r1, Point c2, float r2) {
        return (c1 - c2).Magnitude() < r1 + r2;
    }
    static std::vector<Point> GetAxes(const std::vector<Point>& poly) {
        std::vector<Point> axes;
        for (size_t i = 0; i < poly.size(); ++i) {
            Point p1 = poly[i];
            Point p2 = poly[(i + 1) % poly.size()];
            Point edge = p2 - p1;
            Point axis(-edge.y, edge.x);
            if (axis.Magnitude() != 0)
                axis = axis.Normalize();
            axes.push_back(axis);
        }
        return axes;
    }

    static void ProjectPolygon(const std::vector<Point>& poly, Point axis, float& min, float& max) {
        min = max = poly[0].Dot(axis);
        for (size_t i = 1; i < poly.size(); ++i) {
            float p = poly[i].Dot(axis);
            if (p < min) min = p;
            if (p > max) max = p;
        }
    }

    static bool OverlapOnAxis(const std::vector<Point>& poly1, const std::vector<Point>& poly2, Point axis) {
        float min1, max1, min2, max2;
        ProjectPolygon(poly1, axis, min1, max1);
        ProjectPolygon(poly2, axis, min2, max2);
        return !(max1 < min2 || max2 < min1);
    }

    static bool PolygonOverlapPolygon(const std::vector<Point>& poly1, const std::vector<Point>& poly2) {
        auto axes1 = GetAxes(poly1);
        auto axes2 = GetAxes(poly2);
        axes1.insert(axes1.end(), axes2.begin(), axes2.end());
        for (auto& axis : axes1) {
            if (!OverlapOnAxis(poly1, poly2, axis))
                return false;
        }
        return true;
    }

    bool Collider::IsPolygonOverlapRect(const std::vector<Point>& poly, Point rectMin, Point rectMax) {
        std::vector<Point> rect = {
            rectMin,
            Point(rectMax.x, rectMin.y),
            rectMax,
            Point(rectMin.x, rectMax.y)
        };
        return PolygonOverlapPolygon(poly, rect);
    }
}
