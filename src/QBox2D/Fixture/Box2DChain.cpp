#include "Box2DChain.hpp"
#include "Geometry/Primitives.hpp"

Box2DChain::Box2DChain(SceneGraph::Item* parent): Box2DPolygon(parent) {
}

b2Shape* Box2DChain::createShape() const {
    int vertexCount = m_vertices.size()-1;

    b2ChainShape* chainShape = new b2ChainShape;
    b2Vec2 vertices[vertexCount];
    for (int i=0; i<vertexCount; i++)
        vertices[i].Set(m_vertices[i].x(), m_vertices[i].y());
    chainShape->CreateLoop(vertices, vertexCount);

    return chainShape;
}

QSGNode* Box2DChain::createNode() const {
    PolygonNode* node = new PolygonNode(std::vector<QPointF>(m_vertices.begin(),
                                                             m_vertices.end()-1));
    node->setColor(Qt::red);
    return node;
}

