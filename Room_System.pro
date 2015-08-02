TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += C++11

QMAKE_LFLAGS += -lpanel
QMAKE_LFLAGS += -lcurses

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    room.cpp \
    coordinates.cpp \
    nc_drawing_functions.cpp \
    entity.cpp \
    Components/component.cpp \
    Components/coordinate_comps.cpp \
    entity_manager.cpp \
    Entity_archetypes/entity_archetype.cpp \
    event_manager.cpp

HEADERS += \
    room.hh \
    coordinates.hh \
    nc_drawing_functions.hh \
    entity.hh \
    Components/component.hh \
    Components/coordinate_comps.hh \
    Components/components_include.hh \
    entity_manager.hh \
    Entity_archetypes/entity_archetype.hh \
    helper_functions.hh \
    event_manager.hh

