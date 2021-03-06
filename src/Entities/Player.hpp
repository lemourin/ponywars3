﻿#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Hand.hpp"
#include "Lighting/StaticLight.hpp"
#include "Pony.hpp"

class World;

class Player : public Pony {
 private:
  friend class ViewWorld;

  Hand m_hand;
  StaticLight m_light;

 protected:
  void healthChanged();
  void focusChanged();
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);
  void beginContact(QFixture* other, b2Contact*);

  void synchronize();
  bool read(const QJsonObject&);

 public:
  Player(SceneGraph::Item* parent = nullptr);

  void initialize(QWorld*);
  void destroyBody();

  inline Hand* hand() { return &m_hand; }
  World* world() const;

  bool write(QJsonObject&) const;

  inline static Type* key() {
    static Type t;
    return &t;
  }

  inline Type* type() const { return key(); }
};

#endif  // PLAYER_HPP
