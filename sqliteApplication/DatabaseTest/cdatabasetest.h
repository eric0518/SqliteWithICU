#ifndef CDATABASETEST_H
#define CDATABASETEST_H

#include <QObject>

class CDatabaseTest : public QObject
{
    Q_OBJECT
public:
    explicit CDatabaseTest(QObject *parent = nullptr);

private slots:
    void initTestCase();
    void cleanupTestCase();

    void setIsOpenTest();
    void setSourceTest();
    void setPasswordTest();
    void execTestWithCreateTable();
    void execTestWithInsert();
    void execTestWithInsertBind();
    void execTestWithCount();
    void execTestWithInsertByTransactionWithSuccessed();
    void execTestWithInsertByTransactionWithFailed();
    void exetTestWithInsertByAssembledStatement();
    void execTestWithSelect();
    void execTestWithMatch();
    void execTestWithDelete();
};

#endif // CDATABASETEST_H
