#-------------------------------------------------
#
# Project created by QtCreator 2018-03-14T10:56:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZFfire_10_cameras
TEMPLATE = app

QMAKE_CXXFLAGS += /openmp

SOURCES += main.cpp\
        mainwindow.cpp \
    cqtopencvviewergl.cpp \
    tinyxml2.cpp \
    dialog_config.cpp

HEADERS  += mainwindow.h \
    cqtopencvviewergl.h \
    tinyxml2.h \
    dialog_config.h

INCLUDEPATH+=d:/opencv/build/include/opencv \
            d:/opencv/build/include/opencv2 \
            d:/opencv/build/include



LIBS+=D:\opencv\build\x64\vc14\lib\opencv_world331.lib \


FORMS    += mainwindow.ui \
    dialog_config.ui
