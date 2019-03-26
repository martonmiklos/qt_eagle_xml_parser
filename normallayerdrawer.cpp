#include "normallayerdrawer.h"


NormalLayerDrawer::NormalLayerDrawer(QColor color, QColor highLightColor, int layerIndex) :
    LayerDrawer(color, highLightColor, layerIndex)
{

}


void NormalLayerDrawer::paint(QPainter *painter)
{
    for (Wire *wire : m_wires) {
        drawWire(*wire, painter);
    }

    for (Rectangle *rect : m_rectangles) {
        //drawRectangle(rect);
    }
}

void NormalLayerDrawer::drawWire(const Wire &wire, QPainter *painter)
{
    painter->setBrush(m_color);
    
}

void NormalLayerDrawer::drawPolygon(const Polygon &rect, QPainter *painter)
{
    
}
