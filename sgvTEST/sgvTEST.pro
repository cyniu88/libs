#-------------------------------------------------
#
# Project created by QtCreator 2016-12-16T11:30:46
#
#-------------------------------------------------

QT       += core gui svg
CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++1y
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sgvTEST
TEMPLATE = app


SOURCES += main.cpp\
        eye.cpp \
    thermometerSVG/thermometer.cpp

HEADERS  += eye.h \
    thermometerSVG/thermometer.h

FORMS    += eye.ui

CONFIG += mobility
MOBILITY = 

