#ifndef CDATABASEMANAGER_H
#define CDATABASEMANAGER_H

#include "databasemanager_global.h"
#include <QString>

struct sqlite3;

class DATABASEMANAGERSHARED_EXPORT CDatabaseManager
{

public:
    CDatabaseManager();

public:
    bool open(QString name);
    bool close();

    bool exec(const QString &query);
    QString lastError();

private:
    sqlite3 *m_sqliteDB;
    QString m_errorString;
};

#endif // CDATABASEMANAGER_H
