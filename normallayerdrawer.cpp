#include "normallayerdrawer.h"


NormalLayerDrawer::NormalLayerDrawer(QColor color, QColor highLightColor) :
    LayerDrawer(color, highLightColor)
{

}


void NormalLayerDrawer::paint(QPainter *painter)
{
    for (Wire *wire : m_wires) {
        drawWire(wire, painter);
    }
}

void NormalLayerDrawer::drawWire(Wire *wire, QPainter *painter)
{
    painter->setBrush(m_color);


}
