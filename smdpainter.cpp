#include "smdpainter.h"

#include "utils.h"
#include "unitutilities.h"

void SmdPainter::draw(QPainter *painter, Smd *smd)
{
    if (smd->roundness() != 0) {

    } else {
        bool mirror = false;
        float rotation = EAGLE_Utils::rotationToDegrees(smd->rot(), &mirror);
        painter->translate(smd->x(), smd->y());
        painter->rotate(rotation);
        painter->drawRect(QRectF(
                    - (smd->dx() / 2.0),
                    - (smd->dy() / 2.0),
                    smd->dx(),
                    smd->dy()));
        painter->rotate(-rotation);
        painter->translate(-smd->x(), -smd->y());
    }
}

void SmdPainter::drawCream(QPainter *painter, Smd *smd, qreal factor)
{
    // TODO implement cream mask factor!!
    if (smd->cream() == Smd::Cream_yes)
        draw(painter, smd);
}

void SmdPainter::drawStopMask(QPainter *painter, Smd *smd, qreal factor)
{
    // TODO implement stop mask factor!!
    if (smd->stop() == Smd::Stop_yes)
        draw(painter, smd);
}
