#-------------------------------------------------
#
# Project created by QtCreator 2018-03-16T17:19:01
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PeryiteUI
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
        mainwindow.cpp \
    peryiteuidata.cpp \
    geogenvisualization.cpp \
    visualizationcircle.cpp \
    geogendata.cpp \
    geogridlocation.cpp \
    util.cpp \
    popgenvisualization.cpp \
    visualizationgraphbar.cpp \
    popgendata.cpp \
    testingwindow.cpp \
    testingscattergraph.cpp \
    mapviewerwindow.cpp \
    editconfigform.cpp

HEADERS += \
        mainwindow.h \
    peryiteuidata.h \
    geogenvisualization.h \
    visualizationcircle.h \
    geogendata.h \
    geogridlocation.h \
    util.h \
    popgenvisualization.h \
    visualizationgraphbar.h \
    popgendata.h \
    testingwindow.h \
    testingscattergraph.h \
    mapviewerwindow.h \
    editconfigform.h

FORMS += \
        mainwindow.ui \
    geogenvisualization.ui \
    popgenvisualization.ui \
    testingwindow.ui \
    testingscattergraph.ui \
    mapviewerwindow.ui \
    editconfigform.ui

DISTFILES +=
