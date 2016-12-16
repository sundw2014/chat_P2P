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
    sessionworkerthread.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    sessionswindow.h \
    common.h \
    updatefriendslistthread.h \
    sessionworkerthread.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    sessionswindow.ui

CONFIG += c++11
