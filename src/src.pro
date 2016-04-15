TEMPLATE = lib
QT += quick network
CONFIG += c++11
CONFIG -= debug_and_release
OBJECTS_DIR = .obj
MOC_DIR = .moc
INCLUDEPATH += GameEngine GameEngine/Box2D
TARGET = src
SOURCES += \
    Entities/Creature.cpp \
    Entities/Deagle.cpp \
    Entities/Enemy.cpp \
    Entities/Game.cpp \
    Entities/Gun.cpp \
    Entities/Hand.cpp \
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
    Entities/Player.hpp \
    Entities/Pony.hpp \
    Entities/PonyAnimation.hpp \
    Entities/ViewWorld.hpp \
    Entities/Weapon.hpp \
    Entities/World.hpp \
    Utility/Window.hpp

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/ -lGameEngine
INCLUDEPATH += $$PWD/GameEngine
DEPENDPATH += $$PWD/GameEngine

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/SceneGraph/ -lSceneGraph
INCLUDEPATH += $$PWD/GameEngine/SceneGraph
DEPENDPATH += $$PWD/GameEngine/SceneGraph

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/Box2D/ -lBox2D
INCLUDEPATH += $$PWD/GameEngine/Box2D
DEPENDPATH += $$PWD/GameEngine/Box2D
