#ifndef NORMALLAYERDRAWER_H
#define NORMALLAYERDRAWER_H

#include "eagle.h"
#include "layerdrawer.h"

class NormalLayerDrawer : public LayerDrawer
{
public:
    NormalLayerDrawer(QColor color, QColor highLightColor);

    void paint(QPainter *painter) override;

private:
    QList<Wire*> m_wires;
    QList<Circle*> m_circles;
    QList <Polygon*> m_polygons;

    void drawWire(Wire *wire, QPainter *painter);
};

#endif // NORMALLAYERDRAWER_H
