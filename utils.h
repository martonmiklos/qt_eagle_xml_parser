#ifndef UTILS_H
#define UTILS_H

#include <QLineF>
#include <QPainterPath>
#include <QRectF>
#include <QString>

#include "druloader.h"
#include "eagle.h"

class EAGLE_Utils
{
public:
    static QLineF wire2QLine(const Wire & wire);
    static QRectF smdToQRectF(const Smd & smd);
    static void QLineF_ToWire(const QLineF & line, Wire *wire);

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
     * @param dru
     * DRULoader class with loaded DRC informations
     * @return
     * Number of intersection points 0, 1 or 2
     */
    static int smdStopMaskWireIntersections(const Smd & smd,
                                            const Wire &wire,
                                            QPointF *internalPoint,
                                            QPointF *intersectionPt1,
                                            QPointF *intersectionPt2,
                                            DRULoader *dru);
    /**
     * @brief painterPathLineIntersections
     * Get intersection points of an SMD pad and a Wire
     * @param pad
     * TH pad to be checked
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
    static int padStopMaskWireIntersections(const Pad & pad,
                                            const Wire &wire,
                                            QPointF *internalPoint,
                                            QPointF *intersectionPt1,
                                            QPointF *intersectionPt2,
                                            DRULoader *dru);

    /**
     * @brief painterPathLineIntersections
     * Get intersection points of an SMD pad and a Wire
     * @param rect
     * Rectange (on stop layers) to be checked
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
     * @return
     * Number of intersection points 0, 1 or 2
     */
    static int rectWireIntersections(const Rectangle & rect,
                                            const Wire &wire,
                                            QPointF *internalPoint,
                                            QPointF *intersectionPt1,
                                            QPointF *intersectionPt2);

    static qreal wireAngle(const Wire & wire);
    static QPainterPath padShapeToPainterPath(const Pad &pad, qreal diameter, DRULoader *dru);
    static QPainterPath viaShapeToPainterPath(const Via &via);
    static float rotationToDegrees(const QString& rotaion, bool* mirrored = nullptr, bool *spin = nullptr);
    static QRectF boardBoundingRect(Board* brd);

private:
    static QPainterPath smdToStopMaskPainterPath(const Smd &smd, DRULoader *dru);
    static QPainterPath padToStopMaskPainterPath(const Pad &pad, DRULoader *dru);
    static int painterPathWireIntersections(const QPainterPath & path,
                                            const QPointF &pathCenter,
                                            const Wire &wire,
                                            QPointF *internalPoint,
                                            QPointF *intersectionPt1,
                                            QPointF *intersectionPt2);
};

#endif // UTILS_H
