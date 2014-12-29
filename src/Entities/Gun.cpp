#include "Gun.hpp"
#include "World.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include <QJsonObject>

Gun::Gun(SceneGraph::Item* parent):
    Weapon(parent) {
}

void Gun::shoot() {
    /*if (bulletCount() <= 0)
        return;
    setBulletCount(bulletCount()-1);

    Circle circle((Vector2d)mapToItem(world(), effectiveShootPoint()), 0.5);

    Bullet* bullet = new Bullet(circle, world());
    bullet->initialize();

    float angle = body()->GetAngle();
    bullet->applyForce(QPointF(cos(angle)*5000, sin(angle)*5000));*/
}

Bullet::Bullet(Circle circle, Item* p): QBody(p) {
    setPosition(QPointF(circle.pos()));
    setLinearDamping(0);
    setBullet(true);
    setBodyType(Dynamic);

    Box2DCircle* f = new Box2DCircle(this);
    f->setShadowCaster(false);
    f->setRadius(circle.radius());
    f->setGroupIndex(-1);
    //f->setFlag(ItemHasContents);

    addFixture(f);
}

void Bullet::beginContact(QFixture* other, b2Contact*) {
    if (other->isSensor())
        return;

    //deleteLater();
    emit static_cast<World*>(world())->explosion(worldCenter());
}
