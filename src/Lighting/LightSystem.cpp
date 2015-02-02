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
    m_lightTexture(this),
    m_enlightedItems(this, lightTexture()->sourceItem()) {
    //setVisible(false);
}

LightSystem::~LightSystem() {
    for (StaticLight* light: m_light) {
        if (light->dynamicLight())
            light->dynamicLight()->setLightSystem(nullptr);

        light->setLightSystem(nullptr);
    }

    for (StaticLight* light: m_loadedLights)
        delete light;

    for (DynamicLight* light: m_unusedLight)
        light->setLightSystem(nullptr);
}

void LightSystem::read(const QJsonObject& obj) {
    QJsonArray array = obj["lights"].toArray();

    for (int i=0; i<array.size(); i++) {
        QJsonObject lightData = array[i].toObject();

        StaticLight* light = new StaticLight;
        light->setLightSystem(this);
        light->read(lightData);
        light->initialize(world());

        m_loadedLights.insert(light);
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

    visibleAreaChanged();
}

void LightSystem::setSize(QSizeF s) {
    m_size = s;

    resetTransform();
    scale(m_size.width(), m_size.height());
}

void LightSystem::setResolution(QSize s) {
    m_resolution = s;

    for (SceneGraph::ShaderSource& fbo: m_framebuffer)
        fbo.setTextureSize(m_resolution);
    normalMap()->setTextureSize(m_resolution);
    lightTexture()->setTextureSize(m_resolution);
}

World* LightSystem::world() const {
    assert(m_game->view());
    return m_game->view()->world();
}

void LightSystem::addLight(StaticLight* light) {
    m_light.push_back(light);
    light->setParent(nullptr);
}

void LightSystem::removeLight(StaticLight* light) {
    light->setVisible(false);

    auto it1 = std::find(m_light.begin(), m_light.end(), light);
    assert(it1 != m_light.end());
    m_light.erase(it1);

    auto it2 = m_loadedLights.find(light);
    if (it2 != m_loadedLights.end())
        m_loadedLights.erase(it2);
}

void LightSystem::lightVisibilityChanged(StaticLight* light) {
    if (light->visible()) {
        m_visibleLights.insert(light);
        light->setParent(lightTexture()->sourceItem());
    }
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
        if (light->visible() && light->dynamicShadows() && !m_unusedLight.empty()) {
            DynamicLight* p = m_unusedLight.back();
            m_unusedLight.pop_back();

            p->setVisible(true);
            p->bindLight(light);

            light->setParent(nullptr);
        }
    }
}

void LightSystem::visibleAreaChanged() {
    QRectF rect = world()->view()->visibleArea();

    for (SceneGraph::ShaderSource& i: m_framebuffer)
        i.setSourceRect(rect);

    normalMap()->setSourceRect(rect);
    lightTexture()->setSourceRect(rect);
}

void LightSystem::addBody(QBody* body) {
    body->content()->setParent(normalMap()->sourceItem());
}

SceneGraph::Node *LightSystem::synchronize(SceneGraph::Node *old) {
    LightBlender* node = static_cast<LightBlender*>(old);
    if (!node) {
        node = new LightBlender;

        SceneGraph::ShaderSource* array[DYNAMIC_LIGHTS_COUNT];
        for (uint i=0; i<DYNAMIC_LIGHTS_COUNT; i++)
            array[i] = &m_framebuffer[i];

        node->material()->setLights(array);
        node->material()->setAmbient(QColor(20, 20, 20, 0));
        node->material()->setLightTexture(lightTexture());
    }

    return node;
}

void LightSystem::initializeDynamicLights() {
    for (uint i=0; i<DYNAMIC_LIGHTS_COUNT; i++) {
        m_framebuffer[i].setParent(this);

        m_dynamicLight[i].setParent(m_framebuffer[i].sourceItem());
        m_dynamicLight[i].setLightSystem(this);
        m_dynamicLight[i].setVisible(false);
        m_dynamicLight[i].initialize(world());

        m_unusedLight.push_back(&m_dynamicLight[i]);
    }
}
