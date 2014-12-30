#ifndef GUN_HPP
#define GUN_HPP
#include "Weapon.hpp"
#include "Geometry/Circle.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"

class Box2DBox;

class Gun: public Weapon {
    public:
        Gun(Item* = nullptr);

        void shoot();

};

class Bullet: public QBody {
    protected:
        void beginContact(QFixture* other, b2Contact *);

    public:
        Bullet(Circle circle, SceneGraph::Item* = nullptr);
};

#endif // GUN_HPP
