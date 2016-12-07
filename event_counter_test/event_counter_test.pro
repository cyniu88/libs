TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    event_counters/event_counters.cpp \
    event_counters/event_counters_handler.cpp \
    event_counters/event/event_unknown.cpp \
    event_counters/event/new_connect_event.cpp \
    event_counters/event/event_mpd.cpp \
    event_counters/event/event_pilot.cpp \
    event_counters/event/event_command.cpp

HEADERS += \
    event_counters/event_counters.h \
    event_counters/event_counters_handler.h \
    event_counters/event/event_unknown.h \
    event_counters/event/new_connect_event.h \
    event_counters/event/event_mpd.h \
    event_counters/event/event_pilot.h \
    event_counters/event/event_command.h
