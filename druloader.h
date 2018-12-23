#ifndef DRULOADER_H
#define DRULOADER_H


#include <QString>

class DRULoader
{
public:
    DRULoader(const QString druPath = QString(""));

    bool load(const QString druPath);
private:
    QString m_error;
};

#endif // DRULOADER_H
