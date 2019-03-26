#ifndef DRULOADER_H
#define DRULOADER_H


#include <QString>

class DRULoader
{
public:
    DRULoader(const QString druPath = QString(""));

    bool load(const QString druPath);

    qreal stopMaskPercentage() const;
    qreal stopMaskOffsetMinInMm() const;
    qreal stopMaskOffsetMaxInMm() const;

    qreal elongationPercentageForLongPads() const;
    qreal elongationPercentageForOffsetPads() const;

    qreal padTopAnnularRingWidthMinMm() const;
    qreal padTopAnnularRingWidthMaxMm() const;
    qreal padTopAnnularRingPercentage() const;

    qreal padBottomAnnularRingWidthMinMm() const;
    qreal padBottomAnnularRingWidthMaxMm() const;
    qreal padBottomAnnularRingPercentage() const;

private:
    QString m_error;

    qreal m_stopMaskPercentage;
    qreal m_stopMaskOffsetMinInMm;
    qreal m_stopMaskOffsetMaxInMm;

    qreal m_elongationPercentageForLongPads;
    qreal m_elongationPercentageForOffsetPads;

    qreal m_padTopAnnularRingWidthMinMm, m_padTopAnnularRingWidthMaxMm;
    qreal m_padTopAnnularRingPercentage;

    qreal m_padBottomAnnularRingWidthMinMm, m_padBottomAnnularRingWidthMaxMm;
    qreal m_padBottomAnnularRingPercentage;
};

#endif // DRULOADER_H
