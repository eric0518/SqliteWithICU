#include "mainwindow.h"
#include <QApplication>

#include <stdio.h>
#include <stdlib.h>
#include "cdatabasemanager.h"

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

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    CDatabaseManager databaseManager;
    if (!databaseManager.open("xxxtest.db")) {

        qDebug() << "ERROR :" << databaseManager.lastError();
    }

    QString sql = "CREATE VIRTUAL TABLE KKK USING FTS5(content, tokenize = 'mmicu');";
    if (!databaseManager.exec(sql)) {

        qDebug() << "ERROR :" << databaseManager.lastError();
    }

    sql = QStringLiteral("insert into KKK values('中华人民共和国你好');");
    if (!databaseManager.exec(sql)) {

        qDebug() << "ERROR :" << databaseManager.lastError();
    }

    sql = QStringLiteral("SELECT * from KKK where content MATCH '你'");
    if (!databaseManager.exec(sql)) {

        qDebug() << "ERROR :" << databaseManager.lastError();
    }

    if (!databaseManager.close()) {

        qDebug() << "ERROR :" << databaseManager.lastError();
    }

    /*
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    const char* data = "Callback function called";


    // testign for open
    rc = sqlite3_open("testxxx.db", &db);
    if (rc)
    {
        qDebug() << "Can't open database:" << sqlite3_errmsg(db);
        return 0;
    }
    else
    {
        qDebug() << "Opened database successfully";
    }

    // register
    rc = sqlite3_register_mm_utils(db);
    if (rc != SQLITE_OK)
    {
        qDebug() << "sqlite3_register_mm_utils error";
    }
    else
    {
        qDebug() << "sqlite3_register_mm_utils successfully";
    }


    const char *sql = "CREATE VIRTUAL TABLE KKK USING FTS5(content, tokenize = 'mmicu');";
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        qDebug() << "SQL error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }
    else
    {
        qDebug() << "Table created successfully.";
    }

    // insert row
    // QString sqlText = QStringLiteral("insert into KKK values('111 台湾（Taiwan），位于中国大陆东南沿海的大陆架上，东临太平洋，东北邻琉球群岛，南界巴士海峡与菲律宾群岛相对，西隔台湾海峡与福建省相望， [1]  总面积约3.6万平方千米，包括台湾岛及兰屿、绿岛、钓鱼岛等21个附属岛屿和澎湖列岛64个岛屿。台湾岛面积35882.6258平方千米，是中国第一大岛， [2]  7成为山地与丘陵，平原主要集中于西部沿海，地形海拔变化大。由于地处热带及亚热带气候之交界，自然景观与生态资源丰富。人口约2350万，逾7成集中于西部5大都会区，其中以首要都市台北为中心的台北都会区最大。');");
    QString sqlText = QStringLiteral("insert into KKK values('中华人民共和国你好');");
    //sql = "insert into KKK values('位于中国大陆东南沿海的大陆架上');";
    rc = sqlite3_exec(db, sqlText.toStdString().c_str(), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        qDebug() << "insert error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }
    else {
        qDebug() << "Insert successfully";
    }


    // select
    sqlText = QStringLiteral("SELECT * from KKK where content MATCH '你'");
    sql = sqlText.toStdString().c_str();
    qDebug() << "sql : " << sqlText.toStdString().c_str();
    rc = sqlite3_exec(db, sqlText.toStdString().c_str(), callback, (void*)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        qDebug() << "MATCH error: " << zErrMsg;
        sqlite3_free(zErrMsg);
    }
    else {
        qDebug() << "MATCH  successfully";
    }

    sqlite3_close(db);

    */
    return a.exec();
}


