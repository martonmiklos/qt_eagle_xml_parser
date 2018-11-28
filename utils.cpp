#include "utils.h"

QLineF EAGLE_Utils::wire2QLine(const Wire &wire)
{
    return QLineF(wire.x1(), wire.y1(), wire.x2(), wire.y2());
}

QRectF EAGLE_Utils::smdToQRectF(const Smd &smd)
{
    return QRectF(smd.x() + (smd.dx() / 2.0),
                  smd.y() + (smd.dy() / 2.0),
                  smd.dx(),
                  smd.dy());
}

int EAGLE_Utils::layerIndexFromName(const QString &layerName, Eagle *design)
{
    for (Layer layer : design->drawing().layers().layerList()) {
        if (layer.name() == layerName)
            return layer.number();
    }
    return -1;
}
