#include "mainwindow.h"
#include <QApplication>

#include <stdio.h>
#include <stdlib.h>
#include "cdatabasemanager.h"

#include <QDebug>
#include <QTextCodec>
#include "sqlite3.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    // open
    Database20 database20;
    database20.setSource(QUrl("D:/gitcopy/SqliteWithICU.git/trunk/sqliteApplication-build/DatabaseApplication/kkk.db"));
    database20.setIsOpen(true);

    // create virtual table
    QString sql = "CREATE VIRTUAL TABLE KKK USING FTS5(content, tokenize = 'mmicu');";
    qDebug() << "create table : " << database20.exec(sql);

    // insert
    sql = QStringLiteral("insert into KKK values('中华人民共和国你好');");
    qDebug() << "insert : " << database20.exec(sql);

    // match
    sql = QStringLiteral("SELECT * from KKK where content MATCH '中华'");
    qDebug() << "match : " << database20.exec(sql);

    // tables
    sql = QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
    qDebug() << "match : " << database20.exec(sql);
    qDebug() << "tables = " << database20.tables();

    // count
    sql = QStringLiteral("SELECT COUNT(*) from KKK;");
    qDebug() << "count = " << database20.exec(sql).toList().at(0).toMap().value("COUNT(*)").toInt();

    // trancaction
    bool execResult = true;
    qDebug() << "transaction : " << database20.transaction();
    sql = QStringLiteral("insert into KKK values('中华人民共和国你好');");

    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    execResult &= database20.exec(sql).toBool();
    if (execResult)
    {
        qDebug() << "commit : " << database20.commit();
    }
    else
    {
        qDebug() << "rollback : " << database20.rollback();
    }

    // close
    database20.setIsOpen(false);

    return a.exec();
}


