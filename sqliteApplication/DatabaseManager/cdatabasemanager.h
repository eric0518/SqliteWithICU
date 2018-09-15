#ifndef CDATABASEMANAGER_H
#define CDATABASEMANAGER_H

#include "databasemanager_global.h"
#include <QString>
#include <QVariant>
#include <QUrl>

struct sqlite3;
struct sqlite3_stmt;

class DATABASEMANAGERSHARED_EXPORT Database20 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(bool isOpen READ isOpen WRITE setIsOpen NOTIFY isOpenChanged)
    Q_PROPERTY(QStringList tables READ tables)

public:
    explicit Database20(QObject *parent = nullptr);
    ~Database20();
    Q_DISABLE_COPY(Database20)

public slots:
    bool transaction();
    bool rollback();
    bool commit();
    QVariant exec(QString cmd, QVariantMap bindValues = QVariantMap());

public:
    QUrl source() const;
    QString password() const;
    bool isOpen() const;
    QStringList tables();

public slots:
    void setSource(QUrl source);
    void setPassword(QString password);
    void setIsOpen(bool isOpen);

signals:
    void sourceChanged(QUrl source);
    void passwordChanged(QString password);
    void isOpenChanged(bool isOpen);

private:
    typedef QObject Base;

private:
    bool openDb();
    void closeDb();

    // added by Eric
    bool encrypt(QString &password);
    bool bind(sqlite3_stmt *statement, const int index, const QVariant value);

private:
    QUrl m_source;
    QString m_password;

    // added by Eric
    sqlite3 *m_sqliteDB;
    QString m_errorString;
    bool m_isOpened;
};

#endif // CDATABASEMANAGER_H
