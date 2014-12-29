#include "Box2DEdge.hpp"
#include "Geometry/Primitives.hpp"

Box2DEdge::Box2DEdge(Item* parent): QFixture(parent) {
}

std::vector<QPointF> Box2DEdge::vertices() const {
    QPointF p1(m_edgeShape.m_vertex1.x, m_edgeShape.m_vertex1.y);
    QPointF p2(m_edgeShape.m_vertex2.x, m_edgeShape.m_vertex2.y);

    return { p1, p2 };
}

b2Shape* Box2DEdge::createShape() const {
    return new b2EdgeShape(m_edgeShape);
}

QSGNode* Box2DEdge::createNode() const {
    std::vector<QPointF> v = vertices();

    return new EdgeNode(v[0], v[1]);
}
