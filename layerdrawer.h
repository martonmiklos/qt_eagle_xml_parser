#ifndef LAYERDRAWER_H
#define LAYERDRAWER_H

#include <QColor>
#include <QPainter>

class LayerDrawer
{
public:
    LayerDrawer(const QColor &color, const QColor &highLightColor, int layerIndex);
    virtual ~LayerDrawer();

    virtual void paint(QPainter *painter) = 0;

    int layerIndex() const;

protected:
    QColor m_color, m_highLightColor;
    int m_layerIndex;
};

#endif // LAYERDRAWER_H
