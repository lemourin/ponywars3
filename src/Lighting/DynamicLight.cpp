#include "DynamicLight.hpp"
#include "StaticLight.hpp"
#include "LightSystem.hpp"
#include "Entities/Game.hpp"
#include "Entities/World.hpp"
#include <QQuickWindow>
#define GLSL(shader) #shader

const QSGGeometry::AttributeSet& ShadowNode::attributes() {
    static QSGGeometry::Attribute attrib[] = {
        QSGGeometry::Attribute::create(0, 4, GL_FLOAT, true)
    };

    static QSGGeometry::AttributeSet attribSet = {
        1,
        sizeof(QVector4D),
        attrib
    };

    return attribSet;
}

ShadowNode::ShadowNode(QPointF p1, QPointF p2):
    m_geometry(attributes(), 4) {

    setGeometry(&m_geometry);
    setMaterial(&m_material);

    m_geometry.setVertexDataPattern(QSGGeometry::DynamicPattern);
    m_geometry.setDrawingMode(GL_TRIANGLE_FAN);

    setVertices(p1, p2);
    setColor(Qt::black);
}

void ShadowNode::setVertices(QPointF p1, QPointF p2) {
    QVector4D* array = static_cast<QVector4D*>(m_geometry.vertexData());

    array[0] = QVector4D(p1.x(), p1.y(), 0.0, 1.0);
    array[1] = QVector4D(p2.x(), p2.y(), 0.0, 1.0);
    array[2] = QVector4D(p2.x(), p2.y(), 0.0, 0.0);
    array[3] = QVector4D(p1.x(), p1.y(), 0.0, 0.0);

    m_geometry.markVertexDataDirty();
    markDirty(DirtyGeometry);
}

void ShadowNode::setColor(QColor color) {
    m_material.setColor(color);
    markDirty(DirtyMaterial);
}

DynamicLight::DynamicLight(Item* item):
    Light(item),
    m_boundLight() {
}

DynamicLight::~DynamicLight() {
    if (m_boundLight) {
        m_boundLight->m_dynamicLight = nullptr;
        m_boundLight = nullptr;
    }
}

void DynamicLight::bindLight(StaticLight* light) {
    if (m_boundLight)
        m_boundLight->m_dynamicLight = nullptr;

    m_boundLight = light;

    if (!light) {
        setVisible(false);
        return;
    }

    setPosition(light->position());
    setRadius(light->radius());
    setColor(light->color());
    setAttenuation(light->attenuation());
    setRenderFraction(light->renderFraction());
    //setZ(light->z());

    light->m_dynamicLight = this;
}


bool DynamicLight::castingShadow(QPointF p1, QPointF p2) const {
    return p1.x()*p2.y()-p1.y()*p2.x() > 0;
}

/*QSGNode* DynamicLight::updatePaintNode(QSGNode* old, UpdatePaintNodeData*) {
    DynamicNode* rootNode = static_cast<DynamicNode*>(old);
    if (!rootNode)
        rootNode = new DynamicNode;
    rootNode->synchronize(this, static_cast<World*>(world())->view());

    if (isVisible())
        update();

    return rootNode;
}*/

DynamicLight::DynamicNode::DynamicNode() {
    //appendChildNode(&m_shadows);
}

DynamicLight::DynamicNode::~DynamicNode() {
    for (ShadowNode* node: m_unused)
        delete node;
    m_unused.clear();

    while (m_shadows.firstChild())
        delete m_shadows.firstChild();
}

void DynamicLight::DynamicNode::synchronize(DynamicLight* light, ViewWorld* display) {
    /*Light::Node::synchronize(light);

    while (m_shadows.firstChild()) {
        m_unused.push_back(static_cast<ShadowNode*>(m_shadows.firstChild()));
        m_shadows.removeChildNode(m_shadows.firstChild());
    }

    QRectF lightRect = light->mapRectToScene(light->boundingRect());
    QRectF rect = display->visibleArea().intersected(lightRect);
    for (QFixture* fixture: light->world()->fixtures(rect)) {
        if (!fixture->shadowCaster() || !fixture->body()->isVisible())
            continue;

        std::vector<QPointF> vertices;
        for (QPointF vert: fixture->vertices()) {
            QPointF p = display->mapFromItem(fixture, vert);
            vertices.push_back(light->mapFromScene(p));
        }

        for (uint i=0; i+1<vertices.size(); i++)
            makeShadowNode(light, vertices[i], vertices[i+1]);
    }*/
}

void DynamicLight::DynamicNode::makeShadowNode(DynamicLight* light,
                                               QPointF p1,
                                               QPointF p2) {
    if (!light->castingShadow(p1, p2))
        return;
    if (!m_unused.empty()) {
        ShadowNode* node = m_unused.back();
        m_unused.pop_back();

        node->setVertices(p1, p2);

        m_shadows.appendChildNode(node);

    }
    else {
        ShadowNode* node = new ShadowNode(p1, p2);
        m_shadows.appendChildNode(node);
    }
}

