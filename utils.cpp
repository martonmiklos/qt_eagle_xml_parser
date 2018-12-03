#include "utils.h"

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>

#include "../graphicsutilities.h" // TODO move to here

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

int EAGLE_Utils::smdStopMaskWireIntersections(const Smd &smd,
                                              const Wire &wire,
                                              QPointF *internalPoint,
                                              QPointF *intersectionPt1,
                                              QPointF *intersectionPt2,
                                              qreal stopMaskPercentage)
{
    QImage image(800, 800, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.translate(QPointF(400, 400));
    QPen pen;
    pen.setWidth(0.01);

    pen.setColor(Qt::green);
    painter.drawEllipse(-10, -10, 20, 20);
    painter.setPen(pen);
    painter.scale(75.0, -75.0);

    pen.setWidth(0.03);
    pen.setColor(Qt::red);
    painter.setPen(pen);

    QPainterPath rectanglePath, wireContourPath;
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


    wireContourPath.moveTo(wire.x1(), wire.y1());
    wireContourPath.lineTo(wire.x2(), wire.y2());
    painter.drawPath(wireContourPath);

    QPainterPath centerPath = wireContourPath;


    QTransform lineTransform;

    // push line to 0,0
    lineTransform.translate(-wire.x1(), -wire.y1());
    wireContourPath = lineTransform.map(wireContourPath);

    // rotate line to be parallel to X
    lineTransform.reset();
    qreal lineAngle = wireAngle(wire);
    lineTransform.rotate(lineAngle);
    wireContourPath = lineTransform.map(wireContourPath);

    // move line upwards with 0.5 width
    lineTransform.reset();
    lineTransform.translate(0, wire.width() / 2.0);
    wireContourPath = lineTransform.map(wireContourPath);

    // draw the first cap
    wireContourPath.arcTo(wireContourPath.elementAt(1).x - wire.width() / 2.0,
                          wireContourPath.elementAt(1).y,
                          wire.width(),
                          -wire.width(),
                          90,
                          -180);

    // draw the lower line
    wireContourPath.lineTo(wireContourPath.elementAt(0).x, wireContourPath.elementAt(1).y - wire.width());

    // draw the second endcap
    wireContourPath.arcTo(wireContourPath.elementAt(0).x - wire.width() / 2.0,
                          wireContourPath.elementAt(0).y ,
                          wire.width(),
                          -wire.width(),
                          -90,
                          -180);


    // rotate back
    lineTransform.reset();
    lineTransform.rotate(-lineAngle);
    wireContourPath = lineTransform.map(wireContourPath);

    // move back
    lineTransform.reset();
    lineTransform.translate(wire.x1(), wire.y1());
    wireContourPath = lineTransform.map(wireContourPath);

    painter.drawPath(wireContourPath);

    pen.setColor(Qt::blue);
    painter.setPen(pen);
    painter.drawPath(rectanglePath.intersected(wireContourPath));


    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawLine(-250, 0, 250, 0);
    painter.drawLine(0, 250, 0, -250);

    if (!rectanglePath.intersected(wireContourPath).isEmpty()) {
        qWarning() << rectanglePath;
        int intersectionCount = 0;
        QLineF centerLine = wire2QLine(wire);
        qreal centerLineAngle = centerLine.angle();

        qreal x = 0.0, y = 0.0;
        for (int elementIndex = 0; elementIndex<rectanglePath.elementCount() - 1; elementIndex++) {
            QPainterPath::Element e = rectanglePath.elementAt(elementIndex);
            switch (e.type) {
            case QPainterPath::MoveToElement:
                x = e.x;
                y = e.y;
                break;
            case QPainterPath::LineToElement: {
                QLineF rectLine = QLineF(x, y, e.x, e.y);
                if (rectLine.angle() != centerLineAngle) {
                    QPointF *intersectionPt = intersectionCount == 0 ? intersectionPt1 : intersectionPt2;
                    QLineF::IntersectType hasInterSection = rectLine.intersect(centerLine, intersectionPt);
                    if (hasInterSection == QLineF::BoundedIntersection) {
                        intersectionCount++;
                        if (intersectionCount > 1) {
                            break;
                        }
                    }
                }
                x = e.x;
                y = e.y;
                break;
            }
            case QPainterPath::CurveToElement: {
                QPointF thirdInterSectionPt;
                int curveInterSectionCount = 0;
                bool hasInterSection = GraphicsUtilities::lineIntersectsCurve(centerLine,
                                                                              QPointF(x, y),
                                                                              QPointF(rectanglePath.elementAt(elementIndex+2).x, rectanglePath.elementAt(elementIndex+2).y),
                                                                              QPointF(e.x, e.y),
                                                                              QPointF(rectanglePath.elementAt(elementIndex+1).x, rectanglePath.elementAt(elementIndex+1).y),
                                                                              intersectionPt1,
                                                                              intersectionPt1,
                                                                              &thirdInterSectionPt,
                                                                              &curveInterSectionCount
                                                                              );
                if (hasInterSection) {
                    // rounded corners can have only 2 intersection points
                    Q_ASSERT(curveInterSectionCount < 3);
                    // one line could not have intersection on anotehr item and two on a corner
                    Q_ASSERT(!(intersectionCount && curveInterSectionCount > 1));
                    qWarning() << "intersections: " << *intersectionPt1 << *intersectionPt2 << thirdInterSectionPt;
                    intersectionCount += curveInterSectionCount;
                    if (intersectionCount > 1) {
                        break;
                    }
                }
                elementIndex += 3;
                x = rectanglePath.elementAt(elementIndex+2).x;
                y = rectanglePath.elementAt(elementIndex+2).y;
                break;
            }
            case QPainterPath::CurveToDataElement:
                Q_ASSERT_X(false, "EAGLE_Utils::smdStopMaskWireIntersections", "This shall be skipped");
                break;
            }
        } // end for on rectanglePath segments

        // the user is curious about the internal point
        if (intersectionCount == 1 && internalPoint) {
            QLineF endCheckLine1(centerLine.p1(), smdToQRectF(smd).center());
            QLineF endCheckLine2(centerLine.p1(), smdToQRectF(smd).center());
            if (endCheckLine1.length() < endCheckLine2.length()) {
                *internalPoint = centerLine.p1();
            } else if (endCheckLine1.length() > endCheckLine2.length()) {
                *internalPoint = centerLine.p2();
            } else {
                // wire length equal -> return with two points
                if (intersectionPt2)
                    *intersectionPt2 = *intersectionPt1;
                if (intersectionCount != 2)
                    intersectionCount = 2;
            }

            pen.setColor(Qt::magenta);
            painter.setPen(pen);
            painter.drawEllipse(*internalPoint, 1, 1);
        }
    }

    painter.end();
    QLabel *label = new QLabel(nullptr, Qt::Window);
    label->setPixmap(QPixmap::fromImage(image));
    label->show();

    return 0;
}

qreal EAGLE_Utils::wireAngle(const Wire &wire)
{
    return wire2QLine(wire).angle();
}


