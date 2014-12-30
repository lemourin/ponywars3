#include "EnlightedItems.hpp"
#include "LightSystem.hpp"
#include "Light.hpp"
#include "QBox2D/QWorld.hpp"
#include "QBox2D/QFixture.hpp"
#include "Entities/Player.hpp"
#include "StaticLight.hpp"
#include "Entities/Game.hpp"
#include <cassert>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QSGDynamicTexture>

EnlightedItems::EnlightedItems(Item* parent):
    SceneGraph::Item(parent),
    m_lightSystem(),
    m_world() {
    //setFlag(ItemHasContents);
}

/*QSGNode* EnlightedItems::updatePaintNode(QSGNode* old, UpdatePaintNodeData*) {
    QRectF visibleArea = world()->view()->visibleArea();

    Node* node = static_cast<Node*>(old);
    if (!node)
        node = new Node;
    node->removeAllChildNodes();

    for (StaticLight* light: lightSystem()->visibleLights()) {
        if (!light->enabled() || !light->dynamicLight())
            continue;
        QRectF lightRect = light->mapRectToScene(light->boundingRect());
        QRectF rect = visibleArea.intersected(lightRect);
        for (QFixture* f: world()->fixtures(rect)) {
            if (f->shadowCaster()) {
                QSGNode* enlightedNode = node->getEnlightedNode(f, light);
                node->appendChildNode(enlightedNode);
            }
        }
    }

    update();

    return node;
}*/

EnlightedNode::EnlightedNode(QFixture* fixture):
    m_geometry(QSGGeometry::defaultAttributes_Point2D(),
               fixture->vertices().size()-1) {

    m_geometry.setVertexDataPattern(QSGGeometry::StaticPattern);
    m_geometry.setDrawingMode(GL_TRIANGLE_FAN);

    std::vector<QPointF> v = fixture->vertices();
    for (uint i=0; i<v.size()-1; i++)
        m_geometry.vertexDataAsPoint2D()[i].set(v[i].x(), v[i].y());

    m_geometryNode.setGeometry(&m_geometry);
    m_geometryNode.setMaterial(&m_material);

    appendChildNode(&m_geometryNode);
}

void EnlightedNode::update(QFixture* fixture, Light* light) {
    updateMatrix(fixture);
    updateMaterial(light);
}

void EnlightedNode::updateMaterial(Light* light) {
    m_material.setNormalMap(light->lightSystem()->normalMap()->texture());
    QVector3D p(light->position().x(), light->position().y(), 0/*light->z()*/);
    m_material.setLightPosition(matrix().inverted()*p);
    m_material.setColor(light->color());
    m_material.setAttenuation(light->attenuation());

    m_geometryNode.markDirty(DirtyMaterial);
}

void EnlightedNode::updateMatrix(QFixture* fixture) {
    QMatrix4x4 matrix;
    matrix.translate(fixture->body()->position().x(),
                     fixture->body()->position().y());
    matrix.translate(fixture->position().x(),
                     fixture->position().y());
    matrix.rotate(fixture->body()->rotation(), 0, 0, 1);
    setMatrix(matrix);

    markDirty(DirtyMatrix);
}

EnlightedItems::Node::Node() {
}

EnlightedItems::Node::~Node() {
    for (const auto& pair: m_data)
        delete pair.second;
}

QSGNode* EnlightedItems::Node::getEnlightedNode(QFixture* fixture, Light* light) {
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

