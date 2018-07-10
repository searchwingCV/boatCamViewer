#-------------------------------------------------
#
# Project created by QtCreator 2018-05-22T20:26:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtCamTrackerUi
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    imageio.cpp \
    boxTracker.cpp \
    trackerImageviewer.cpp

HEADERS  += mainwindow.h \
    imageio.h \
    boxTracker.h \
    trackerImageviewer.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv /usr/local/include
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_video -lopencv_tracking

QMAKE_CXXFLAGS += -std=c++0x
