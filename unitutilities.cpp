#include "unitutilities.h"

#include <QString>

qreal UnitUtilities::milToMm(qreal mil)
{
    return  mil * 0.0254;
}

qreal UnitUtilities::inchToMm(qreal inch)
{
    return inch * 25.4;
}

qreal UnitUtilities::stringToMm(const QString &val)
{
    qreal ret = 0;
    if (val.endsWith("mil")) {
        bool ok = false;
        ret = val.left(val.length() - 3).toDouble(&ok);
        if (!ok)
            ret = 0;
        else
            ret = milToMm(ret);
    } else if (val.endsWith("inch")) {
        bool ok = false;
        ret = val.left(val.length() - 4).toDouble(&ok);
        if (!ok)
            ret = 0;
        else
            ret = milToMm(ret);
    } else {
        bool ok = false;
        ret = val.toDouble(&ok);
        if (!ok)
            ret = 0;
    }

    return ret;
}
