#ifndef SMDPAINTER_H
#define SMDPAINTER_H

#include "eagle.h"

#include <QPainter>

class SmdPainter
{
public:
    static void draw(QPainter* painter, Smd* smd);
    static void drawCream(QPainter* painter, Smd* smd, qreal factor = 1.0);
    static void drawStopMask(QPainter* painter, Smd* smd, qreal factor = 1.0);
};

#endif // SMDPAINTER_H
