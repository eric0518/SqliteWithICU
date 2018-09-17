#include "cdatabasetest.h"
#include "cdatabasemanager.h"
#include <QtTest>
#include <QUuid>
#include <QDebug>

CDatabaseTest::CDatabaseTest(QObject *parent) : QObject(parent)
{

}

void CDatabaseTest::initTestCase()
{
}

void CDatabaseTest::cleanupTestCase()
{
}

void CDatabaseTest::setIsOpenTest()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    QTRY_COMPARE(database20.isOpen(), true);

    database20.setIsOpen(false);
    QTRY_COMPARE(database20.isOpen(), false);
}

void CDatabaseTest::setSourceTest()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    QUrl url(dbName);
    database20.setSource(url);

    QTRY_COMPARE(database20.source(), url);
}

void CDatabaseTest::setPasswordTest()
{
    Database20 database20;
    QString password = "China123";
    database20.setPassword(password);

    QTRY_COMPARE(database20.password(), password);
}

void CDatabaseTest::execTestWithCreateTable()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    QStringList tableList = QStringList() << "Test"
                                          << "Test_config"
                                          << "Test_content"
                                          << "Test_data"
                                          << "Test_docsize"
                                          << "Test_idx";
    database20.exec(sql);
    QStringList foundTables = database20.tables();

    QTRY_COMPARE(foundTables.contains(tableList.at(0), Qt::CaseInsensitive), true);
    QTRY_COMPARE(foundTables.contains(tableList.at(1), Qt::CaseInsensitive), true);
    QTRY_COMPARE(foundTables.contains(tableList.at(2), Qt::CaseInsensitive), true);
    QTRY_COMPARE(foundTables.contains(tableList.at(3), Qt::CaseInsensitive), true);
    QTRY_COMPARE(foundTables.contains(tableList.at(4), Qt::CaseInsensitive), true);
    QTRY_COMPARE(foundTables.contains(tableList.at(5), Qt::CaseInsensitive), true);

    database20.setIsOpen(false);
}

void CDatabaseTest::execTestWithInsert()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert
    sql = QStringLiteral("insert into Test values('中华人民共和国你好');");
    QVariant result = database20.exec(sql);

    QTRY_COMPARE(result.toBool(), true);
}

void CDatabaseTest::execTestWithInsertBind()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert
    sql = QStringLiteral("INSERT INTO Test (content) VALUES (:content);");
    QVariantMap map;
    map.insert(QStringLiteral(":content"), QStringLiteral("中华人民共和国你好哈哈哈"));
    QVariant result = database20.exec(sql, map);

    QTRY_COMPARE(result.toBool(), true);
}

void CDatabaseTest::execTestWithCount()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert
    sql = QStringLiteral("insert into Test values('中华人民共和国你好');");
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);

    // count
    sql = QStringLiteral("SELECT COUNT(*) FROM Test;");
    QVariant result = database20.exec(sql);

    QTRY_COMPARE(result.toList().at(0).toMap().value("COUNT(*)").toInt(), 5);
}

void CDatabaseTest::execTestWithInsertByTransactionWithSuccessed()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert with transaction
    bool execResult = true;
    bool commitResult = false;
    bool rollbackResult = false;

    QVariant transactionResult = database20.transaction();
    sql = QStringLiteral("insert into Test values('中华人民共和国你好');");
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    if (execResult)
    {
        commitResult =  database20.commit();
    }
    else
    {
        rollbackResult = database20.rollback();
    }

    QTRY_COMPARE(transactionResult.toBool(), true);
    QTRY_COMPARE(execResult, true);
    QTRY_COMPARE(commitResult, true);
    QTRY_COMPARE(rollbackResult, false);
}

void CDatabaseTest::execTestWithInsertByTransactionWithFailed()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert with transaction
    bool execResult = true;
    bool commitResult = false;
    bool rollbackResult = false;

    QVariant transactionResult = database20.transaction();
    sql = QStringLiteral("insert into Test values('中华人民共和国你好');");
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= false;
    if (execResult)
    {
        commitResult =  database20.commit();
    }
    else
    {
        rollbackResult = database20.rollback();
    }

    QTRY_COMPARE(transactionResult.toBool(), true);
    QTRY_COMPARE(execResult, false);
    QTRY_COMPARE(commitResult, false);
    QTRY_COMPARE(rollbackResult, true);
}

void CDatabaseTest::exetTestWithInsertByAssembledStatement()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert
    sql = QString(QStringLiteral("INSERT INTO Test (content) VALUES (\'%1\');")).arg(QStringLiteral("中华人民共和国你好哈哈哈"));
    QVariant result = database20.exec(sql);

    QTRY_COMPARE(result.toBool(), true);
}

void CDatabaseTest::execTestWithSelect()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert
    sql = QStringLiteral("insert into Test values('中华人民共和国你好');");
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);

    // select
    sql = "SELECT content FROM Test;";
    QVariant result = database20.exec(sql);

    QTRY_COMPARE(result.toList().size(), 5);
    QTRY_COMPARE(result.toList().at(0).toMap().value("content").toString(), QStringLiteral("中华人民共和国你好"));
    QTRY_COMPARE(result.toList().at(1).toMap().value("content").toString(), QStringLiteral("中华人民共和国你好"));
    QTRY_COMPARE(result.toList().at(2).toMap().value("content").toString(), QStringLiteral("中华人民共和国你好"));
    QTRY_COMPARE(result.toList().at(3).toMap().value("content").toString(), QStringLiteral("中华人民共和国你好"));
    QTRY_COMPARE(result.toList().at(4).toMap().value("content").toString(), QStringLiteral("中华人民共和国你好"));
}

void CDatabaseTest::execTestWithMatch()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert
    sql = QStringLiteral("insert into Test values('中华人民共和国你好');");
    database20.exec(sql);
    database20.exec(sql);

    sql = QStringLiteral("insert into Test values('美利坚合众国你好');");
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);

    // match
    sql = QStringLiteral("SELECT * from Test where content MATCH '中'");
    QVariant result = database20.exec(sql);

    QTRY_COMPARE(result.toList().size(), 2);
    QTRY_COMPARE(result.toList().at(0).toMap().value("content").toString(), QStringLiteral("中华人民共和国你好"));
    QTRY_COMPARE(result.toList().at(1).toMap().value("content").toString(), QStringLiteral("中华人民共和国你好"));
}

void CDatabaseTest::execTestWithDelete()
{
    Database20 database20;
    QString dbName = QUuid::createUuid().toString() + ".db";
    database20.setSource(QUrl(dbName));
    database20.setIsOpen(true);

    // create
    QString sql = "CREATE VIRTUAL TABLE Test USING FTS5(content, tokenize = 'mmicu');";
    database20.exec(sql);

    // insert
    sql = QStringLiteral("insert into Test values('中华人民共和国你好');");
    database20.exec(sql);
    database20.exec(sql);

    sql = QStringLiteral("insert into Test values('美利坚合众国你好');");
    database20.exec(sql);
    database20.exec(sql);
    database20.exec(sql);

    // delete
    sql = QStringLiteral("DELETE FROM Test WHERE content LIKE '%美利坚%';");
    QVariant result = database20.exec(sql);

    QTRY_COMPARE(result.toBool(), true);
}



