#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T16:16:34
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fakeXin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    sessionswindow.cpp \
    updatefriendslistthread.cpp \
    sessionworkerthread.cpp \
    sessiontab.cpp \
    selectfriendsdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    sessionswindow.h \
    common.h \
    updatefriendslistthread.h \
    sessionworkerthread.h \
    sessiontab.h \
    selectfriendsdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    sessionswindow.ui \
    sessiontab.ui \
    selectfriendsdialog.ui

CONFIG += c++11

RESOURCES += \
    r.qrc
