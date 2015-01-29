﻿#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Hand.hpp"
#include "Pony.hpp"
#include "Lighting/StaticLight.hpp"

class World;

class Player: public Pony {
    private:
        friend class ViewWorld;

        Hand m_hand;
        StaticLight m_light;

        void mouseMoved(QPointF pos);
        void mousePressed(QPointF pos);
        void mouseReleased(QPointF pos);

    protected:
        void healthChanged();
        void focusChanged();
        void keyPressEvent(QKeyEvent*);
        void keyReleaseEvent(QKeyEvent*);
        void mousePressEvent(QMouseEvent*);
        void beginContact(QFixture* other, b2Contact *);

        void synchronize();

    public:
        Player(SceneGraph::Item* parent = nullptr);
        ~Player();

        void initialize(QWorld*);

        inline Hand* hand() { return &m_hand; }
        World* world() const;

        bool read(const QJsonObject &);
        bool write(QJsonObject &) const;

};

#endif // PLAYER_HPP
