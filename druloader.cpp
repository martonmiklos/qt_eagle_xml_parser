#include "druloader.h"

#include <QObject>
#include <QSettings>

DRULoader::DRULoader(const QString druPath)
{
    if (druPath.isEmpty()) {
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

    return true;
}
