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
    materials.cpp \
    character.cpp \
    pc.cpp \
    event_handler.cpp

HEADERS += \
    room.hh \
    coordinates.hh \
    nc_drawing_functions.hh \
    world_object.hh \
    materials.hh \
    character.hh \
    pc.hh \
    ../event_handler.h \
    event_handler.hh

