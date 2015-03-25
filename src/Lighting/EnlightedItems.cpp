#include "EnlightedItems.hpp"
#include "LightSystem.hpp"
#include "Light.hpp"
#include "QBox2D/QWorld.hpp"
#include "QBox2D/QFixture.hpp"
#include "Entities/Player.hpp"
#include "StaticLight.hpp"
#include "Entities/Game.hpp"
#include <cassert>

EnlightedItems::EnlightedItems(LightSystem* system, SceneGraph::Item* parent):
    SceneGraph::Item(parent),
    m_lightSystem(system) {
    //setVisible(false);
}

World *EnlightedItems::world() const {
    return lightSystem()->world();
}

SceneGraph::Node *EnlightedItems::synchronize(SceneGraph::Node *old) {
    QRectF visibleArea = world()->view()->visibleArea();

    Node* node = static_cast<Node*>(old);
    if (!node)
        node = new Node;
    while (node->firstChild())
        node->removeChild(node->firstChild());

    for (StaticLight* light: lightSystem()->visibleLights()) {
        if (!light->dynamicLight())
            continue;

        QRectF lightRect = light->matrix().mapRect(light->boundingRect());
        QRectF rect = visibleArea.intersected(lightRect);
        for (QFixture* f: world()->fixtures(rect)) {
            if (f->shadowCaster()) {
                SceneGraph::Node* enlightedNode = node->getEnlightedNode(f, light);
                node->appendChild(enlightedNode);
            }
        }
    }

    update();

    return node;
}

EnlightedNode::EnlightedNode(QFixture* fixture):
    m_geometry({ { 2, GL_FLOAT } },
               fixture->vertices().size()-1,
               sizeof(Vertex)) {

    m_geometry.setDrawingMode(GL_TRIANGLE_FAN);

    std::vector<QPointF> v = fixture->vertices();
    for (uint i=0; i<v.size()-1; i++)
        m_geometry.vertexData<Vertex>()[i] = { float(v[i].x()), float(v[i].y()) };
    m_geometry.updateVertexData();

    m_geometryNode.setGeometry(&m_geometry);
    m_geometryNode.setMaterial(&m_material);

    appendChild(&m_geometryNode);
}

void EnlightedNode::update(QFixture* fixture, Light* light) {
    updateMatrix(fixture);
    updateMaterial(light);
}

void EnlightedNode::updateMaterial(Light* light) {
    m_material.setNormalMap(light->lightSystem()->normalMap()->shaderNode());
    QVector3D p(light->position().x(), light->position().y(), light->z());
    m_material.setLightPosition(matrix().inverted()*p);
    m_material.setColor(light->color());
    m_material.setAttenuation(light->attenuation());
}

void EnlightedNode::updateMatrix(QFixture* fixture) {
    setMatrix(fixture->body()->matrix()*fixture->matrix());
}

EnlightedItems::Node::Node() {
}

EnlightedItems::Node::~Node() {
    for (const auto& pair: m_data)
        delete pair.second;
}

SceneGraph::Node* EnlightedItems::Node::getEnlightedNode(QFixture* fixture, Light* light) {
    std::pair<QFixture*, Light*> key = { fixture, light };
    if (m_data.find(key) == m_data.end()) {
        EnlightedNode* node = new EnlightedNode(fixture);
        node->update(fixture, light);
        return m_data[key] = node;
    }

    EnlightedNode* node = m_data[key];
    node->update(fixture, light);
    return node;
}

