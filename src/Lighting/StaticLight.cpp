#include "StaticLight.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/QWorld.hpp"
#include "Utility/DisplayItem.hpp"
#include "Utility/Utility.hpp"
#include "LightSystem.hpp"
#include <QJsonObject>

StaticLight::StaticLight(Item* parent):
    Light(parent),
    m_dynamicShadows(),
    m_dynamicLight() {

    addFixture(&m_fixture);

    m_fixture.setSensor(true);
    m_fixture.setVisible(false);
    m_fixture.setShadowCaster(false);
}

StaticLight::~StaticLight() {
    if (dynamicLight()) {
        dynamicLight()->bindLight(nullptr);
    }

    if (lightSystem())
        lightSystem()->removeLight(this);
}

void StaticLight::initialize(QWorld* w) {
    m_fixture.setPosition(QPointF(-radius(), -radius()));
    m_fixture.setSize(QSizeF(2*radius(), 2*radius()));

    lightSystem()->addLight(this);

    QBody::initialize(w);
}

bool StaticLight::read(const QJsonObject& obj) {
    Light::read(obj);
    setDynamicShadows(obj["dynamicShadows"].toBool());

    return true;
}

bool StaticLight::write(QJsonObject& obj) const {
    Light::write(obj);
    obj["dynamicShadows"] = dynamicShadows();

    return true;
}

void StaticLight::synchronize() {
    setTransform(position(), rotation());
}

void StaticLight::geometryChanged(const QRectF& newGeometry,
                                  const QRectF& oldGeometry) {
    //Light::geometryChanged(newGeometry, oldGeometry);

    if (dynamicLight()) {
        dynamicLight()->setPosition(position());
    }
}

/*void StaticLight::itemChange(ItemChange change, const ItemChangeData& data) {
    Light::itemChange(change, data);

    if (change == ItemVisibleHasChanged && lightSystem())
        lightSystem()->lightVisibilityChanged(this);
}*/

