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
    colorwindow.cpp

HEADERS  += colorLevels.h \
    imageviewer.h \
    colorwindow.h

QT_CONFIG -= no-pkg-config

CONFIG += c++11

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += ImageMagick++

QMAKE_CXXFLAGS -= -stdlib=libc++
QMAKE_CXXFLAGS += -stdlib=libstdc++
QMAKE_LFLAGS -= -stdlib=libc++
QMAKE_LFLAGS += -stdlib=libstdc++
