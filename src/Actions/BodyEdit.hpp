#ifndef BODYEDIT_HPP
#define BODYEDIT_HPP
#include "MapEditorAction.hpp"

class QBody;
class QMouseEvent;

class BodyEdit: public MapEditorAction {
    private:
        /*Q_PROPERTY(QBody* body READ body NOTIFY bodyChanged)
        Q_PROPERTY(qreal density READ density
                   WRITE setDensity NOTIFY densityChanged)
        Q_PROPERTY(qreal friction READ friction
                   WRITE setFriction NOTIFY frictionChanged)
        Q_PROPERTY(qreal restitution READ restitution
                   WRITE setRestitution NOTIFY restitutionChanged)
        Q_PROPERTY(qreal gravityScale READ gravityScale
                   WRITE setGravityScale NOTIFY gravityScaleChanged)
        Q_PROPERTY(qreal linearDamping READ linearDamping
                   WRITE setLinearDamping NOTIFY linearDampingChanged)
        Q_PROPERTY(qreal angularDamping READ angularDamping
                   WRITE setAngularDamping NOTIFY angularDampingChanged)
        Q_PROPERTY(bool dynamic READ dynamic
                   WRITE setDynamic NOTIFY dynamicChanged)*/

        QBody* m_body;
        qreal m_density;
        qreal m_friction;
        qreal m_restitution;
        qreal m_gravityScale;
        qreal m_linearDamping;
        qreal m_angularDamping;
        bool m_dynamic;

    protected:
        void mousePressEvent(QMouseEvent*);
        void mouseReleaseEvent(QMouseEvent*);

    public:
        BodyEdit(MapEditor*);
        void reset();

        inline QBody* body() const { return m_body; }
        void setBody(QBody*);

        inline qreal density() const { return m_density; }
        void setDensity(qreal);

        inline qreal friction() const { return m_friction; }
        void setFriction(qreal);

        inline qreal restitution() const { return m_restitution; }
        void setRestitution(qreal);

        inline qreal gravityScale() const { return m_gravityScale; }
        void setGravityScale(qreal);

        inline qreal linearDamping() const { return m_linearDamping; }
        void setLinearDamping(qreal);

        inline qreal angularDamping() const { return m_angularDamping; }
        void setAngularDamping(qreal);

        inline bool dynamic() const { return m_dynamic; }
        void setDynamic(bool);

        Q_INVOKABLE void applyChanges() const;

    //signals:
//        void bodyChanged();
//        void densityChanged();
//        void frictionChanged();
//        void restitutionChanged();
//        void gravityScaleChanged();
//        void linearDampingChanged();
//        void angularDampingChanged();
//        void dynamicChanged();

};

#endif // BODYEDIT_HPP
