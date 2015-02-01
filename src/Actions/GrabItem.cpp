#include "GrabItem.hpp"
#include "Entities/World.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QFixture.hpp"
#include "Geometry/Edge.hpp"
#include "Geometry/Vector2d.hpp"
#include "Geometry/Circle.hpp"
#include "Entities/ViewWorld.hpp"
#include <Box2D/Box2D.h>

GrabItem::GrabItem(MapEditor* parent):
    SubAction(parent),
    m_mouseJoint(),
    m_grabbedBody(),
    m_pressed(),
    m_object(this) {
    setVisible(false);
}

GrabItem::~GrabItem() {
    destroyJoint();
}

bool GrabItem::pickItem(QPointF point) {
    QWorld* w = world();

    m_grabbedBody = w->bodyUnderPoint(point);

    if (!m_grabbedBody)
        return false;

    m_mouseJoint = new QMouseJoint(m_grabbedBody);
    m_mouseJoint->setMaxForce(1000*m_grabbedBody->body()->GetMass());
    m_mouseJoint->setTarget(point);
    m_mouseJoint->initialize();
    setVisible(true);

    return true;
}

void GrabItem::releaseItem() {
    destroyJoint();
    setVisible(false);
}

void GrabItem::setDirection(QPointF pos) {
    if (m_mouseJoint)
        m_mouseJoint->setTarget(pos);
}

void GrabItem::reset() {
    m_pressed = false;
    destroyJoint();
}

void GrabItem::destroyJoint() {
    delete m_mouseJoint;

    m_mouseJoint = nullptr;
    m_grabbedBody = nullptr;
}

SceneGraph::Node *GrabItem::synchronize(SceneGraph::Node *old) {
    ArrowNode* node = static_cast<ArrowNode*>(old);

    if (!node)
        node = new ArrowNode;

    if (m_mouseJoint && m_mouseJoint->joint()) {
        node->setP2(m_mouseJoint->anchorA());
        node->setP1(m_mouseJoint->anchorB());

        node->updateGeometry();
    }

    update();

    return node;

}

void GrabItem::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton)
        return event->ignore();

    if (pickItem(mapFromScreen(event->pos())))
        m_pressed = true;
    else
        event->ignore();
}

void GrabItem::mouseReleaseEvent(QMouseEvent* event) {
    if (!m_pressed || event->button() != Qt::LeftButton)
        event->ignore();
    else {
        m_pressed = false;
        releaseItem();
    }
}

void GrabItem::mouseMoveEvent(QMouseEvent* event) {
    if (!m_pressed)
        event->ignore();
    else {
        setDirection(mapFromScreen(event->pos()));
    }
}

ArrowNode::ArrowNode():
    m_geometry({ { 2, GL_FLOAT } }, 5, sizeof(QPointF)) {
    m_geometry.setDrawingMode(GL_LINE_STRIP);
    setGeometry(&m_geometry);
    setMaterial(&m_material);

    m_material.setColor(Qt::red);
}

void ArrowNode::updateGeometry() {
    m_geometry.vertexData<QPointF>()[0] = m_p1;
    m_geometry.vertexData<QPointF>()[1] = m_p2;

    Edge edge((Vector2d)m_p1, (Vector2d)m_p2);

    Vector2d v1 = edge.vector(M_PI/4, 5);
    m_geometry.vertexData<QPointF>()[2] = QPointF(m_p2.x()+v1.x, m_p2.y()+v1.y);

    m_geometry.vertexData<QPointF>()[3] = m_p2;

    Vector2d v2 = edge.vector(-M_PI/4, 5);
    m_geometry.vertexData<QPointF>()[4] = QPointF(m_p2.x()+v2.x, m_p2.y()+v2.y);

    m_geometry.updateVertexData();
}
