#ifndef LAYERDRAWER_H
#define LAYERDRAWER_H

#include <QColor>
#include <QPainter>

class LayerDrawer
{
public:
    LayerDrawer(const QColor &color, const QColor &highLightColor);

    virtual void paint(QPainter *painter) = 0;

protected:
    QColor m_color, m_highLightColor;
};

#endif // LAYERDRAWER_H
