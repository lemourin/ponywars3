#include "Pony.hpp"
#include "World.hpp"
#include <QJsonObject>

Pony::Pony(SceneGraph::Item* parent):
    Creature(parent),
    m_body(this),
    m_wings(this) {
}

void Pony::initialize() {
    Creature::initialize();

    m_body.initialize(m_bodySource.c_str());
    m_body.setX(-0.5*width());
    m_body.setY(-0.5*height());
    m_body.setWidth(width());
    m_body.setHeight(height());

    m_wings.initialize(m_wingsSource.c_str());
    m_wings.setX(-0.5*width());
    m_wings.setY(-0.5*height());
    m_wings.setWidth(width());
    m_wings.setHeight(height());

    setCurrentSprite("standStill");
}

void Pony::setCurrentSprite(const char* sprite) {
    m_body.setCurrentSprite(sprite);
    m_wings.setCurrentSprite(sprite);
}

void Pony::jumpRequested() {
    setCurrentSprite("prepareJump");
}

void Pony::punchRequested() {
    setCurrentSprite("preparePunch");
}

bool Pony::read(const QJsonObject& obj) {
    Creature::read(obj);

    m_bodySource = obj["bodySource"].toString().toStdString();
    m_wingsSource = obj["wingsSource"].toString().toStdString();

    return true;
}

bool Pony::write(QJsonObject& obj) const {
    Creature::write(obj);

    obj["bodySource"] = QString(m_bodySource.c_str());
    obj["wingsSource"] = QString(m_wingsSource.c_str());

    return true;
}

void Pony::currentStateChanged(unsigned added, unsigned removed) {
    Creature::currentStateChanged(added, removed);

    m_body.updateState(added, removed);
    m_wings.updateState(added, removed);
}
