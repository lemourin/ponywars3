#ifndef ADDCIRCLE_HPP
#define ADDCIRCLE_HPP
#include "AddFixture.hpp"
#include <QPointF>

class AddBody;

class AddCircle: public AddFixture {
    private:
        enum StateChange {
            PositionSet = 1 << 0,
            RadiusSet = 1 << 1,
            RadiusChanged = 1 << 2,
            ResetAction = 1 << 3
        };

        unsigned m_stateChange, m_state;
        QPointF m_position;
        float m_radius;
        ActionObject m_object;

    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        //void hoverMoveEvent(QHoverEvent *event);
        //QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    public:
        AddCircle(AddBody*);

        QFixture* fixture() const;
        void reset();

        inline QString name() const { return "AddCircle"; }
};

#endif // ADDCIRCLE_HPP
