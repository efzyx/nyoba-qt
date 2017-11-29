#-------------------------------------------------
#
# Project created by QtCreator 2017-11-28T00:13:35
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cobadb
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        login.cpp \
    mainwindow.cpp \
    datadosen.cpp \
    datamahasiswa.cpp \
    matakuliah.cpp \
    datauser.cpp \
    nilai.cpp

HEADERS += \
        login.h \
    mysqlconnect.h \
    mainwindow.h \
    datadosen.h \
    datamahasiswa.h \
    matakuliah.h \
    datauser.h \
    nilai.h

FORMS += \
        login.ui \
    mainwindow.ui \
    datadosen.ui \
    datamahasiswa.ui \
    matakuliah.ui \
    datauser.ui \
    nilai.ui

win32: LIBS += -LE:/Qt/mysql-connector/mysql-connector-c-6.1.11-win32/lib/ -llibmysql

INCLUDEPATH += E:/Qt/mysql-connector/mysql-connector-c-6.1.11-win32/include
DEPENDPATH += E:/Qt/mysql-connector/mysql-connector-c-6.1.11-win32/include
