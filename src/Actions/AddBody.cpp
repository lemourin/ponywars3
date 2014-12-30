#include "AddBody.hpp"
#include "AddPolygon.hpp"
#include "AddCircle.hpp"
#include "Geometry/Primitives.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QFixture.hpp"
#include "Entities/World.hpp"
#include <QSGTransformNode>
#include <QSGFlatColorMaterial>

AddBody::AddBody(MapEditor* p):
    MapEditorAction(p),
    m_addPolygon(this),
    m_addCircle(this),
    m_addRectangle(this),
    m_action({ addPolygon(), addCircle(), addRectangle() }){
    //setFlag(ItemIsFocusScope);
    //setAcceptedMouseButtons(Qt::LeftButton);
}

void AddBody::finished() {
    QBody* body = new QBody(world());

    for (QFixture* f: m_fixtures)
        body->addFixture(f);
    body->initialize();

    m_fixtures.clear();
}

void AddBody::finishedSubaction(AddFixture* action) {
    QFixture* fixture = action->fixture();
    if (fixture) {
        //fixture->setParent(this);
        //fixture->setFlag(ItemHasContents);
        m_fixtures.push_back(fixture);
    }

    action->reset();
}

void AddBody::actionFocusChanged(AddFixture* action) {
    //QString name = action->metaObject()->className();
    /*if (action->hasFocus())
        setCurrentAction(name);
    else if (currentAction() == name)
        setCurrentAction("");*/
}

void AddBody::setCurrentAction(QString name) {
    if (m_currentAction == name)
        return;
    m_currentAction = name;
    //emit currentActionChanged();
}

void AddBody::geometryChanged(const QRectF& newGeometry,
                              const QRectF& oldGeometry) {
    /*Action::geometryChanged(newGeometry, oldGeometry);

    for (Action* action: m_action) {
        action->setWidth(width());
        action->setHeight(height());
    }*/
}

void AddBody::reset() {
    Action::reset();

    for (QFixture* f: m_fixtures)
        delete f;
    m_fixtures.clear();
}

