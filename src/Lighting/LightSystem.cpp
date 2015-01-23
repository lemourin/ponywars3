#include "LightSystem.hpp"
#include "StaticLight.hpp"
#include "Entities/Game.hpp"
#include "Utility/Utility.hpp"
#include <QSGTextureProvider>
#include <QSGDynamicTexture>
#include <QQmlEngine>
#include <QJsonArray>
#include <QJsonObject>
#include <cassert>

LightSystem::LightSystem(Game* game):
    SceneGraph::Item(game),
    m_game(game),
    m_resolution(),
    m_normalMap(this),
    m_lightTexture(this) {
}

LightSystem::~LightSystem() {
    for (StaticLight* light: m_light) {
        if (light->dynamicLight())
            light->dynamicLight()->setLightSystem(nullptr);

        light->setLightSystem(nullptr);
    }

    for (DynamicLight* light: m_unusedLight)
        light->setLightSystem(nullptr);
}

void LightSystem::read(const QJsonObject& obj) {
    QJsonArray array = obj["lights"].toArray();

    for (int i=0; i<array.size(); i++) {
        QJsonObject lightData = array[i].toObject();

        StaticLight* light = new StaticLight(this);
        light->setLightSystem(this);
        light->read(lightData);
        light->initialize(world());
    }
}

void LightSystem::write(QJsonObject& obj) const {
    QJsonArray array;
    for (StaticLight* light: m_light) {
        QJsonObject object;
        if (light->parent() == this && light->write(object)) {
            array.append(object);
        }
    }

    obj["lights"] = array;
}

const std::unordered_set<StaticLight*>& LightSystem::visibleLights() const {
    return m_visibleLights;
}

void LightSystem::initialize() {
    initializeDynamicLights();

    QColor color;
    color.setRgbF(0.5, 0.5, 1.0);
    m_normalMap.setBackground(color);

    /*QQuickItem* particleSystem = createParticleSystem();
    particleSystem->setParent(this);
    particleSystem->setParentItem(m_lightTexture.sourceItem());
    particleSystem->setProperty("lightSystem", QVariant::fromValue(this));

    m_enlightedItems.setParentItem(lightTexture()->sourceItem());
    m_enlightedItems.setLightSystem(this);
    m_enlightedItems.setWorld(world());*/

    visibleAreaChanged();
}

void LightSystem::setSize(QSizeF s) {
    m_size = s;

    resetTransform();
    scale(m_size.width(), m_size.height());
}

void LightSystem::setResolution(QSize s) {
    m_resolution = s;

    //for (SceneGraph::ShaderSource& fbo: m_framebuffer)
    //    fbo.setTextureSize(m_resolution);
    normalMap()->setTextureSize(m_resolution);
    lightTexture()->setTextureSize(m_resolution);
}

World* LightSystem::world() const {
    return m_game->view()->world();
}

void LightSystem::addLight(StaticLight* light) {
    m_light.push_back(light);
    initializeLight(light);
}

void LightSystem::removeLight(StaticLight* light) {
    light->setVisible(false);

    auto it = std::find(m_light.begin(), m_light.end(), light);
    if (it != m_light.end())
        m_light.erase(it);
}

void LightSystem::lightVisibilityChanged(StaticLight* light) {
    if (light->visible())
        m_visibleLights.insert(light);
    else
        m_visibleLights.erase(m_visibleLights.find(light));

    if (light->dynamicLight()) {
        if (!light->visible()) {
            DynamicLight* p = light->dynamicLight();
            p->setVisible(false);
            m_unusedLight.push_back(p);

            p->bindLight(nullptr);
        }
    }
    else {
        if (light-visible() && light->enabled() && light->dynamicShadows()) {
            if (!m_unusedLight.empty()) {
                DynamicLight* p = m_unusedLight.back();
                m_unusedLight.pop_back();

                p->bindLight(light);
                p->setVisible(true);
                p->update();

                //light->setOpacity(0);
            }
            else {
                //light->setOpacity(1);
            }
        }
    }
}

void LightSystem::visibleAreaChanged() {
    QRectF rect = world()->view()->visibleArea();
    normalMap()->setSourceRect(rect);
    lightTexture()->setSourceRect(rect);
}

void LightSystem::worldSizeChanged() {
    /*QSize size(world()->width(), world()->height());

    m_normalMap.sourceItem()->setSize(size);
    m_lightTexture.sourceItem()->setSize(size);

    for (uint i=0; i<DYNAMIC_LIGHTS_COUNT; i++) {
        m_framebuffer[i].sourceItem()->setSize(size);
        m_dynamicLight[i].setSize(size);
    }*/

}

void LightSystem::addBody(QBody* body) {
    body->content()->setParent(normalMap()->sourceItem());
}

QQuickItem* LightSystem::createParticleSystem() {
    const QUrl url("qrc:/lighting/qml/Lighting/ParticleSystem.qml");
    static QQmlComponent component(Utility::qmlEngine(), url);

    QObject* obj = component.create(Utility::qmlEngine()->rootContext());
    assert(obj);

    return static_cast<QQuickItem*>(obj);
}

SceneGraph::Node *LightSystem::synchronize(SceneGraph::Node *old) {
    LightBlender* node = static_cast<LightBlender*>(old);
    if (!node) {
        //QSGDynamicTexture* textureSet[DYNAMIC_LIGHTS_COUNT];
        //uint it = 0;
        //for (ShaderSource& fbo: m_framebuffer)
        //    textureSet[it++] = fbo.texture();

        node = new LightBlender;
        //node->material()->setLights(textureSet);
        node->material()->setAmbient(QColor(20, 20, 20, 0));
    }

    if (!normalMap()->shaderNode())
        update();
    else
        node->material()->setLightTexture(lightTexture()->shaderNode());

    return node;
}

void LightSystem::initializeDynamicLights() {
    for (uint i=0; i<DYNAMIC_LIGHTS_COUNT; i++) {
        //m_framebuffer[i].setParent(this);

        //m_dynamicLight[i].setParent(m_framebuffer[i].sourceItem());
        m_dynamicLight[i].setLightSystem(this);
        //m_dynamicLight[i].setWorld(world());
        m_dynamicLight[i].setVisible(false);

        m_unusedLight.push_back(&m_dynamicLight[i]);
    }
}

void LightSystem::initializeLight(StaticLight* light) {
    assert(lightTexture());
    light->setParent(lightTexture()->sourceItem());
    light->setVisible(false);
}

/*QSGNode* LightSystem::updatePaintNode(QSGNode* old, UpdatePaintNodeData*) {
    LightBlender* node = static_cast<LightBlender*>(old);
    if (!node) {
        QSGDynamicTexture* textureSet[DYNAMIC_LIGHTS_COUNT];
        uint it = 0;
        for (ShaderSource& fbo: m_framebuffer)
            textureSet[it++] = fbo.texture();

        node = new LightBlender;
        node->material()->setLights(textureSet);
        node->material()->setLightTexture(lightTexture()->texture());
        node->material()->setAmbient(QColor(20, 20, 20));
    }
    node->updateGeometry(this);
    update();

    return node;
}*/

/*void LightSystem::geometryChanged(const QRectF& newGeometry,
                                  const QRectF& oldGeometry) {
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    m_enlightedItems.setSize(newGeometry.size());

    for (ShaderSource& fbo: m_framebuffer)
        fbo.setSize(newGeometry.size());

    normalMap()->setSize(newGeometry.size());
    lightTexture()->setSize(newGeometry.size());
}*/
