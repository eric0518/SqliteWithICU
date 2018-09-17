#include "cdatabasetest.h"
#include <QTest>

int main(int argc, char *argv[])
{
    CDatabaseTest databaseTest;
    return QTest::qExec(&databaseTest, argc, argv);
}
