#include "cdatabasemanager.h"
#include "sqlite3.h"
#include <QDebug>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{

    Q_UNUSED(NotUsed)
    int i;
    qDebug() << "-------------------------------------------------";
    for (i = 0; i<argc; i++)
    {
        qDebug() << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL");
    }
    qDebug() << "-------------------------------------------------";

    return 0;
}

CDatabaseManager::CDatabaseManager()
{
    m_errorString = "No error";
}

// public:
bool CDatabaseManager::open(QString name) {

    int resust = sqlite3_open(name.toStdString().c_str(), &m_sqliteDB);
    m_errorString = sqlite3_errmsg(m_sqliteDB);

    return (resust == SQLITE_OK);
}

void CDatabaseManager::close()
{
    int resust = sqlite3_close(m_sqliteDB);
    m_errorString = sqlite3_errmsg(m_sqliteDB);

    return (resust == SQLITE_OK);
}

QString CDatabaseManager::lastError() {
    return m_errorString;
}

bool CDatabaseManager::exec(const QString &query) {

    char *errorMessage = 0;
    m_errorString = "";

    int result = sqlite3_exec(m_sqliteDB, query.toStdString().c_str(), callback, 0, &errorMessage);
    if (result != SQLITE_OK) {

        m_errorString = errorMessage;
        sqlite3_free(errorMessage);
    }

    return (result == SQLITE_OK);
}
