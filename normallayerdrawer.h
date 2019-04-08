#ifndef NORMALLAYERDRAWER_H
#define NORMALLAYERDRAWER_H

#include "eagle.h"
#include "layerdrawer.h"

class NormalLayerDrawer : public LayerDrawer
{
public:
    NormalLayerDrawer(QColor color, QColor highLightColor, int layerIndex);

    void paint(QPainter *painter) override;

private:
    QList<Wire*> m_wires;
    QList<Circle*> m_circles;
    QList <Polygon*> m_polygons;
    QList <Rectangle*> m_rectangles;

    void drawWire(const Wire & wire, QPainter *painter);
    void drawRectangle(const Rectangle & rect, QPainter *painter);
    void drawPolygon(const Polygon & rect, QPainter *painter);
};


#endif // NORMALLAYERDRAWER_H
