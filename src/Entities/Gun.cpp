#include "Gun.hpp"
#include <QJsonObject>
#include "QBox2D/QChain.hpp"
#include "World.hpp"

Gun::Gun(SceneGraph::Item *parent) : Weapon(parent) {}

bool Gun::write(QJsonObject &obj) const {
  Weapon::write(obj);
  obj["class"] = QString("Gun");

  return true;
}

void Gun::shoot() {
  if (bulletCount() <= 0) return;
  setBulletCount(bulletCount() - 1);

  Circle circle((Vector2d)(matrix() * effectiveShootPoint()), 0.5);

  auto bullet = std::make_unique<Bullet>(circle, world());
  bullet->initialize(world());

  float angle = body()->GetAngle();
  bullet->applyForce(QPointF(cos(angle) * 5000, sin(angle) * 5000));

  static_cast<World *>(world())->itemSet()->addBody(std::move(bullet));
}

Bullet::Bullet(Circle circle, Item *p) : QBody(p) {
  setPosition(QPointF(circle.pos()));
  setLinearDamping(0);
  setBullet(true);
  setBodyType(Dynamic);

  auto fixture = std::make_unique<Box2DCircle>(this);
  fixture->setShadowCaster(false);
  fixture->setRadius(circle.radius());
  fixture->setGroupIndex(-1);

  addFixture(std::move(fixture));
}

void Bullet::beginContact(QFixture *other, b2Contact *) {
  if (other->isSensor()) return;

  destroyLater();

  ParticleSystem *p = static_cast<World *>(world())->particleSystem();
  p->addExplosion(worldCenter(), 5, 0.1, 20);

  if (QChain *chain = dynamic_cast<QChain *>(other->body())) {
    chain->cutCircle(Circle(Vector2d(worldCenter()), 5));
  }
}
