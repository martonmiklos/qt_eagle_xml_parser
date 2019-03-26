#ifndef VIALAYERDRAWER_H
#define VIALAYERDRAWER_H

#include "eagle.h"
#include "layerdrawer.h"

class ViaLayerDrawer : public LayerDrawer
{
public:
    ViaLayerDrawer(QColor color, QColor highLightColor, int layerIndex);

    void paint(QPainter *painter) override;

    void clearVias();
    void addVia(Via *via);
    void removeVia(Via *via);

private:
    QList<Via*> m_vias;

    void paintVia(Via *via, QPainter *painter);
};

#endif // VIALAYERDRAWER_H
