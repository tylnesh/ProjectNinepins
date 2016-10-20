#-------------------------------------------------
#
# Project created by QtCreator 2016-08-13T08:32:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Ninepins_v2
TEMPLATE = app

CONFIG += c++11

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    comm.cpp \
    chooserwindow.cpp \
    fullgamewindow.cpp \
    commthread.cpp \
    global.cpp \
    partialgamewindow.cpp \
    gpioclass.cpp \
    fault.cpp \
    changerwindow.cpp

HEADERS  += mainwindow.h \
    comm.h \
    chooserwindow.h \
    fullgamewindow.h \
    commthread.h \
    global.h \
    partialgamewindow.h \
    gpioclass.h \
    fault.h \
    changerwindow.h

FORMS    += mainwindow.ui \
    chooserwindow.ui \
    fullgamewindow.ui \
    partialgamewindow.ui \
    fault.ui \
    changerwindow.ui

unix:!macx: LIBS += -lrf24

unix:!macx: LIBS += -lrf24

RESOURCES += \
    ninepins.qrc
