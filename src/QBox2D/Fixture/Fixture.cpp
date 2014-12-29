#include "Fixture.hpp"
#include "Entities/World.hpp"
#include "Lighting/LightSystem.hpp"
#include <QJsonObject>

Fixture::Fixture(QQuickItem *parent):
    QFixture(parent),
    m_texture(),
    m_normalMap() {
}

Fixture::~Fixture() {
    m_texture = nullptr;
    m_normalMap = nullptr;
}

void Fixture::updatePosition() {
    if (normalMap() && body())
        normalMap()->setPosition(body()->position()+position());
}

void Fixture::updateRotation() {
    if (normalMap() && body())
        normalMap()->setRotation(body()->rotation());
}

void Fixture::updateSize() {
    if (texture())
        texture()->setSize(QSizeF(width(), height()));
    if (normalMap())
        normalMap()->setSize(QSizeF(width(), height()));
}

void Fixture::updateVisibility() {
    if (normalMap())
        normalMap()->setVisible(isVisible());
}

void Fixture::initialize() {
    QFixture::initialize();

    if (normalMap()) {
        World* w = static_cast<World*>(body()->world());

        assert(w->lightSystem());
        normalMap()->setParentItem(w->lightSystem()->normalMap()->sourceItem());
        normalMap()->setTransformOrigin(QQuickItem::TopLeft);
    }

    updatePosition();
    updateRotation();
    updateSize();
    updateVisibility();
}

void Fixture::geometryChanged(const QRectF& newGeometry,
                              const QRectF& oldGeometry) {
    QFixture::geometryChanged(newGeometry, oldGeometry);

    updatePosition();
    updateSize();
}

void Fixture::itemChange(ItemChange change, const ItemChangeData& data) {
    QFixture::itemChange(change, data);

    if (change == ItemVisibleHasChanged)
        updateVisibility();
}

void Fixture::bodyPositionChanged() {
    updatePosition();
}

void Fixture::bodyRotationChanged() {
    updateRotation();
}

QString Fixture::textureSource() const {
    return texture() ? texture()->source() : "";
}

void Fixture::setTextureSource(QString path) {
    if (texture())
        texture()->setSource(path);
}

QString Fixture::normalMapSource() const {
    return normalMap() ? normalMap()->source() : "";
}

void Fixture::setNormalMapSource(QString path) {
    if (normalMap())
        normalMap()->setSource(path);
}

bool Fixture::read(const QJsonObject& obj) {
    QFixture::read(obj);

    setTextureSource(obj["textureSource"].toString());
    setNormalMapSource(obj["normalMapSource"].toString());
    return true;
}

bool Fixture::write(QJsonObject& obj) const {
    QFixture::write(obj);

    obj["textureSource"] = textureSource();
    obj["normalMapSource"] = normalMapSource();
    return true;
}

