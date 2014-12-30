#include "Box2DCircle.hpp"
#include "Geometry/Circle.hpp"
#include "Geometry/Primitives.hpp"

Box2DCircle::Box2DCircle(Item* parent):
    QFixture(parent),
    m_radius() {
}

std::vector<QPointF> Box2DCircle::vertices() const {
    const size_t accuracy = 8;

    std::vector<QPointF> v;
    for (size_t i=0; i<accuracy; i++) {
        qreal angle = i*2*M_PI/accuracy;
        v.push_back(QPointF(cos(angle)*radius(), sin(angle)*radius()));
    }

    v.push_back(v.front());

    return v;
}

b2Shape* Box2DCircle::createShape() const {
    b2CircleShape *shape = new b2CircleShape;
    shape->m_radius = m_radius;
    shape->m_p.Set(position().x(), position().y());

    return shape;
}

QSGNode* Box2DCircle::createNode() const {
    CircleNode* node = new CircleNode(QPointF(), radius());
    node->setColor(Qt::green);

    return node;
}
