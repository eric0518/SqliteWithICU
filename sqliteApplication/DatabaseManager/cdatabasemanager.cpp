#include "cdatabasemanager.h"
#include "sqlite3.h"
#include <QBitArray>
#include <QTextCodec>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

static bool s_dbHasPassword = false;

Database20::Database20(QObject *parent)
    :Base(parent)
    ,m_sqliteDB(0)
    ,m_source()
    ,m_errorString("No Error")
    ,m_password()
    ,m_isOpened(false) {
}

Database20::~Database20() {
    setIsOpen(false);
}

bool Database20::transaction() {
    char *errormessage = 0;
    int result = sqlite3_exec(m_sqliteDB, "begin;", NULL, 0, &errormessage);
    if (result != SQLITE_OK) {

        m_errorString = errormessage;
        sqlite3_free(errormessage);
    }

    return (result == SQLITE_OK);
}

bool Database20::rollback() {
    char *errormessage = 0;
    int result = sqlite3_exec(m_sqliteDB, "rollback;", NULL, 0, &errormessage);
    if (result != SQLITE_OK) {

        m_errorString = errormessage;
        sqlite3_free(errormessage);
    }

    return (result == SQLITE_OK);
}

bool Database20::commit() {
    char *errormessage = 0;
    int result = sqlite3_exec(m_sqliteDB, "commit;", NULL, 0, &errormessage);
    if (result != SQLITE_OK) {

        m_errorString = errormessage;
        sqlite3_free(errormessage);
    }

    return (result == SQLITE_OK);
}

QUrl Database20::source() const {
    return m_source;
}

QString Database20::password() const {
    return m_password;
}

bool Database20::isOpen() const {
    return m_isOpened;
}

QStringList Database20::tables() {
    QStringList result;
    if (isOpen()) {
        QString sql = QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
        QVariant tables = exec(sql);
        QList<QVariant> list = tables.toList();

        foreach (QVariant table, list) {
            result.append(table.toMap().values().at(0).toString());
        }
    }

    return result;
}

void Database20::setSource(QUrl source) {
    if (m_source == source) {
        return;
    }

    m_source = source;
    emit sourceChanged(m_source);
}


void Database20::setPassword(QString password) {
    if (m_password == password) {
        return ;
    }

    m_password = password;
    emit passwordChanged(m_password);
}

void Database20::setIsOpen(bool open) {

    if (isOpen() == open) {
        return ;
    }

    bool result = true;
    if (open) {
        result = openDb();
    } else {
        closeDb();
    }

    if (result) {
        emit isOpenChanged(isOpen());
    }
}

QVariant Database20::exec(QString sql, QVariantMap bindValues) {
    QVariant queryResult(false);
    m_errorString = "";

    sqlite3_stmt * statement = NULL;
    const char *tail;

    // prepare
    int result = sqlite3_prepare_v2(m_sqliteDB, sql.toStdString().c_str(), -1, &statement, &tail);
    if (result != SQLITE_OK)
    {
        return queryResult;
    }

    // bind values
    int bindCount = sqlite3_bind_parameter_count(statement);
    // notice : the placeholder index is stated with 1, not 0
    for (int bindIndex=1; bindIndex<=bindCount; bindIndex++) {

        const char * parameterName = sqlite3_bind_parameter_name(statement, bindIndex);
        int parameterIndex = sqlite3_bind_parameter_index(statement, parameterName);
        QVariant  parameterValue = bindValues.value(QString(parameterName));

        qDebug() << "parameterName = " << QString(parameterName);
        qDebug() << "parameterIndex = " << parameterIndex;
        qDebug() << "parameterValue = " << parameterValue;
        bind(statement, bindIndex, parameterValue);
    }

    QVariantList recordList;
    QVariantMap record;

    while( (result = sqlite3_step(statement)) == SQLITE_ROW ) {

        record.clear();
        int cloumnCount = sqlite3_column_count(statement);
        for (int columnIndex = 0; columnIndex < cloumnCount; columnIndex ++) {

            const char *fieldName = sqlite3_column_name(statement, columnIndex);
            const unsigned char *fieldValue = sqlite3_column_text(statement, columnIndex);

            record.insert(QString(fieldName), QString::fromUtf8((char *)fieldValue));
        }
        recordList.append(record);
    }

    sqlite3_finalize(statement);

    // error
    if (result != SQLITE_DONE)
    {
        return queryResult;
    }

    // not select
    if (!recordList.size())
    {
        queryResult = QVariant(true);
        return queryResult;
    }

    // select
    queryResult = QVariant(recordList);
    return queryResult;
}

// private
bool Database20::bind(sqlite3_stmt *statement, const int index, const QVariant value) {
    int result = SQLITE_ERROR;
    switch (value.type()) {

    case QVariant::Bool: {
        result = sqlite3_bind_int(statement, index, value.toBool() ? 1 : 0);
        break;
    }
    case QVariant::Int: {
        result = sqlite3_bind_int(statement, index, value.toInt());
        break;
    }
    case QVariant::UInt: {
        result = sqlite3_bind_int(statement, index, value.toUInt());
        break;
    }
    case QVariant::LongLong: {
        result = sqlite3_bind_int64(statement, index, value.toLongLong());
        break;
    }
    case QVariant::ULongLong: {
        result = sqlite3_bind_int64(statement, index, value.toULongLong());
        break;
    }
    case QVariant::Double: {
        result = sqlite3_bind_double(statement, index, value.toDouble());
        break;
    }
    case QVariant::String: {
        result = sqlite3_bind_text(statement, index, value.toString().toStdString().c_str(), -1, SQLITE_STATIC);
        qDebug() << "bind text = " << value.toString().toStdString().c_str();
        break;
    }
    case QVariant::ByteArray: {
        result = sqlite3_bind_blob(statement, index, value.toByteArray().data(), -1, SQLITE_STATIC);
        break;
    }
    default: {
        break;
    }
    }

    return (result == SQLITE_OK);
}

bool Database20::openDb() {
    if (m_isOpened) {
        return true;
    }

    QString localPath = this->m_source.toLocalFile();
    QFileInfo fileInfo(localPath);
    QDir dir = fileInfo.dir();
    if (!dir.mkpath(dir.absolutePath())) {
        return false;
    }

    QString databaseName = fileInfo.absoluteFilePath() + "/" + m_source.fileName();
    int resust = sqlite3_open(databaseName.toStdString().c_str(), &m_sqliteDB);
    if (resust != SQLITE_OK) {
        m_errorString = sqlite3_errmsg(m_sqliteDB);
    }

    if (s_dbHasPassword) {
        encrypt(m_password);
    }

    m_isOpened = (resust == SQLITE_OK);

    return m_isOpened;
}

void Database20::closeDb() {
    sqlite3_close(m_sqliteDB);
    m_errorString = sqlite3_errmsg(m_sqliteDB);
    m_isOpened = false;
}

bool Database20::encrypt(QString &password) {
    int result = SQLITE_OK;
    if (password.isEmpty()) {
        return false;
    }

    //result = sqlite3_key(m_sqliteDB, password.toStdString().c_str(), (int)password.length());
    m_errorString = sqlite3_errmsg(m_sqliteDB);
    return (result == SQLITE_OK);
}
