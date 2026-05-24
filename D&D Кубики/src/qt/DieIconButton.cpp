#include "DieIconButton.h"
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

namespace {
void drawPolygon(QPainter& p, const QPolygonF& poly) {
    QPainterPath path;
    path.addPolygon(poly);
    p.drawPath(path);
}
} // namespace

DieIconButton::DieIconButton(int sides, QWidget* parent) : QPushButton(parent), m_sides(sides) {
    setCursor(Qt::PointingHandCursor);
    setToolTip(QString("d%1").arg(sides));
}

void DieIconButton::paintEvent(QPaintEvent* event) {
    QPushButton::paintEvent(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    QPen pen(QColor(20, 20, 20));
    pen.setWidth(2);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);

    QRectF r = rect().adjusted(10, 10, -10, -10);
    const qreal cx = r.center().x();
    const qreal cy = r.center().y();
    const qreal rad = qMin(r.width(), r.height()) / 2.0;

    switch (m_sides) {
    case 4:
        drawPolygon(p, QPolygonF{{cx, r.top()}, {r.right(), r.bottom()}, {r.left(), r.bottom()}});
        break;
    case 6:
        p.drawRect(r);
        break;
    case 8: {
        QPolygonF poly;
        for (int i = 0; i < 4; ++i) {
            qreal a = qDegreesToRadians(45.0 + i * 90.0);
            poly << QPointF(cx + rad * qCos(a), cy + rad * qSin(a));
        }
        drawPolygon(p, poly);
        break;
    }
    case 10: {
        QPolygonF poly;
        for (int i = 0; i < 5; ++i) {
            qreal a = qDegreesToRadians(-90.0 + i * 72.0);
            poly << QPointF(cx + rad * qCos(a), cy + rad * qSin(a));
        }
        drawPolygon(p, poly);
        break;
    }
    case 12: {
        QPolygonF poly;
        for (int i = 0; i < 6; ++i) {
            qreal a = qDegreesToRadians(-90.0 + i * 60.0);
            poly << QPointF(cx + rad * qCos(a), cy + rad * qSin(a));
        }
        drawPolygon(p, poly);
        break;
    }
    case 20:
    default: {
        QPolygonF poly;
        for (int i = 0; i < 6; ++i) {
            qreal a = qDegreesToRadians(-90.0 + i * 60.0);
            poly << QPointF(cx + rad * qCos(a), cy + rad * qSin(a));
        }
        drawPolygon(p, poly);
        break;
    }
    }
}
