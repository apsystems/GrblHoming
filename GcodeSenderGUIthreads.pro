#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T17:48:40
#
#-------------------------------------------------

QT       += core gui

TARGET = GcodeSenderGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rs232.cpp \
    readthread.cpp \
    options.cpp \
    grbldialog.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    rs232.h \
    readthread.h \
    options.h \
    grbldialog.h \
    definitions.h \
    about.h \
    images.rcc

FORMS    += mainwindow.ui \
    options.ui \
    grbldialog.ui \
    about.ui

RESOURCES += \
    images.qrc
