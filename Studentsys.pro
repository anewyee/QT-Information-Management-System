#-------------------------------------------------
#
# Project created by pyuxing 2018-07-10T17:55:27
#
#-------------------------------------------------


QT       += core gui
QT       += sql
QT       += axcontainer

RC_ICONS = icon_window.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
include (3rdparty/src/xlsx/qtxlsx.pri)



INCLUDEPATH +="framelesswindow"

TARGET = Studentsys
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    connection.cpp \
    DarkStyle.cpp \
    framelesswindow.cpp \
    windowdragger.cpp \
    mymessagebox.cpp \
    mytitlebar.cpp \
    basewindow.cpp

HEADERS  += mainwindow.h \
    connection.h \
    DarkStyle.h \
    framelesswindow.h \
    windowdragger.h \
    mymessagebox.h \
    mytitlebar.h \
    basewindow.h

FORMS    += mainwindow.ui \
    framelesswindow.ui \
    mymessagebox.ui \


RESOURCES   += darkstyle.qrc \
               framelesswindow.qrc \
               mainwindow_icon.qrc \
               testmessagebox.qrc

