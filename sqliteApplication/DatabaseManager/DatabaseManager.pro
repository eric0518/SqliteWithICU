#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T17:47:03
#
#-------------------------------------------------

QT       -= gui

TARGET = DatabaseManager
TEMPLATE = lib

DEFINES += DATABASEMANAGER_LIBRARY

DESTDIR = $${PWD}/../DatabaseApplication\databasemanager\bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += C:/Dev/sqlite3/include \
               $${PWD}/../icu/include


LIBS += -L. -LC:/Dev/sqlite3/lib -lsqlite3
LIBS +=-L. -LC:/Dev/icu/lib -licuuc -licutu -licuin

SOURCES += \
        cdatabasemanager.cpp

HEADERS += \
        cdatabasemanager.h \
        databasemanager_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
