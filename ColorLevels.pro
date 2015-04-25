#-------------------------------------------------
#
# Project created by QtCreator 2015-02-28T13:21:39
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ColorLevels
TEMPLATE = app

SOURCES += colorLevels.cpp \
    imageviewer.cpp \
    colorwindow.cpp \


HEADERS  += colorLevels.h \
    imageviewer.h \
    colorwindow.h

QT_CONFIG -= no-pkg-config

CONFIG += c++11

unix: ICON = colorlevels.icns
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += ImageMagick++

win32: RC_FILE = colorlevels.rc
win32: LIBS += -LC:/Users/yxing/Documents/workspace/ImageMagick-6.9.0-Q16/lib/ -lCORE_RL_Magick++_
win32: INCLUDEPATH += C:/Users/yxing/Documents/workspace/ImageMagick-6.9.0-Q16/include
win32: DEPENDPATH += C:/Users/yxing/Documents/workspace/ImageMagick-6.9.0-Q16/include
