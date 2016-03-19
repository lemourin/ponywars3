#include "World.hpp"

#include "Lighting/LightSystem.hpp"

#include "SceneGraph/Window.hpp"

#include "Utility/Factory.hpp"
#include "Utility/Utility.hpp"

#include "Entities/Game.hpp"
#include "Entities/Player.hpp"
#include "Entities/Pony.hpp"
#include "Entities/Gun.hpp"
#include "Entities/Deagle.hpp"
#include "Entities/Enemy.hpp"

#include "QBox2D/QBody.hpp"
#include "QBox2D/QChain.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/Fixture/Box2DChain.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include "QBox2D/Fixture/Box2DEdge.hpp"
#include "QBox2D/Fixture/Box2DPolygon.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <unordered_set>

World::World(ViewWorld *viewWorld)
    : QWorld(viewWorld), m_viewWorld(viewWorld), m_player(),
      m_mainAction(this, std::unique_ptr<FileActionResolver>(
                             new WorldFileActionResolver(this))),
      m_worldObject(this) {
  factory()->registerType<Box2DBox>("Box2DBox");
  factory()->registerType<Box2DChain>("Box2DChain");
  factory()->registerType<Box2DCircle>("Box2DCircle");
  factory()->registerType<Box2DEdge>("Box2DEdge");
  factory()->registerType<Box2DPolygon>("Box2DPolygon");

  factory()->registerType<QBody>("QBody");
  factory()->registerType<Gun>("Gun");
  factory()->registerType<Deagle>("Deagle");
  factory()->registerType<Enemy>("Enemy");
  factory()->registerType<QChain>("QChain");
  factory()->registerType<Player>("Player");
}

World::~World() { clear(); }

void World::step() {
  QWorld::step();

  particleSystem()->step();
}

void World::clear() {
  Player *item = player();
  setPlayer(nullptr);

  if (item) {
    item->destroyBody();
    delete item;
  }

  itemSet()->clear();
}

void World::onBodyDestroyed(QBody *body) {
  if (player() == body)
    setPlayer(nullptr);
  if (mainAction()->mapEditor()->grabItem()->m_grabbedBody == body)
    mainAction()->mapEditor()->grabItem()->releaseItem();

  QWorld::onBodyDestroyed(body);
}

void World::onBodyAdded(QBody *body) {
  QWorld::onBodyAdded(body);

  assert(lightSystem());
  lightSystem()->addBody(body);
}

void World::onFixtureDestroyed(QFixture *f) {
  lightSystem()->onFixtureDestroyed(f);
}

void World::setPlayer(Player *player) {
  if (m_player == player)
    return;

  m_player = player;

  if (player)
    player->setFocus(true);

  view()->setFocusedObject(player);

  emit object()->playerChanged();
  emit object()->equippedWeaponChanged();
}

LightSystem *World::lightSystem() const {
  return view()->game()->lightSystem();
}

ParticleSystem *World::particleSystem() const {
  return view()->game()->particleSystem();
}

void World::setPaused(bool p) {
  if (paused() == p)
    return;
  setRunning(!p);
  setFocus(!p);
}

void World::read(const QJsonObject &obj) {
  QWorld::read(obj);

  QJsonObject p = obj["player"].toObject();
  Player *player =
      factory()->create<Player>(p["class"].toString().toLocal8Bit());
  assert(player);

  player->setParent(this);
  static_cast<QBody*>(player)->initialize(p, this);

  setPlayer(player);
}

void World::write(QJsonObject &obj) const {
  QWorld::write(obj);

  if (player()) {
    QJsonObject p;
    player()->write(p);
    obj["player"] = p;
  }
}

WorldObject::WorldObject(World *world) : m_world(world), m_fps() {
  m_fpscounter.restart();
  connect(world->window(), &SceneGraph::Window::beforeRendering, this,
          &WorldObject::updateFps);
}

bool WorldObject::player() {
  return m_world->player() ? m_world->player()->focus() : false;
}

void WorldObject::updateFps() {
  qreal t = m_fpscounter.restart();

  if (!qFuzzyIsNull(t))
    setFps(1000.0 / t);
}

void WorldObject::setFps(qreal f) {
  m_fps = f;
  emit fpsChanged();
}

uint WorldObject::playerHealth() const {
  if (m_world->player() == nullptr)
    return 0;

  return m_world->player()->health();
}

bool WorldObject::equippedWeapon() const {
  if (m_world->player() == nullptr)
    return false;

  return m_world->player()->hand()->grabbedWeapon() != nullptr;
}

uint WorldObject::bulletCount() const {
  assert(equippedWeapon());
  return m_world->player()->hand()->grabbedWeapon()->bulletCount();
}

void WorldObject::playerEnableGoLeft() { m_world->player()->enableGoLeft(); }

void WorldObject::playerDisableGoLeft() { m_world->player()->disableGoLeft(); }

void WorldObject::playerEnableGoRight() { m_world->player()->enableGoRight(); }

void WorldObject::playerDisableGoRight() {
  m_world->player()->disableGoRight();
}

void WorldObject::playerJumpRequested() {
  if (m_world->player()->currentState() & Player::OnGround)
    m_world->player()->jumpRequested();
}

void WorldObject::playerPunchRequested() {
  m_world->player()->punchRequested();
}

void WorldObject::playerDropWeapon() {
  m_world->player()->hand()->dropWeapon();
}
WorldFileActionResolver::WorldFileActionResolver(World *w) : m_world(w) {}

void WorldFileActionResolver::load(QString path) const {
  m_world->view()->game()->load(path);
}

void WorldFileActionResolver::dump(QString path) const {
  m_world->view()->game()->dump(path);
}
