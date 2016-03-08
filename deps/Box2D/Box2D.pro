TEMPLATE = lib
QT =

CONFIG += warn_off
QMAKE_CXXFLAGS += -fno-strict-aliasing

INCLUDEPATH += ..
OBJECTS_DIR = .obj

DESTDIR = $$OUT_PWD/../../build
TARGET = Box2D

SOURCES += \
    Collision/b2CollideEdge.cpp \
    Collision/b2CollideCircle.cpp \
    Collision/b2Distance.cpp \
    Collision/b2CollidePolygon.cpp \
    Collision/Shapes/b2PolygonShape.cpp \
    Collision/Shapes/b2CircleShape.cpp \
    Collision/Shapes/b2EdgeShape.cpp \
    Collision/Shapes/b2ChainShape.cpp \
    Collision/b2TimeOfImpact.cpp \
    Collision/b2Collision.cpp \
    Collision/b2DynamicTree.cpp \
    Collision/b2BroadPhase.cpp \
    Common/b2StackAllocator.cpp \ 
    Common/b2Draw.cpp \
    Common/b2BlockAllocator.cpp \
    Common/b2Settings.cpp \
    Common/b2Math.cpp \
    Common/b2Timer.cpp \
    Rope/b2Rope.cpp \
    Dynamics/b2Island.cpp \
    Dynamics/b2World.cpp \
    Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    Dynamics/Contacts/b2Contact.cpp \ 
    Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    Dynamics/Contacts/b2ContactSolver.cpp \
    Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    Dynamics/Contacts/b2CircleContact.cpp \
    Dynamics/Contacts/b2PolygonContact.cpp \
    Dynamics/b2Body.cpp \
    Dynamics/b2WorldCallbacks.cpp \
    Dynamics/b2Fixture.cpp \
    Dynamics/Joints/b2MouseJoint.cpp \ 
    Dynamics/Joints/b2FrictionJoint.cpp \
    Dynamics/Joints/b2PrismaticJoint.cpp \
    Dynamics/Joints/b2GearJoint.cpp \ 
    Dynamics/Joints/b2WheelJoint.cpp \
    Dynamics/Joints/b2Joint.cpp \ 
    Dynamics/Joints/b2PulleyJoint.cpp \
    Dynamics/Joints/b2RevoluteJoint.cpp \
    Dynamics/Joints/b2WeldJoint.cpp \
    Dynamics/Joints/b2DistanceJoint.cpp \
    Dynamics/Joints/b2MotorJoint.cpp \
    Dynamics/Joints/b2RopeJoint.cpp \
    Dynamics/b2ContactManager.cpp

HEADERS += \
    Collision/b2TimeOfImpact.h \
    Collision/b2DynamicTree.h \
    Collision/b2Distance.h \
    Collision/b2Collision.h \
    Collision/b2BroadPhase.h \
    Collision/Shapes/b2Shape.h \
    Collision/Shapes/b2PolygonShape.h \
    Collision/Shapes/b2EdgeShape.h \
    Collision/Shapes/b2CircleShape.h \
    Collision/Shapes/b2ChainShape.h \
    Common/b2Timer.h \
    Common/b2StackAllocator.h \
    Common/b2Settings.h \
    Common/b2Math.h \
    Common/b2GrowableStack.h \
    Common/b2Draw.h \
    Common/b2BlockAllocator.h \
    Dynamics/b2WorldCallbacks.h \
    Dynamics/b2World.h \
    Dynamics/b2TimeStep.h \
    Dynamics/b2Island.h \
    Dynamics/b2Fixture.h \
    Dynamics/b2ContactManager.h \
    Dynamics/b2Body.h \
    Dynamics/Joints/b2WheelJoint.h \
    Dynamics/Joints/b2WeldJoint.h \
    Dynamics/Joints/b2RopeJoint.h \
    Dynamics/Joints/b2RevoluteJoint.h \
    Dynamics/Joints/b2PulleyJoint.h \
    Dynamics/Joints/b2PrismaticJoint.h \
    Dynamics/Joints/b2MouseJoint.h \
    Dynamics/Joints/b2MotorJoint.h \
    Dynamics/Joints/b2Joint.h \
    Dynamics/Joints/b2GearJoint.h \
    Dynamics/Joints/b2FrictionJoint.h \
    Dynamics/Joints/b2DistanceJoint.h \
    Rope/b2Rope.h \
    Box2D.h \
    Dynamics/Contacts/b2ChainAndCircleContact.h \
    Dynamics/Contacts/b2ChainAndPolygonContact.h \
    Dynamics/Contacts/b2CircleContact.h \
    Dynamics/Contacts/b2Contact.h \
    Dynamics/Contacts/b2ContactSolver.h \
    Dynamics/Contacts/b2EdgeAndCircleContact.h \
    Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    Dynamics/Contacts/b2PolygonAndCircleContact.h \
    Dynamics/Contacts/b2PolygonContact.h

