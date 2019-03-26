#include "druloader.h"

#include <QObject>
#include <QSettings>

#include "unitutilities.h"

DRULoader::DRULoader(const QString druPath)
{
    if (!druPath.isEmpty()) {
        load(druPath);
    }
}

bool DRULoader::load(const QString druPath)
{
    m_error.clear();

    QSettings dru(druPath, QSettings::IniFormat);
    switch (dru.status()) {
    case QSettings::NoError:
        break;
    case QSettings::AccessError:
        m_error = QObject::tr("Access error");
        return false;
    case QSettings::FormatError:
        m_error = QObject::tr("Format error");
        return false;
    }
    m_stopMaskPercentage = dru.value("mvStopFrame", 1.0).toDouble() * 100.0;
    m_stopMaskOffsetMinInMm = UnitUtilities::stringToMm(dru.value("mlMinStopFrame", "4mil").toString());
    m_stopMaskOffsetMaxInMm = UnitUtilities::stringToMm(dru.value("mlMaxStopFrame", "4mil").toString());

    m_elongationPercentageForLongPads = dru.value("psElongationLong", 100).toDouble();
    m_elongationPercentageForOffsetPads = dru.value("psElongationOffset", 100).toDouble();

    m_padTopAnnularRingPercentage = dru.value("rvPadTop", 0.25).toDouble() * 100.0;
    m_padBottomAnnularRingPercentage = dru.value("rvPadBottom", 0.25).toDouble() * 100.0;

    m_padTopAnnularRingWidthMinMm = UnitUtilities::stringToMm(dru.value("rlMinPadTop", "0.25").toString());
    m_padTopAnnularRingWidthMaxMm = UnitUtilities::stringToMm(dru.value("rlMaxPadTop", "0.25").toString());

    m_padBottomAnnularRingWidthMinMm = UnitUtilities::stringToMm(dru.value("rlMinPadBottom", "0.25").toString());
    m_padBottomAnnularRingWidthMaxMm = UnitUtilities::stringToMm(dru.value("rlMaxPadBottom", "0.25").toString());

    return true;
}

qreal DRULoader::stopMaskPercentage() const
{
    return m_stopMaskPercentage;
}

qreal DRULoader::stopMaskOffsetMinInMm() const
{
    return m_stopMaskOffsetMinInMm;
}

qreal DRULoader::stopMaskOffsetMaxInMm() const
{
    return m_stopMaskOffsetMaxInMm;
}

qreal DRULoader::elongationPercentageForLongPads() const
{
    return m_elongationPercentageForLongPads;
}

qreal DRULoader::elongationPercentageForOffsetPads() const
{
    return m_elongationPercentageForOffsetPads;
}

qreal DRULoader::padTopAnnularRingWidthMinMm() const
{
    return m_padTopAnnularRingWidthMinMm;
}

qreal DRULoader::padTopAnnularRingWidthMaxMm() const
{
    return m_padTopAnnularRingWidthMaxMm;
}

qreal DRULoader::padTopAnnularRingPercentage() const
{
    return m_padTopAnnularRingPercentage;
}

qreal DRULoader::padBottomAnnularRingWidthMinMm() const
{
    return m_padBottomAnnularRingWidthMinMm;
}

qreal DRULoader::padBottomAnnularRingWidthMaxMm() const
{
    return m_padBottomAnnularRingWidthMaxMm;
}

qreal DRULoader::padBottomAnnularRingPercentage() const
{
    return m_padBottomAnnularRingPercentage;
}


