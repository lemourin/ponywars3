#include "AddCircle.hpp"
#include "AddBody.hpp"
#include "Geometry/Primitives.hpp"
#include "Geometry/Vector2d.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include "QBox2D/QWorld.hpp"
#include <QMouseEvent>
#include <QSGFlatColorMaterial>

AddCircle::AddCircle(AddBody* parent):
    AddFixture(parent),
    m_stateChange(),
    m_state() {
    //setAcceptedMouseButtons(Qt::LeftButton);
    //setAcceptHoverEvents(true);
    //setFlag(ItemHasContents);
}

QFixture* AddCircle::fixture() const {
    Box2DCircle* circle = new Box2DCircle;
    circle->translate(m_position.x(), m_position.y());
    circle->setRadius(m_radius);

    return circle;
}

void AddCircle::reset() {
    m_stateChange = ResetAction;
    m_state = 0;
    m_radius = 0;
    update();
}

void AddCircle::mousePressEvent(QMouseEvent*) {
}

void AddCircle::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (!(m_state & PositionSet)) {
            m_position = event->localPos();
            m_stateChange |= PositionSet;
            m_state |= PositionSet;
        }
        else {
            m_stateChange |= RadiusSet;
            m_state |= RadiusSet;
            emit finished();
        }

        update();
    }
}

/*void AddCircle::hoverMoveEvent(QHoverEvent* event) {
    if ((m_state & PositionSet) && !(m_state & RadiusSet)) {
        m_radius = Vector2d(m_position-event->posF()).length();
        m_stateChange |= RadiusChanged;
        update();
    }

    Action::hoverMoveEvent(event);
}*/

/*QSGNode* AddCircle::updatePaintNode(QSGNode* n, UpdatePaintNodeData*) {
    CircleNode* node = static_cast<CircleNode*>(n);

    if (m_stateChange & ResetAction) {
        m_stateChange ^= ResetAction;
        delete node;
        node = nullptr;
    }
    else if (m_stateChange) {
        m_stateChange = 0;

        delete node;
        node = new CircleNode(m_position, m_radius);

        QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
        material->setColor("yellow");
        node->setMaterial(material);
    }

    return node;
}*/
