#include "GrabItem.hpp"
#include "Entities/World.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QFixture.hpp"
#include "Geometry/Edge.hpp"
#include "Geometry/Vector2d.hpp"
#include "Geometry/Circle.hpp"
#include <Box2D/Box2D.h>
#include <QSGFlatColorMaterial>

GrabItem::GrabItem(MapEditor* parent):
    MapEditorAction(parent),
    m_mouseJoint() {
    //setAcceptedMouseButtons(Qt::LeftButton);
    //setFlag(ItemHasContents);
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

    return true;
}

void GrabItem::releaseItem() {
    destroyJoint();
}

void GrabItem::setDirection(QPointF pos) {
    if (m_mouseJoint)
        m_mouseJoint->setTarget(pos);
}

void GrabItem::reset() {
    destroyJoint();
}

void GrabItem::destroyJoint() {
    delete m_mouseJoint;

    m_mouseJoint = nullptr;
    m_grabbedBody = nullptr;
}

/*QSGNode* GrabItem::updatePaintNode(QSGNode* n, UpdatePaintNodeData*) {
    ArrowNode* node = static_cast<ArrowNode*>(n);

    if (!node)
        node = new ArrowNode;

    if (m_mouseJoint && m_mouseJoint->joint()) {
        setOpacity(1);

        node->setP2(m_mouseJoint->anchorA());
        node->setP1(m_mouseJoint->anchorB());

        node->updateGeometry();
    }
    else {
        setOpacity(0);
    }

    update();

    return node;
}*/

void GrabItem::mousePressEvent(QMouseEvent* event) {
    //if (!pickItem(event->localPos()))
    //    return Action::mousePressEvent(event);

    event->accept();
}

void GrabItem::mouseReleaseEvent(QMouseEvent*) {
    releaseItem();
}

void GrabItem::mouseMoveEvent(QMouseEvent* event) {
    setDirection(event->localPos());
    event->accept();
}

ArrowNode::ArrowNode():
    m_geometry(QSGGeometry::defaultAttributes_Point2D(), 5) {
    m_geometry.setDrawingMode(GL_LINE_STRIP);
    setGeometry(&m_geometry);

    QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
    material->setColor(Qt::red);
    setMaterial(material);

    setFlags(OwnsMaterial);
}

void ArrowNode::updateGeometry() {
    m_geometry.vertexDataAsPoint2D()[0].set(m_p1.x(), m_p1.y());
    m_geometry.vertexDataAsPoint2D()[1].set(m_p2.x(), m_p2.y());

    Edge edge((Vector2d)m_p1, (Vector2d)m_p2);

    Vector2d v1 = edge.vector(M_PI/4, 5);
    m_geometry.vertexDataAsPoint2D()[2].set(m_p2.x()+v1.x, m_p2.y()+v1.y);

    m_geometry.vertexDataAsPoint2D()[3].set(m_p2.x(), m_p2.y());

    Vector2d v2 = edge.vector(-M_PI/4, 5);
    m_geometry.vertexDataAsPoint2D()[4].set(m_p2.x()+v2.x, m_p2.y()+v2.y);

    m_geometry.markVertexDataDirty();
    markDirty(DirtyGeometry);
}
