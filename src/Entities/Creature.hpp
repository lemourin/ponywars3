#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "QBox2D/QBody.hpp"

class Creature: public QBody {
    private:
//        Q_PROPERTY(int health READ health
//                   WRITE setHealth NOTIFY healthChanged)

        struct RayCastCallback: public b2RayCastCallback {
            QBody* m_hit;

            float32 ReportFixture(b2Fixture *fixture,
                                  const b2Vec2 &point,
                                  const b2Vec2 &normal,
                                  float32 fraction);
        };

        unsigned m_state;
        int m_health;
        QPointF m_force;

        void updateOnGround();
        void onWorldStepped();

    protected:
        virtual inline void onHealthChanged() { }
        virtual void currentStateChanged(unsigned added, unsigned removed);

    public:
        explicit Creature(Item* = nullptr);
        void initialize(QWorld*);

        enum State {
            GoingLeft = 1 << 0,
            GoingRight = 1 << 1,
            OnGround = 1 << 2,
            TurnedLeft = 1 << 3,
            TurnedRight = 1 << 4
        };

        inline void setForce(const QPointF& force) { m_force = force; }
        inline QPointF force() const { return m_force; }
        void addForce(const QPointF&);

        inline unsigned currentState() const { return m_state; }
        void modifyState(unsigned enable, unsigned disable);
        void enableState(unsigned state);
        void disableState(unsigned state);

        inline int health() const { return m_health; }
        void setHealth(int);

        bool read(const QJsonObject &);
        bool write(QJsonObject &) const;

        bool canSee(QBody*);

        Q_INVOKABLE void jump();
        Q_INVOKABLE void punch();

        Q_INVOKABLE void enableGoLeft();
        Q_INVOKABLE void enableGoRight();
        Q_INVOKABLE void disableGoLeft();
        Q_INVOKABLE void disableGoRight();

        Q_INVOKABLE virtual void jumpRequested();
        Q_INVOKABLE virtual void punchRequested();

    //signals:
    //    void healthChanged();
};

#endif // CREATURE_HPP
