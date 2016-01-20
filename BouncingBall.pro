#-------------------------------------------------
#
# Project created by QtCreator 2016-01-12T16:18:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BouncingBall
TEMPLATE = app

CONFIG += c++11


SOURCES += main.cpp\
        dialog.cpp \
    phys.cpp \
    circle.cpp \
    vec.cpp \
    camera.cpp \
    clock.cpp \
    polygon.cpp \
    wpos.cpp \
    spring.cpp

HEADERS  += dialog.h \
    phys.h \
    vec.h \
    camera.h \
    circle.h \
    clock.h \
    polygon.h \
    wpos.h \
    spring.h \
    renderable.h \
    staticpoly.h \
    line.h

FORMS    += dialog.ui
