#include "DieDraw.h"
#include <cmath>
#include <vector>

namespace {
void strokePolygon(HDC hdc, const std::vector<POINT>& pts) {
    if (pts.size() < 2) return;
    MoveToEx(hdc, pts[0].x, pts[0].y, nullptr);
    for (size_t i = 1; i < pts.size(); ++i)
        LineTo(hdc, pts[i].x, pts[i].y);
    LineTo(hdc, pts[0].x, pts[0].y);
}

void insetRect(RECT rc, int pad, RECT& out) {
    out.left = rc.left + pad;
    out.top = rc.top + pad;
    out.right = rc.right - pad;
    out.bottom = rc.bottom - pad;
}

void regularPolygon(HDC hdc, RECT rc, int n, float rotDeg = -90.f) {
    int cx = (rc.left + rc.right) / 2;
    int cy = (rc.top + rc.bottom) / 2;
    int r = std::min(rc.right - rc.left, rc.bottom - rc.top) / 2;
    std::vector<POINT> pts;
    const float rot = rotDeg * 3.14159265f / 180.f;
    for (int i = 0; i < n; ++i) {
        float a = rot + (2.f * 3.14159265f * i / n);
        pts.push_back({cx + static_cast<int>(r * std::cos(a)),
                       cy + static_cast<int>(r * std::sin(a))});
    }
    strokePolygon(hdc, pts);
}
} // namespace

namespace DieDraw {

void drawDie(HDC hdc, const RECT& rc, int sides) {
    HPEN pen = CreatePen(PS_SOLID, 2, RGB(20, 20, 20));
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    RECT inner{};
    insetRect(rc, 8, inner);

    switch (sides) {
    case 4: {
        int cx = (inner.left + inner.right) / 2;
        strokePolygon(hdc, {{cx, inner.top}, {inner.right, inner.bottom}, {inner.left, inner.bottom}});
        break;
    }
    case 6:
        Rectangle(hdc, inner.left, inner.top, inner.right, inner.bottom);
        break;
    case 8:
        regularPolygon(hdc, inner, 4, 45.f);
        break;
    case 10:
        regularPolygon(hdc, inner, 5, -90.f);
        break;
    case 12:
        regularPolygon(hdc, inner, 6, -90.f);
        break;
    case 20:
    default:
        regularPolygon(hdc, inner, 6, -90.f);
        break;
    }

    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

} // namespace
