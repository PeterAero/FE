#-------------------------------------------------
#
# Project created by QtCreator 2017-12-09T13:31:19
#
#-------------------------------------------------

QT       += core

QT       -= gui

INCLUDEPATH += /usr/include/gdal
INCLUDEPATH += c++/data_management
LIBS += -L/usr/lib -lgdal

TARGET = RW
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    c++/data_management/datamanager.cpp \
    c++/image_proc/imageproc.cpp

HEADERS += \
    c++/data_management/datamanager.h \
    c++/image_proc/imageproc.h
