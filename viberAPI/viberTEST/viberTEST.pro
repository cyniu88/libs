TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lcurl
SOURCES += main.cpp \
    ../viber_api.cpp

HEADERS += \
    ../viber_api.h
