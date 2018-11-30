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

    /**
     * @brief painterPathLineIntersections
     * Get intersection points of an SMD pad and a Wire
     * @param smd
     * SMD pad to be checked
     * @param wire
     * Wire to be checked
     * @param pt1
     * Intersection point 1, if there is not any intersection point this argument
     * will not be changed
     * @param pt2
     * Intersection point 2, if there is only one intersection point this argument
     * will not be changed
     * @return
     * Number of intersection points 0, 1 or 2
     */
    static int smdWireIntersections(const Smd & smd, const Wire & wire, QPointF & intersectionPt1, QPointF & intersectionPt2);

    /**
     * @brief painterPathLineIntersections
     * Get intersection points of an SMD pad and a Wire
     * @param smd
     * SMD pad to be checked
     * @param wire
     * Wire to be checked
     * @param pt1
     * Intersection point 1, if there is not any intersection point this argument
     * will not be change
     * @param pt2
     * Intersection point 2, if there is only one intersection point this argument
     * will not be changed
     * @param stopMaskPercentage
     * Size ratio between the stop mask size and the smd pad size (0-100%)
     * @return
     * Number of intersection points 0, 1 or 2
     */
    static int smdStopMaskWireIntersections(const Smd & smd, const Wire & wire, QPointF & intersectionPt1, QPointF & intersectionPt2, qreal stopMaskPercentage);
    static qreal wireAngle(const Wire & wire);
};

#endif // UTILS_H
