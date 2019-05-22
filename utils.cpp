#include "utils.h"

#include "druloader.h"
#include "eaglelayers.h"
#include "graphicsutilities.h" // TODO move to here

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <QRegularExpression>
#include <QTransform>

#include <qmath.h>

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

void EAGLE_Utils::QLineF_ToWire(const QLineF &line, Wire *wire)
{
    wire->setX1(line.x1());
    wire->setX2(line.x2());
    wire->setY1(line.y1());
    wire->setY2(line.y2());
}

int EAGLE_Utils::layerIndexFromName(const QString &layerName, Eagle *design)
{
    for (Layer* layer :  *design->drawing()->layers()->layerList()) {
        if (layer->name() == layerName)
            return layer->number();
    }
    return -1;
}

QPainterPath EAGLE_Utils::smdToStopMaskPainterPath(const Smd & smd, DRULoader *dru)
{
    QPainterPath ret;
    ret.addRoundedRect(EAGLE_Utils::smdToQRectF(smd),
                                 smd.roundness(),
                                 smd.roundness(),
                                 Qt::RelativeSize);

    qreal rotation = smd.rot().mid(1).toInt();
    QTransform transform;
    transform.translate(smd.x(), smd.y());

    qreal scalePercentage = dru->stopMaskPercentage();
    if (smd.dx() * ((dru->stopMaskPercentage() / 100.0) - 1.0) > dru->stopMaskOffsetMaxInMm()) {
        scalePercentage = (dru->stopMaskOffsetMaxInMm() / smd.dx()) * 100.0;
    } else if (smd.dx() * ((dru->stopMaskPercentage() / 100.0) - 1.0) < dru->stopMaskOffsetMinInMm()) {
        scalePercentage = (dru->stopMaskOffsetMinInMm() / smd.dx()) * 100.0;
    }

    if (smd.dy() * ((dru->stopMaskPercentage() / 100.0) - 1.0) > dru->stopMaskOffsetMaxInMm()) {
        scalePercentage = (dru->stopMaskOffsetMaxInMm() / smd.dy()) * 100.0;
    } else if (smd.dy() * ((dru->stopMaskPercentage() / 100.0) - 1.0) < dru->stopMaskOffsetMinInMm()) {
        scalePercentage = (dru->stopMaskOffsetMinInMm() / smd.dy()) * 100.0;
    }

    transform.scale(1.0 + (scalePercentage / 50.0),
                    1.0 + (scalePercentage / 50.0));
    transform.rotate(rotation);
    transform.translate(-smd.x(), -smd.y());
    return transform.map(ret);
}

QPainterPath EAGLE_Utils::padToStopMaskPainterPath(const Pad & pad, DRULoader *dru)
{
    qreal diameter = pad.diameter();
    if (!pad.diameterSet()) {
        diameter = pad.drill() * (1.0 + (dru->padTopAnnularRingPercentage() / 100.0));
        // FIXME check stop limits and only top annular ring is used...
    }

    QPainterPath ret = padShapeToPainterPath(pad, diameter, dru);
    qreal rotation = pad.rot().mid(1).toInt();
    QTransform transform;
    transform.translate(pad.x(), pad.y());

    qreal scalePercentage = dru->stopMaskPercentage();
    if (pad.diameter() * ((dru->stopMaskPercentage() / 100.0) - 1.0) > dru->stopMaskOffsetMaxInMm()) {
        scalePercentage = (dru->stopMaskOffsetMaxInMm() / pad.diameter()) * 100.0;
    } else if (pad.diameter() * ((dru->stopMaskPercentage() / 100.0) - 1.0) < dru->stopMaskOffsetMinInMm()) {
        scalePercentage = (dru->stopMaskOffsetMinInMm() / pad.diameter()) * 100.0;
    }

    transform.scale(1.0 + (scalePercentage / 50.0),
                    1.0 + (scalePercentage / 50.0));
    transform.rotate(rotation);
    transform.translate(-pad.x(), -pad.y());
    return transform.map(ret);
}

QPainterPath EAGLE_Utils::padShapeToPainterPath(const Pad &pad, qreal diameter, DRULoader *dru)
{
    qreal radius = diameter / 2.0;
    QPainterPath ret;
    switch (pad.shape()) {
    case Pad::Shape_square:
        ret.addRect(QRectF(pad.x() + radius,
                           pad.y() + radius,
                           diameter,
                           diameter));
        break;
    case Pad::Shape_round:
        ret.addRoundedRect(QRectF(pad.x() + radius,
                           pad.y() + radius,
                           diameter,
                           diameter),
                           50, 50);
        break;
    case Pad::Shape_octagon: {
        QPolygonF p;
        qreal halfSide = diameter / 4.0;
        p << QPointF(pad.x() - radius,   pad.y() + halfSide);
        p << QPointF(pad.x() - halfSide, pad.y() + radius);
        p << QPointF(pad.x() + halfSide, pad.y() + radius);
        p << QPointF(pad.x() + radius,   pad.y() + halfSide);
        p << QPointF(pad.x() + radius,   pad.y() - halfSide);
        p << QPointF(pad.x() + halfSide, pad.y() - radius);
        p << QPointF(pad.x() - halfSide, pad.y() - radius);
        p << QPointF(pad.x() - radius,   pad.y() - halfSide);
        p << QPointF(pad.x() - radius,   pad.y() + halfSide);
        ret.addPolygon(p);
    } break;
    case Pad::Shape_long: {
        qreal e = (dru->elongationPercentageForLongPads() / 100.0) * diameter;
        ret.moveTo(-radius - (e/2.0 - radius), radius);
        ret.arcTo(QRectF(e/2.0 - radius, radius, diameter, -diameter),
                  90,
                  -180);
        ret.arcTo(QRectF(-radius - e/2.0, radius, diameter, -diameter),
                  270,
                  -180);
    } break;
    case Pad::Shape_offset: {
        qreal e = (dru->elongationPercentageForOffsetPads() / 100.0) * diameter;
        ret.moveTo(0, radius);
        ret.arcTo(QRectF(radius * (dru->elongationPercentageForOffsetPads() / 100.0), radius, diameter, -diameter),
                  90,
                  -180);
        ret.arcTo(QRectF(-radius, radius, diameter, -diameter),
                  270,
                  -180);
    } break;
    case Pad::Shape_Invalid:
        break;
    }
    return ret;
}

QPainterPath EAGLE_Utils::viaShapeToPainterPath(const Via &via)
{
    QPainterPath path;
    switch (via.shape()) {
    case Via::Shape_Invalid:
        return path;
    case Via::Shape_square:
        path.addRect(via.x() - (via.diameter()/2.0),
                     via.y() - (via.diameter()/2.0),
                     via.diameter(),
                     via.diameter());
        break;
    case Via::Shape_round:
        path.addEllipse(QPointF(via.x(), via.y()), via.diameter(), via.diameter());
        break;
    case Via::Shape_octagon: {
        qreal halfSide = (via.diameter() * sin(qDegreesToRadians(45.0))) / 2.0;
        QPolygonF p;
        qreal radius = via.diameter() / 2.0;
        p << QPointF(via.x() - radius,   via.y() + halfSide);
        p << QPointF(via.x() - halfSide, via.y() + radius);
        p << QPointF(via.x() + halfSide, via.y() + radius);
        p << QPointF(via.x() + radius,   via.y() + halfSide);
        p << QPointF(via.x() + radius,   via.y() - halfSide);
        p << QPointF(via.x() + halfSide, via.y() - radius);
        p << QPointF(via.x() - halfSide, via.y() - radius);
        p << QPointF(via.x() - radius,   via.y() - halfSide);
        p << QPointF(via.x() - radius,   via.y() + halfSide);
        path.addPolygon(p);
    } break;
    }
    return path;
}

float EAGLE_Utils::rotationToDegrees(const QString& rotation, bool *mirrored, bool* spin)
{
    if (mirrored != nullptr)
        *mirrored = rotation.contains('M');
    if (spin != nullptr)
        *spin = rotation.contains('S');

    QRegularExpression re("[^0-9]*(.*)");
    return re.match(rotation).captured(1).toFloat();
}

QRectF EAGLE_Utils::boardBoundingRect(Board *brd)
{
    QPolygonF poly;
    for (Wire* wire : *brd->plain()->wireList()) {
        if (wire->layer() == EagleLayers::Dimension) {
            QLineF wireLine = wire2QLine(*wire);
            if (poly.isEmpty()) {
                poly.append(wireLine.p1());
                poly.append(wireLine.p2());
            } else {
                if (poly.last() == wireLine.p1())
                    poly.append(wireLine.p2());
            }
        }
    }
    return poly.boundingRect();
}

Qt::PenCapStyle EAGLE_Utils::wireCapStyleToPenCapStyle(const Wire::CapEnum capStyle)
{
    if (capStyle == Wire::Cap_round)
        return Qt::RoundCap;
    return Qt::FlatCap;
}

int EAGLE_Utils::painterPathWireIntersections(const QPainterPath &path,
                                              const QPointF &pathCenter,
                                              const Wire &wire,
                                              QPointF *internalPoint,
                                              QPointF *intersectionPt1,
                                              QPointF *intersectionPt2)
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

    pen.setColor(Qt::darkGreen);
    painter.setPen(pen);
    painter.drawPath(path);

    QPainterPath wireContourPath;
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
    //painter.drawPath(rectanglePath.intersected(wireContourPath));


    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawLine(-250, 0, 250, 0);
    painter.drawLine(0, 250, 0, -250);

    int intersectionCount = 0;
    if (!path.intersected(wireContourPath).isEmpty()) {
        qWarning() << path;
        QLineF centerLine = wire2QLine(wire);
        qreal centerLineAngle = centerLine.angle();

        qreal x = 0.0, y = 0.0;
        for (int elementIndex = 0; elementIndex<path.elementCount(); elementIndex++) {
            if (intersectionCount > 1)
                break;
            QPainterPath::Element e = path.elementAt(elementIndex);
            switch (e.type) {
            case QPainterPath::MoveToElement:
                x = e.x;
                y = e.y;
                break;
            case QPainterPath::LineToElement: {
                QLineF rectLine = QLineF(x, y, e.x, e.y);
                if (!qFuzzyCompare(rectLine.angle(), centerLineAngle)) {
                    QPointF tmp;
                    QLineF::IntersectType hasInterSection = rectLine.intersect(centerLine, &tmp);
                    if (hasInterSection == QLineF::BoundedIntersection) {
                        QPointF *intersectionPt = intersectionCount == 0 ? intersectionPt1 : intersectionPt2;
                        intersectionPt->setX(tmp.x());
                        intersectionPt->setY(tmp.y());
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
                QPointF p1, p2;
                int curveInterSectionCount = 0;
                bool hasInterSection = GraphicsUtilities::lineIntersectsCurve(centerLine,
                                                                              QPointF(x, y),
                                                                              QPointF(path.elementAt(elementIndex+2).x, path.elementAt(elementIndex+2).y),
                                                                              QPointF(e.x, e.y),
                                                                              QPointF(path.elementAt(elementIndex+1).x, path.elementAt(elementIndex+1).y),
                                                                              &p1,
                                                                              &p2,
                                                                              &thirdInterSectionPt,
                                                                              &curveInterSectionCount);
                if (hasInterSection) {
                    // rounded corners can have only 2 intersection points
                    Q_ASSERT(curveInterSectionCount < 3);
                    // one line could not have intersection on anotehr item and two on a corner
                    Q_ASSERT(!(intersectionCount && curveInterSectionCount > 1));
                    qWarning() << "intersection(s): " << p1 << p2 << thirdInterSectionPt;
                    if (intersectionCount == 0) {
                        *intersectionPt1 = p1;
                        if (curveInterSectionCount > 1)
                            *intersectionPt2 = p2;
                    } else if (intersectionCount == 1) {
                        *intersectionPt2 = p1;
                    }
                    intersectionCount += curveInterSectionCount;
                    if (intersectionCount > 1) {
                        break;
                    }
                }
                elementIndex += 2;
                x = path.elementAt(elementIndex).x;
                y = path.elementAt(elementIndex).y;
                break;
            }
            case QPainterPath::CurveToDataElement:
                Q_ASSERT_X(false, "EAGLE_Utils::smdStopMaskWireIntersections", "This shall be skipped");
                break;
            }
        } // end for on rectanglePath segments

        // the user is curious about the internal point
        if (intersectionCount == 1 && internalPoint) {
            QLineF endCheckLine1(centerLine.p1(), pathCenter);
            QLineF endCheckLine2(centerLine.p2(), pathCenter);
            if (endCheckLine1.length() < endCheckLine2.length()) {
                *internalPoint = centerLine.p1();
            } else if (endCheckLine1.length() > endCheckLine2.length()) {
                *internalPoint = centerLine.p2();
            } else {
                // wire length equal -> return with two points at the very same position
                if (intersectionPt2)
                    *intersectionPt2 = *intersectionPt1;
                intersectionCount = 2;
                internalPoint = nullptr;
            }

            if (internalPoint) {
                pen.setColor(Qt::magenta);
                painter.setPen(pen);
                painter.drawEllipse(*internalPoint, 0.1, 0.1);

                QLineF internalPart(*internalPoint, *intersectionPt1);
                internalPart.setLength(internalPart.length() + wire.width() / 2.0);

                QLineF externalPart(*internalPoint == centerLine.p1() ? centerLine.p2() : centerLine.p1(),
                                    *intersectionPt1);
                externalPart.setLength(externalPart.length() - wire.width() / 2.0);

                pen.setColor(Qt::white);
                painter.setPen(pen);
                painter.drawLine(internalPart);

                pen.setColor(Qt::darkCyan);
                painter.setPen(pen);
                painter.drawLine(externalPart);
            }
        }

        pen.setColor(Qt::lightGray);
        painter.setPen(pen);
        if (intersectionCount >= 1) {
            painter.drawEllipse(*intersectionPt1, 0.1, 0.1);
        }
        if (intersectionCount == 2) {
            painter.drawEllipse(*intersectionPt2, 0.1, 0.1);

            QLineF interSectP1Check(centerLine.p1(), *intersectionPt1);
            QLineF interSectP2Check(centerLine.p2(), *intersectionPt1);
            QLineF externalPart1(interSectP1Check.length() < interSectP2Check.length() ?
                                     centerLine.p1() : centerLine.p2(),
                                 *intersectionPt1);
            externalPart1.setLength(externalPart1.length() - wire.width() / 2.0);

            QLineF externalPart2(centerLine.p1() == externalPart1.p1() ?
                                     centerLine.p2() : centerLine.p1(),
                                 *intersectionPt2);
            externalPart2.setLength(externalPart2.length() - wire.width() / 2.0);

            QLineF internalPart(externalPart1.p2(), externalPart2.p2());

            pen.setColor(Qt::white);
            painter.setPen(pen);
            painter.drawLine(internalPart);

            pen.setColor(Qt::darkCyan);
            painter.setPen(pen);
            painter.drawLine(externalPart1);

            pen.setColor(Qt::darkCyan);
            painter.setPen(pen);
            painter.drawLine(externalPart2);
        }
    }

    painter.end();
    QLabel *label = new QLabel(nullptr, Qt::Window);
    label->setPixmap(QPixmap::fromImage(image));
    label->show();

    return intersectionCount;
}

int EAGLE_Utils::smdStopMaskWireIntersections(const Smd &smd,
                                              const Wire &wire,
                                              QPointF *internalPoint,
                                              QPointF *intersectionPt1,
                                              QPointF *intersectionPt2,
                                              DRULoader *dru)
{
    QPainterPath stopMaskPath = smdToStopMaskPainterPath(smd, dru);
    return painterPathWireIntersections(
                stopMaskPath,
                EAGLE_Utils::smdToQRectF(smd).center(),
                wire,
                internalPoint,
                intersectionPt1,
                intersectionPt2);

}

int EAGLE_Utils::padStopMaskWireIntersections(const Pad &pad,
                                              const Wire &wire,
                                              QPointF *internalPoint,
                                              QPointF *intersectionPt1,
                                              QPointF *intersectionPt2,
                                              DRULoader *dru)
{
    QPainterPath stopMaskPath = padToStopMaskPainterPath(pad, dru);
    return painterPathWireIntersections(
                stopMaskPath,
                QPointF(pad.x(), pad.y()),
                wire,
                internalPoint,
                intersectionPt1,
                intersectionPt2);
}

int EAGLE_Utils::rectWireIntersections(const Rectangle &rect, 
                                       const Wire &wire, 
                                       QPointF *internalPoint, 
                                       QPointF *intersectionPt1, 
                                       QPointF *intersectionPt2)
{
    QPainterPath stopMaskPath;
    stopMaskPath.addRect(QRectF(rect.x1(), rect.y1(), rect.x2(), rect.y2()));
    return painterPathWireIntersections(
                stopMaskPath,
                QPointF(rect.x1() + (rect.x2() - rect.x1()) / 2.0,
                        rect.y1() + (rect.y2() - rect.y1()) / 2.0),
                wire,
                internalPoint,
                intersectionPt1,
                intersectionPt2);
}

qreal EAGLE_Utils::wireAngle(const Wire &wire)
{
    return wire2QLine(wire).angle();
}


