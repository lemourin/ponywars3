#include "Weapon.hpp"
#include "Hand.hpp"
#include "Player.hpp"
#include "QBox2D/QWorld.hpp"
#include "QBox2D/QFixture.hpp"
#include "Utility/Utility.hpp"
#include "Geometry/Primitives.hpp"
#include "Geometry/Circle.hpp"
#include "Graphics/TexturedItem.hpp"
#include <QJsonObject>

Weapon::Weapon(Item* parent):
    QBody(parent),
    m_bulletCount(),
    m_flip(),
    m_grabber(),
    m_texture(this) {
}

Weapon::~Weapon() {
    if (grabber())
        grabber()->dropWeapon();
}

void Weapon::setBulletCount(int count) {
    if (m_bulletCount == count)
        return;
    m_bulletCount = count;
    //emit bulletCountChanged();
}

void Weapon::setFlip(int f) {
    m_flip = f;
    updateEffectiveShootPoint();
}

bool Weapon::read(const QJsonObject& obj) {
    QBody::read(obj);
    setBulletCount(obj["bulletCount"].toDouble());
    setShootPoint(Utility::Json::toPoint(obj["shootPoint"].toObject()));
    m_texture.setSource(obj["textureSource"].toString());
    m_texture.setTextureRect(Utility::Json::toRect(obj["textureRect"].toObject()));

    setSize(QSizeF(obj["width"].toDouble(), obj["height"].toDouble()));

    return true;
}

bool Weapon::write(QJsonObject& obj) const {
    QBody::write(obj);
    obj["bulletCount"] = bulletCount();
    obj["shootPoint"] = Utility::Json::toObject(shootPoint());
    obj["textureSource"] = m_texture.source();
    obj["textureRect"] = Utility::Json::toObject(m_texture.textureRect());
    obj["width"] = m_size.width();
    obj["height"] = m_size.height();

    return true;
}

void Weapon::initialize(QWorld* w) {
    QBody::initialize(w);

    for (QFixture* f = firstFixture(); f; f = f->next())
        f->setGroupIndex(-1);
}

void Weapon::setSize(QSizeF s) {
    m_size = s;

    m_texture.resetTransform();
    m_texture.scale(s.width(), s.height());
}

void Weapon::setShootPoint(QPointF p) {
    m_shootPoint = p;
    updateEffectiveShootPoint();
}

void Weapon::update() {
    if (!grabber())
        return;

    b2Vec2 dir = grabber()->owner()->body()->GetPosition() - body()->GetPosition();
    float fAngle = atan2(dir.y, dir.x);

    body()->SetAngularVelocity(0.0);
    body()->SetTransform(body()->GetPosition(), fAngle+M_PI);

    enqueueFunction(std::bind(&Weapon::update, this));
}

void Weapon::updateEffectiveShootPoint() {
    QMatrix4x4 matrix;

    if (flip() == -1) {
        matrix.scale(1, -1);
        //matrix.translate(0, -height());
        m_texture.setMatrix(matrix);
    }

    m_texture.setMatrix(matrix);
    setEffectiveShootPoint(matrix*shootPoint());
}

void Weapon::preSolve(QFixture*, b2Contact* contact, const b2Manifold*) {
    if (grabber())
        contact->SetEnabled(false);
}
