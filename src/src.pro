TEMPLATE = lib

QT += quick network
CONFIG += c++11

OBJECTS_DIR = .obj
MOC_DIR = .moc

QMAKE_CXXFLAGS += -fno-strict-aliasing

LIBS += -L$$OUT_PWD/../build -lBox2D -lSceneGraph -lGameEngine

INCLUDEPATH += $$PWD/../deps/ $$PWD/../deps/GameEngine

DESTDIR = $$OUT_PWD/../build
TARGET = src

SOURCES += \
    Entities/Creature.cpp \
    Entities/Deagle.cpp \
    Entities/Enemy.cpp \
    Entities/Game.cpp \
    Entities/Gun.cpp \
    Entities/Hand.cpp \
    Entities/ItemSet.cpp \
    Entities/Player.cpp \
    Entities/Pony.cpp \
    Entities/PonyAnimation.cpp \
    Entities/ViewWorld.cpp \
    Entities/Weapon.cpp \
    Entities/World.cpp \
    Utility/Window.cpp

HEADERS += \
    Entities/Creature.hpp \
    Entities/Deagle.hpp \
    Entities/Enemy.hpp \
    Entities/Game.hpp \
    Entities/Gun.hpp \
    Entities/Hand.hpp \
    Entities/ItemSet.hpp \
    Entities/Player.hpp \
    Entities/Pony.hpp \
    Entities/PonyAnimation.hpp \
    Entities/ViewWorld.hpp \
    Entities/Weapon.hpp \
    Entities/World.hpp \
    Utility/Window.hpp

