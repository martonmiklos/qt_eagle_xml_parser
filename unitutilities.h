#ifndef UNITUTILITIES_H
#define UNITUTILITIES_H

#include <qmath.h>

class UnitUtilities
{
public:
    static inline qreal milToMm(qreal mil);
    static inline qreal inchToMm(qreal inch);
    static qreal stringToMm(const QString & val);

};

#endif // UNITUTILITIES_H
