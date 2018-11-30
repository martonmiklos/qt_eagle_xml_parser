#include "utils.h"

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>

QLineF EAGLE_Utils::wire2QLine(const Wire &wire)
{
    return QLineF(wire.x1(), wire.y1(), wire.x2(), wire.y2());
}

QRectF EAGLE_Utils::smdToQRectF(const Smd &smd)
{
    return QRectF(smd.x() - (smd.dx() / 2.0),
                  smd.y() - (smd.dy() / 2.0),
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

int EAGLE_Utils::smdWireIntersections(const Smd &smd, const Wire &wire, QPointF &intersectionPt1, QPointF &intersectionPt2)
{

}

int EAGLE_Utils::smdStopMaskWireIntersections(const Smd &smd,
                                              const Wire &wire,
                                              QPointF &intersectionPt1,
                                              QPointF &intersectionPt2,
                                              qreal stopMaskPercentage)
{
    QImage image(500, 500, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.translate(QPointF(250, 250));
    QPen pen;
    pen.setWidth(0.01);

    pen.setColor(Qt::green);
    painter.drawEllipse(-10, -10, 20, 20);
    painter.setPen(pen);
    painter.scale(50.0, -50.0);

    pen.setWidth(0.03);
    pen.setColor(Qt::red);
    painter.setPen(pen);

    QPainterPath rectanglePath, linePath;
    rectanglePath.addRoundedRect(EAGLE_Utils::smdToQRectF(smd),
                                 smd.roundness(),
                                 smd.roundness(),
                                 Qt::RelativeSize);


    qreal rotation = smd.rot().mid(1).toInt();
    QTransform transform;
    transform.translate(smd.x(), smd.y());
    transform.rotate(rotation);
    transform.translate(-smd.x(), -smd.y());
    // TODO transform.scale((100.0 + stopMaskPercentage) / 100.0, (100.0 + stopMaskPercentage) / 100.0);
    rectanglePath = transform.map(rectanglePath);
    painter.drawPath(rectanglePath);


    linePath.moveTo(wire.x1(), wire.y1());
    linePath.lineTo(wire.x2(), wire.y2());
    painter.drawPath(linePath);

    QTransform lineTransform;
    lineTransform.translate(-wire.x1(), -wire.y1());
    linePath = lineTransform.map(linePath);
    painter.drawPath(linePath);

    lineTransform.reset();
    lineTransform.rotate(wireAngle(wire));
    linePath = lineTransform.map(linePath);
    painter.drawPath(linePath);
    qWarning() << linePath;

    lineTransform.reset();
    lineTransform.translate(0, wire.width() / 2.0);
    linePath = lineTransform.map(linePath);
    linePath.arcTo(linePath.elementAt(1).x,
                   linePath.elementAt(1).y,
                   wire.width(),
                   -wire.width(),
                   90,
                   -180);
    linePath.lineTo(linePath.elementAt(0).x, linePath.elementAt(1).y - wire.width());
    linePath.arcTo(linePath.elementAt(0).x - wire.width() / 2.0,
                   linePath.elementAt(0).y ,
                   wire.width(),
                   -wire.width(),
                   -90,
                   -180);
    painter.drawPath(linePath);
    qWarning() << linePath;


    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    //painter.drawLine(-250, 0, 250, 0);
    //painter.drawLine(0, 250, 0, -250);

    painter.end();
    QLabel *label = new QLabel(nullptr, Qt::Window);
    label->setPixmap(QPixmap::fromImage(image));
    label->show();

    if (rectanglePath.intersected(linePath).isEmpty())
        return 0;

}

qreal EAGLE_Utils::wireAngle(const Wire &wire)
{
    return wire2QLine(wire).angle();
}
