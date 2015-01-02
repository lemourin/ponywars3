#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Hand.hpp"
#include "Pony.hpp"
#include "Lighting/StaticLight.hpp"

class World;

class Player: public Pony {
    private:
        friend class ViewWorld;

        //Q_PROPERTY(Hand* hand READ hand CONSTANT)

        //Hand m_hand;
        //StaticLight m_light;

        void mouseMoved(QPointF pos);
        void mousePressed(QPointF pos);
        void mouseReleased(QPointF pos);

    protected:
        void onHealthChanged();
        void focusOutEvent(QFocusEvent *);
        void keyPressEvent(QKeyEvent *event);
        void keyReleaseEvent(QKeyEvent *event);
        void beginContact(QFixture* other, b2Contact *);
        void geometryChanged(const QRectF &newGeometry,
                             const QRectF &oldGeometry);

    public:
        Player(SceneGraph::Item* parent = nullptr);
        void initialize(QWorld*);

        //inline Hand* hand() { return &m_hand; }
        World* world() const;

        bool read(const QJsonObject &);
        bool write(QJsonObject &) const;

};

#endif // PLAYER_HPP
