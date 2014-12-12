TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

QMAKE_LFLAGS += -lpanel
QMAKE_LFLAGS += -lcurses

SOURCES += main.cpp \
    room.cpp \
    coordinates.cpp \
    nc_drawing_functions.cpp \
    world_object.cpp \
    actor.cpp

HEADERS += \
    room.hh \
    coordinates.hh \
    nc_drawing_functions.hh \
    world_object.hh \
    actor.hh

