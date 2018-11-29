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
    ../GOTURN/src/network/regressor.cpp \
    ../GOTURN/src/network/regressor_base.cpp \
    ../GOTURN/src/network/regressor_train.cpp \
    ../GOTURN/src/network/regressor_train_base.cpp \
    ../GOTURN/src/tracker/tracker.cpp \
#    ../GOTURN/src/tracker/tracker_manager.cpp \
    ../GOTURN/src/helper/bounding_box.cpp \
    ../GOTURN/src/helper/image_proc.cpp \
    ../GOTURN/src/helper/helper.cpp



HEADERS  += mainwindow.h \
    imageio.h \
    boxTracker.h \
    trackerImageviewer.h \
    ../GOTURN/src/network/regressor.h \
    ../GOTURN/src/network/regressor_base.h \
    ../GOTURN/src/network/regressor_train.h \
    ../GOTURN/src/network/regressor_train_base.h \
    ../GOTURN/src/tracker/tracker.h \
#    ../GOTURN/src/tracker/tracker_manager.h \
    ../GOTURN/src/helper/bounding_box.h \
    ../GOTURN/src/helper/image_proc.h \
    ../GOTURN/src/helper/helper.h


FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/include/opencv /usr/local/include /home/julle/trackerProg/GOTURN/src /usr/local/cuda-9.0/targets/x86_64-linux/include
LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_video -lopencv_tracking -lopencv_highgui -lcaffe -lboost_system -lboost_filesystem -lboost_regex -lglog

QMAKE_CXXFLAGS += -std=c++0x
