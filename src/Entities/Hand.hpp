#ifndef HAND_HPP
#define HAND_HPP
#include "Weapon.hpp"
#include "QBox2D/QJoint.hpp"

class Player;

class Hand: public SceneGraph::Item {
    private:
        Q_OBJECT

        friend class Weapon;
        friend class Player;

        Q_PROPERTY(Weapon* grabbedWeapon READ grabbedWeapon
                   NOTIFY grabbedWeaponChanged)

        Player* m_owner;
        Weapon* m_grabbedWeapon;
        QMouseJoint* m_mouseJoint;
        float m_range;

        void updateGrabbedWeapon();

    public:
        Hand(Player* owner = nullptr);
        ~Hand();

        void setHandPosition(QPointF pos);

        inline Player* owner() const { return m_owner; }

        inline Weapon* grabbedWeapon() const { return m_grabbedWeapon; }
        void setGrabbedWeapon(Weapon* weapon);

        inline float range() const { return m_range; }
        inline void setRange(float range) { m_range = range; }

        void grabWeapon(Weapon* item);
        Q_INVOKABLE void dropWeapon();

    signals:
        void grabbedWeaponChanged();

};
#endif // HAND_HPP
