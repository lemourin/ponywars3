﻿#ifndef WORLD_HPP
#define WORLD_HPP

#include <QElapsedTimer>

#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QWorld.hpp"

#include "Actions/MainAction.hpp"

#include "Graphics/ParticleSystem.hpp"

class Game;
class Player;
class LightSystem;
class ViewWorld;
class World;

class WorldObject : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(bool player READ player NOTIFY playerChanged)
  Q_PROPERTY(uint playerHealth READ playerHealth NOTIFY playerHealthChanged)
  Q_PROPERTY(
      bool equippedWeapon READ equippedWeapon NOTIFY equippedWeaponChanged)
  Q_PROPERTY(uint bulletCount READ bulletCount NOTIFY bulletCountChanged)
  Q_PROPERTY(qreal fps READ fps NOTIFY fpsChanged)

  World *m_world;

 public:
  WorldObject(World *);

  bool player();

  uint playerHealth() const;
  bool equippedWeapon() const;
  uint bulletCount() const;

  qreal fps() const;

  Q_INVOKABLE void playerEnableGoLeft();
  Q_INVOKABLE void playerDisableGoLeft();

  Q_INVOKABLE void playerEnableGoRight();
  Q_INVOKABLE void playerDisableGoRight();

  Q_INVOKABLE void playerJumpRequested();
  Q_INVOKABLE void playerPunchRequested();

  Q_INVOKABLE void playerDropWeapon();

 signals:
  void playerChanged();
  void playerHealthChanged();
  void equippedWeaponChanged();
  void bulletCountChanged();
  void fpsChanged();
};

class WorldFileActionResolver : public FileActionResolver {
 private:
  World *m_world;

 public:
  WorldFileActionResolver(World *);

  void load(QString) const;
  void dump(QString) const;
};

class WorldMapEditorCallback : public MapEditorCallback {
 private:
  World *m_world;

 public:
  WorldMapEditorCallback(World *);
  void onTriggered();
};

class World : public QWorld {
 private:
  friend class ViewWorld;

  ViewWorld *m_viewWorld;
  std::unique_ptr<Player> m_player;
  MainAction m_mainAction;
  WorldObject m_worldObject;

 protected:
  void onBodyDestroyed(QBody *);
  void onBodyAdded(QBody *);
  void onFixtureDestroyed(QFixture *);
  void focusChanged();

 public:
  explicit World(ViewWorld *);
  ~World();

  void step();
  void clear();

  inline Player *player() const { return m_player.get(); }
  void setPlayer(std::unique_ptr<Player> p);

  inline ViewWorld *view() const { return m_viewWorld; }

  LightSystem *lightSystem() const;
  ParticleSystem *particleSystem() const;

  inline bool paused() const { return !isRunning(); }
  void setPaused(bool);

  inline MainAction *mainAction() { return &m_mainAction; }
  inline const MainAction *mainAction() const { return &m_mainAction; }

  inline WorldObject *object() { return &m_worldObject; }
  inline const WorldObject *object() const { return &m_worldObject; }

  void read(const QJsonObject &);
  void write(QJsonObject &) const;
};

#endif  // WORLD_HPP
