#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T11:26:48
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_databasetesttest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += C:/Dev/sqlite3/include \
               $${PWD}/../DatabaseManager


LIBS += -L. -LC:/Dev/sqlite3/lib -lsqlite3
LIBS +=-L. -LC:/Dev/icu/lib -licuuc -licutu -licuin
LIBS += -L. -L$${PWD}/../DatabaseApplication/databasemanager/bin -lDatabaseManager

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    cdatabasetest.cpp \
    main.cpp

HEADERS += \
    cdatabasetest.h
