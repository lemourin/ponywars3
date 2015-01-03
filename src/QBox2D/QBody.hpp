#ifndef QBODY_HPP
#define QBODY_HPP

#include <QQuickItem>
#include <Box2D/Box2D.h>
#include <queue>
#include "Utility/List.hpp"
#include "Utility/BaseItem.hpp"
#include "Geometry/Circle.hpp"

class QFixture;
class QWorld;

class QBody: public BaseItem {
    private:
        friend class QWorld;
        friend class QFixture;
        friend class QContactListener;

        b2Body* m_body;
        b2BodyDef m_bodyDef;

        List<QFixture*> m_fixtureList;
        QWorld* m_world;

        std::queue< std::function<void()> > m_work;

        void removeFixture(QFixture*);

    protected:
        virtual void synchronize();

        virtual void beginContact(QFixture* other, b2Contact*);
        virtual void endContact(QFixture* other, b2Contact*);
        virtual void preSolve(QFixture* other, b2Contact*, const b2Manifold*);
        virtual void postSolve(QFixture* other, b2Contact*, const b2ContactImpulse*);

    public:
        enum BodyType {
            Static,
            Kinematic,
            Dynamic
        };

        explicit QBody(SceneGraph::Item* parent = nullptr);
        ~QBody();

        virtual void initialize(QWorld*);
        void destroyBody();

        QPointF position() const;
        void setPosition(QPointF);

        inline qreal rotation() const { return m_bodyDef.angle*180.0/M_PI; }
        void setRotation(qreal);

        inline qreal linearDamping() const { return m_bodyDef.linearDamping; }
        void setLinearDamping(qreal linearDamping);

        inline qreal angularDamping() const { return m_bodyDef.angularDamping; }
        void setAngularDamping(qreal angularDamping);

        inline qreal gravityScale() const { return m_bodyDef.gravityScale; }
        void setGravityScale(qreal gravityScale);

        BodyType bodyType() const;
        void setBodyType(BodyType bodyType);

        inline bool isBullet() const { return m_bodyDef.bullet; }
        void setBullet(bool bullet);

        inline bool sleepingAllowed() const { return m_bodyDef.allowSleep; }
        void setSleepingAllowed(bool allowed);

        inline bool fixedRotation() const { return m_bodyDef.fixedRotation; }
        void setFixedRotation(bool fixedRotation);

        inline bool active() const { return m_bodyDef.active; }
        void setActive(bool active);

        QPointF linearVelocity() const;
        void setLinearVelocity(const QPointF &linearVelocity);

        inline qreal angularVelocity() const { return m_bodyDef.angularVelocity; }
        void setAngularVelocity(qreal velocity);

        void setTransform(QPointF position, qreal rotation);

        void addFixture(QFixture* f);
        QFixture* firstFixture() const;

        void applyLinearImpulse(const QPointF &impulse,
                                const QPointF &point);
        void applyTorque(qreal torque);
        void applyForce(const QPointF& force);
        QPointF worldCenter() const;

        inline const b2Body* body() const { return m_body; }
        inline b2Body* body() { return m_body; }

        inline QWorld* world() const { return m_world; }

        QRectF boundingRect() const;

        virtual bool testPoint(const QPointF& point) const;
        bool testOverlap(const QRectF& rect) const;
        bool testOverlap(const QBody* other) const;

        void initializeLater(QWorld*);
        void enqueueFunction(std::function<void()>);

        virtual bool read(const QJsonObject&);
        virtual bool write(QJsonObject&) const;

        static QBody* toQBody(b2Body *body);
        static const QBody* toQBody(const b2Body *body);
};

#endif //QBODY_HPP
