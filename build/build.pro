TEMPLATE = app
QT += quick widgets
CONFIG += c++14
CONFIG -= debug_and_release
QMAKE_CXXFLAGS += -fno-strict-aliasing
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
INCLUDEPATH += ../src/GameEngine ../src/GameEngine/Box2D
LIBS += -L$$OUT_PWD/../../src/ -lsrc
TARGET = PonyWars3
SOURCES += main.cpp

RESOURCES += \
    resources.qrc \
    json.qrc

OTHER_FILES += \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

unix|win32: LIBS += -L$$OUT_PWD/../src/ -lsrc
INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

unix|win32: LIBS += -L$$OUT_PWD/../src/GameEngine/Box2D/ -lBox2D
INCLUDEPATH += $$PWD/../src/GameEngine/Box2D
DEPENDPATH += $$PWD/../src/GameEngine/Box2D

unix|win32: LIBS += -L$$OUT_PWD/../src/GameEngine/ -lGameEngine
INCLUDEPATH += $$PWD/../src/GameEngine
DEPENDPATH += $$PWD/../src/GameEngine

unix|win32: LIBS += -L$$OUT_PWD/../src/GameEngine/SceneGraph/ -lSceneGraph
INCLUDEPATH += $$PWD/../src/GameEngine/SceneGraph
DEPENDPATH += $$PWD/../src/GameEngine/SceneGraph
