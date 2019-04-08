#include "smdpainter.h"

#include "utils.h"
#include "unitutilities.h"

void SmdPainter::draw(QPainter *painter, Smd *smd)
{
    if (smd->roundness() == 0) {

    } else {
        bool mirror = false;
        float rotation = EAGLE_Utils::rotationToDegrees(smd->rot(), &mirror);
        painter->translate(UnitUtilities::mmToU(smd->x()), UnitUtilities::mmToU(smd->y()));
        painter->rotate(rotation);
        painter->drawRect(
                    UnitUtilities::mmToU(- (smd->dx() / 2.0)),
                    UnitUtilities::mmToU(- (smd->dy() / 2.0)),
                    UnitUtilities::mmToU(smd->dx()),
                    UnitUtilities::mmToU(smd->dy()));
        painter->rotate(-rotation);
        painter->translate(-UnitUtilities::mmToU(smd->x()), -UnitUtilities::mmToU(smd->y()));
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
