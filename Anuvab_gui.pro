#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T10:49:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Anuvab_gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lbptopsettings.cpp \
    exprpathsettings.cpp \
    facetracker.cpp \
    facebuffer.cpp \
    lbptop.cpp \
    pattern.cpp \
    classifier.cpp

HEADERS  += mainwindow.h \
    lbptopsettings.h \
    exprpathsettings.h \
    facetracker.h \
    global.h \
    facebuffer.h \
    lbptop.h \
    pattern.h \
    classifier.h

FORMS    += mainwindow.ui \
    lbptopsettings.ui \
    exprpathsettings.ui \
    loadingdialog.ui

INCLUDEPATH += C:\opencv\build\include
LIBS += opencv_calib3d243.lib opencv_contrib243.lib opencv_core243.lib opencv_features2d243.lib opencv_flann243.lib opencv_gpu243.lib opencv_haartraining_engine.lib opencv_highgui243.lib opencv_imgproc243.lib opencv_legacy243.lib opencv_ml243.lib opencv_nonfree243.lib opencv_objdetect243.lib opencv_photo243.lib opencv_stitching243.lib opencv_ts243.lib opencv_video243.lib opencv_videostab243.lib
