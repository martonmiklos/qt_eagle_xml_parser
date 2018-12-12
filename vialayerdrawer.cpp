#include "vialayerdrawer.h"

#include <qmath.h>

ViaLayerDrawer::ViaLayerDrawer(QColor color, QColor highLightColor) :
    LayerDrawer(color, highLightColor)
{

}

void ViaLayerDrawer::paint(QPainter *painter)
{
    for (Via *via : m_vias) {
        paintVia(via, painter);
    }
}

void ViaLayerDrawer::clearVias()
{
    m_vias.clear();
}

void ViaLayerDrawer::addVia(Via *via)
{
    m_vias.append(via);
}

void ViaLayerDrawer::removeVia(Via *via)
{
    m_vias.removeAll(via);
}

void ViaLayerDrawer::paintVia(Via *via, QPainter *painter)
{
    QPainterPath path;
    switch (via->shape()) {
    case Via::Shape_Invalid:
        return;
    case Via::Shape_square:
        path.addRect(via->x() - (via->diameter()/2.0),
                     via->y() - (via->diameter()/2.0),
                     via->diameter(),
                     via->diameter());
        break;
    case Via::Shape_round:
        path.addEllipse(QPointF(via->x(), via->y()), via->diameter(), via->diameter());
        break;
    case Via::Shape_octagon: {
        qreal halfSide = (via->diameter() * sin(qDegreesToRadians(45.0))) / 2.0;
        QPolygonF p;
        qreal radius = via->diameter() / 2.0;
        p << QPointF(via->x() - radius,   via->y() + halfSide);
        p << QPointF(via->x() - halfSide, via->y() + radius);
        p << QPointF(via->x() + halfSide, via->y() + radius);
        p << QPointF(via->x() + radius,   via->y() + halfSide);
        p << QPointF(via->x() + radius,   via->y() - halfSide);
        p << QPointF(via->x() + halfSide, via->y() - radius);
        p << QPointF(via->x() - halfSide, via->y() - radius);
        p << QPointF(via->x() - radius,   via->y() - halfSide);
        p << QPointF(via->x() - radius,   via->y() + halfSide);
        path.addPolygon(p);
    } break;
    }
    path.addEllipse(QPointF(via->x(), via->y()), via->drill(), via->drill());
    painter->setBrush(QBrush(m_color));
    painter->drawPath(path);
}
