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
     * @param internalPoint
     * if the line has 1 intersction with the stop mask then it will be
     * set to the line end which is inside the stop mask.
     * If the line has no intersection or has two intersection
     * with the stop mask it will not be changed.
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
    static int smdStopMaskWireIntersections(const Smd & smd,
                                            const Wire &wire,
                                            QPointF *internalPoint,
                                            QPointF *intersectionPt1,
                                            QPointF *intersectionPt2,
                                            qreal stopMaskPercentage);


    enum SeparationStartLineEnd {
        End1,
        End2
    };
    /**
     * @brief splitWire
     * This function will break the wire specified in the wire parameter
     * into 2 or 3 wires (depending on the separation points).
     * The returned wires will have the same parameters (layer, width, etc.)
     *
     * @param wire
     * The wire to be separated
     *
     * @param separationPoints
     * The wire is going to be separated in interSectionKeepOutInMm distance
     * from these these points in the direction closer to the startEnd
     *
     * @param interSectionKeepOutInMm
     * The separation will happen on the line closer to the startEnd with
     * this distance from the separationPoints
     *
     * @param startEnd
     * Specifies which end of the line should the separation started
     *
     * @return
     * A list of the separated wires.
     * The returned wires will have the same parameters (layer, width, etc.)
     */
    static QList<Wire> splitWire(const Wire & wire,
                                  const QList<QPointF> separationPoints,
                                  qreal interSectionKeepOutInMm,
                                  SeparationStartLineEnd startEnd = End1);
    static qreal wireAngle(const Wire & wire);
};

#endif // UTILS_H
