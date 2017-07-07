#-------------------------------------------------
#
# Project created by QtCreator 2017-06-29T17:30:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = galleryView
TEMPLATE = app


INCLUDEPATH +=$$PWD base
include(base/base.pri)

INCLUDEPATH +=$$PWD gallery
include(gallery/gallery.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    global_value.cpp \
    inotifythread.cpp \
    ueventthread.cpp

HEADERS  += mainwindow.h \
    global_value.h \
    inotifythread.h \
    ueventthread.h

RESOURCES += \
    res_gallery.qrc \
    res_main.qrc
