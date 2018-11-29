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
    trackerImageviewer.cpp \
    horizontDetector.cpp \
    horizontCorrector.cpp \
    imgpreprocessing.cpp

HEADERS  += mainwindow.h \
    imageio.h \
    boxTracker.h \
    trackerImageviewer.h \
    horizontDetector.h \
    horizontCorrector.h \
    imgpreprocessing.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv /usr/local/include
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_video -lopencv_tracking -lopencv_highgui

QMAKE_CXXFLAGS += -std=c++0x

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE += -O3
