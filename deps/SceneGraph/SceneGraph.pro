QT = core gui quick
CONFIG += c++11

TARGET = scenegraph
TEMPLATE = lib

OBJECTS_DIR = .obj
MOC_DIR = .moc

DESTDIR = $$OUT_PWD/../../build

SOURCES += \
    BaseObject.cpp \
    Camera.cpp \
    Cube.cpp \
    DefaultRenderer.cpp \
    Geometry.cpp \
    Image.cpp \
    Item.cpp \
    Material.cpp \
    Node.cpp \
    Renderer.cpp \
    Shader.cpp \
    Window.cpp

HEADERS += \
    BaseObject.hpp \
    Camera.hpp \
    Cube.hpp \
    DefaultRenderer.hpp \
    Geometry.hpp \
    Image.hpp \
    Item.hpp \
    Material.hpp \
    Node.hpp \
    Renderer.hpp \
    Shader.hpp \
    Window.hpp
