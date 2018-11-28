#ifndef UTILS_H
#define UTILS_H

#include "eagle.h"

#include <QLineF>
#include <QRectF>
#include <QString>


class EAGLE_Utils
{
public:
    static QLineF wire2QLine(const Wire & wire);
    static QRectF smdToQRectF(const Smd & smd);

    static int layerIndexFromName(const QString & layerName, Eagle *design);
};

#endif // UTILS_H
