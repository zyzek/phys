#-------------------------------------------------
#
# Project created by QtCreator 2016-01-29T23:53:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phys
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        dialog.cpp \
    camera.cpp \
    circle.cpp \
    clock.cpp \
    phys.cpp \
    polygon.cpp \
    spring.cpp \
    vec.cpp \
    world.cpp \
    wpos.cpp \
    collision.cpp \
    cosmogony.cpp \
    gravvector.cpp \
    gravfield.cpp

HEADERS  += dialog.h \
    camera.h \
    circle.h \
    clock.h \
    line.h \
    phys.h \
    polygon.h \
    renderable.h \
    spring.h \
    vec.h \
    world.h \
    wpos.h \
    collision.h \
    cosmogony.h \
    gravvector.h \
    constants.h \
    gravfield.h

FORMS    += dialog.ui

DISTFILES += \
    universe.txt \
    solar_system.txt \
    TODO
