#include "vialayerdrawer.h"

#include <qmath.h>

#include "utils.h"

ViaLayerDrawer::ViaLayerDrawer(QColor color, QColor highLightColor, int layerIndex) :
    LayerDrawer(color, highLightColor, layerIndex)
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
    QPainterPath path = EAGLE_Utils::viaShapeToPainterPath(*via);
    path.addEllipse(QPointF(via->x(), via->y()), via->drill(), via->drill());
    painter->setBrush(QBrush(m_color));
    painter->drawPath(path);
}
